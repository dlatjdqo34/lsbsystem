#include <stdio.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <signal.h>
#include <assert.h>
#include <errno.h>

#include <system_server.h>
#include <ui.h>
#include <input.h>
#include <web_server.h>

// #define NUM_PROCESS 5
static int process_num = 0;

void block_until_child_die() 
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

    /* Create Processes */
    printf("\t Create input process...\n");
    process_num += create_input();
    printf("\t Create system server process...\n");
    process_num += create_system_server();
    printf("\t Create ui process...\n");
    process_num += create_ui();
    printf("\t Create web server process...\n");
    process_num += create_web_server();

    printf("[MAIN]\t Total %d process created!\n", process_num);

    block_until_child_die();

    printf("\n[Main]\t %d child processes left...\n", process_num);
    printf("[MAIN]\t lsbsystem exit...\n");
    return 0;
}