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

#include <system_server.h>
#include <ui.h>
#include <input.h>
#include <web_server.h>
#include <utils.h>
#include <lsb_timer.h>
#include <lsb_mq.h>

#define CHILD_PROCESS_NUM sizeof(builtin_create_process_func) / sizeof(void *)
#define MAX_MQ_NUM 10

pid_t (*builtin_create_process_func[])() = {
    &create_input,
    &create_system_server,
    &create_ui,
    &create_web_server
};

static int sig_handled_cnt = 0;
static int process_num = 0;
static pid_t child_pids[CHILD_PROCESS_NUM];
static pthread_t watchdog_thread, killer_thread;
static mqd_t watchdog_mq;

void create_processes() 
{
    int i;

    for (i = 0; i < CHILD_PROCESS_NUM; i++) {
        child_pids[i] = builtin_create_process_func[i]();
        process_num++;
    }
    printf("[MAIN]\t Total %d process created!\n", process_num);
}

void create_mqueues()
{
    watchdog_mq = create_mq("/watchdog", MAX_MQ_NUM, sizeof(watchdog_msg_t));
}

void *watchdog_thread_fn(void *arg)
{
    char *thread_name = (char *)arg;
    unsigned int sleep_second = 10;

    printf("[%s]\t thread created! Watchdog starts in %d seconds...\n", 
            thread_name, sleep_second);

    while(1) {
        lsb_sleep(sleep_second, 0);
    }

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

    printf("[%s]\t thread created and blocked...\n", thread_name);

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

int main(int argc, char *argv[])
{
    int watchdog_tid, killer_tid;
    void *status;

    printf("[MAIN]\t pid=%d process created!\n\n", getpid());
    signal(SIGCHLD, wake_up_killer_thread);
    create_mqueues();
    create_processes();

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