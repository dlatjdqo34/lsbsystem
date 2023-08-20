#define _GNU_SOURCE
#include <stdio.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <signal.h>
#include <assert.h>
#include <errno.h>
#include <string.h>

#include <system_server.h>
#include <ui.h>
#include <input.h>
#include <web_server.h>

#define CHILD_PROCESS_NUM sizeof(builtin_create_process_func) / sizeof(void *)

pid_t (*builtin_create_process_func[])() = {
    &create_input,
    &create_system_server,
    &create_ui,
    &create_web_server
};

static int sigchld_handled_cnt = 0;
static int process_num = 0;
static pid_t child_pids[CHILD_PROCESS_NUM];

void make_processes() 
{
    int i;

    for (i = 0; i < CHILD_PROCESS_NUM; i++) {
        child_pids[i] = builtin_create_process_func[i]();
        process_num++;
    }
    printf("[MAIN]\t Total %d process created!\n", process_num);
}


void kill_all_processes_sighandler(int signum)
{
    printf("[SIGCHLD]\t handler called!\n");
    if (sigchld_handled_cnt > 0)
        return;
    
    sigchld_handled_cnt++;
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
    printf("[MAIN]\t MAIN Entry\n\n");

    signal(SIGCHLD, kill_all_processes_sighandler);

    make_processes();
    block_until_children_die();

    printf("\n[MAIN]\t %d child processes left...\n", process_num);
    printf("[MAIN]\t lsbsystem exit...\n");
    return 0;
}