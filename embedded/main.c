#define _GNU_SOURCE
#include <stdio.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <signal.h>
#include <assert.h>
#include <errno.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>
#include <sys/prctl.h>
#include <stdlib.h>
#include <ctype.h>
#include <time.h>
#include <stdbool.h>

#include <system_server.h>
#include <ui.h>
#include <input.h>
#include <web_server.h>
#include <utils.h>
#include <lsb_timer.h>
#include <lsb_mq.h>

#define CHILD_PROCESS_NUM sizeof(pr_desc) / sizeof(builtin_process_desc)
#define MAX_MQ_NUM 10

typedef struct {
    pid_t (*create_process_func)();
    char *name;
    bool is_exec_pr;
} builtin_process_desc;

builtin_process_desc pr_desc[] = {
    { &create_input, "INPUT", false },
    { &create_system_server, "SYSTEM_SERVER", false },
    { &create_ui, "UI", true },
    { &create_web_server, "WEB_SERVER", true }
};

static int sig_handled_cnt = 0;
static int process_num = 0;
static pid_t child_pids[CHILD_PROCESS_NUM];
static pthread_t watchdog_thread, killer_thread;
static mqd_t watchdog_mq;

void create_processes();
void create_mqueues();
void *watchdog_thread_fn(void *arg);
void *killer_thread_fn(void *arg);
void wake_up_killer_thread(int sig);
void block_until_children_die();

int main(int argc, char *argv[])
{
    int watchdog_tid, killer_tid;
    void *status;
    struct sigaction sa;

    printf("[MAIN]\t pid=%d process created!\n\n", getpid());

    create_mqueues();
    create_processes();

    sigemptyset(&sa.sa_mask);
    sa.sa_flags = 0;
    sa.sa_handler = wake_up_killer_thread;
    if (sigaction(SIGCHLD, &sa, NULL) == -1) {
        err_exit("sigaction() in main");
    }
    /* create threads */
    killer_tid = pthread_create(&killer_thread, NULL, killer_thread_fn, "KILLER");
    assert(killer_tid==0);
    watchdog_tid = pthread_create(&watchdog_thread, NULL, watchdog_thread_fn, "WATCHDOG");
    assert(watchdog_tid==0);

    block_until_children_die();

    pthread_join(killer_thread, &status);

    destroy_all_mq();

    printf("\n[MAIN]\t %d child processes left...\n", process_num);
    printf("[MAIN]\t lsbsystem exit...\n");
    return 0;
}

void create_processes() 
{
    int i;

    for (i = 0; i < CHILD_PROCESS_NUM; i++) {
        child_pids[i] = pr_desc[i].create_process_func();
        process_num++;
    }
    printf("[MAIN]\t Total %d process created!\n", process_num);
}

void create_mqueues()
{
    watchdog_mq = create_mq("/watchdog", MAX_MQ_NUM, sizeof(watchdog_msg_t));
}

/*  Watchdog thread function
    This thread send signal to other processes, then check if processes are alive
    For UI and Web server processes, just check existence of the process
    For system server and input processes, send SIGUSR2 
    and wait for receiving message in mq from these processes with timeout
    If one of children is terminated, wake up killer thread
    Otherwise, repeat upper routine after 10 seconds
*/
void *watchdog_thread_fn(void *arg)
{
    char *thread_name = (char *)arg;
    unsigned int sleep_second = 10;
    int i, ret_code = 0;
    watchdog_msg_t msg[CHILD_PROCESS_NUM];
    struct timespec timeout;

    memset(msg, 0, sizeof(msg[0]) * CHILD_PROCESS_NUM);
    timeout.tv_sec = 0.5;
    timeout.tv_nsec = 0; 

    printf("[%s]\t thread created! Watchdog starts in %d seconds...\n", thread_name, sleep_second);

watchdog_kick:
    lsb_sleep(sleep_second, 0);

    for (i = 0; i < CHILD_PROCESS_NUM; i++) {
        if (pr_desc[i].is_exec_pr) { 
            /* Just check existence of the process */
            ret_code = kill(child_pids[i], 0);
            if (ret_code == -1) goto watchdog_reset;
            msg[i].pid = child_pids[i];
        }
        else {
            /* Send signal and receive messages */
            ret_code = kill(child_pids[i], SIGUSR2);
            if (ret_code == -1) goto watchdog_reset;
            ret_code = mq_timedreceive(watchdog_mq, (char *)&msg[i], sizeof(msg[0]), NULL, &timeout);
            if (ret_code == -1) goto watchdog_reset;   
        }
    }
    printf("\n\n<WATCHDOG MESSAGE>\n");
    for (i = 0; i < CHILD_PROCESS_NUM; i++) {
        
    }
    goto watchdog_kick;

watchdog_reset:
    fprintf(stderr, "[WATCHDOG]\t %s process: %s", pr_desc[i].name, strerror(errno));
    wake_up_killer_thread(SIGUSR2);

    return NULL;
}

/*  Killer thread function
    This thread is blocked until one of child processes terminated
    or watchdog thread detected some defect in whole system
    If this thread wake up, kill all the child processes
*/
void *killer_thread_fn(void *arg)
{
    int sig, i, ret;
    sigset_t set;
    char *thread_name = (char *)arg;

    printf("[%s]\t thread created and blocking...\n", thread_name);

    sigemptyset(&set);
    sigaddset(&set, SIGUSR1);
    sigprocmask(SIG_BLOCK, &set, NULL);
    
    ret = sigwait(&set, &sig); // block until SIGUSR1 signal is pending
    assert(ret==0);

    printf("[%s]\t thread wakes up!\n", thread_name);
    /* kill all child processes */
    for (i = 0; i < CHILD_PROCESS_NUM; i++) {
        ret = kill(child_pids[i], 0);
        if (ret != 0) {
            if (errno == ESRCH) 
                printf("[%s]\t No process with pid = %d\n", thread_name, child_pids[i]);
            continue;
        }
        kill(child_pids[i], SIGKILL); // force child to terminate
        printf("[%s]\t thread sends sigkill to pid=%d\n", thread_name, child_pids[i]);
        lsb_sleep(1, 0);
    }

    return NULL;
}

void wake_up_killer_thread(int sig)
{
    char *signame;

    signame = strsignal(sig);
    if (signame != NULL)
        printf("[MAIN]\t handler called: %s\n", strsignal(sig));
    
    /* wake up killer thread only once */
    if (sig_handled_cnt > 0)
        return;

    pthread_kill(killer_thread, SIGUSR1);
    sig_handled_cnt++;
}

/* Reap all child processes */
void block_until_children_die() 
{
    pid_t pid;
    int wstatus;

    while (process_num) {
        pid = waitpid(-1, &wstatus, 0);
        if (WIFEXITED(wstatus)) {
            printf("%d...\t PID=%d EXITED with EXIT STATUS: %d\n", 
                process_num, pid, WEXITSTATUS(wstatus));
            process_num--;
        }
        else if (WIFSIGNALED(wstatus)) {
            printf("%d...\t PID=%d EXITED with SIGNAL NUMBER: %d\n", 
                process_num, pid, WTERMSIG(wstatus));
            process_num--;
        }   
    }
}