#include <stdio.h>
#include <sys/types.h>
#include <sys/wait.h>

#include <system_server.h>
#include <ui.h>
#include <input.h>
#include <web_server.h>

// #define NUM_PROCESS 5

int main(int argc, char* argv[])
{
    pid_t pid;
    int wstatus, num_process = 0;

    printf("[Main]\t Main Entry\n");

    /* Create Processes */
    num_process += create_input();
    num_process += create_system_server();
    num_process += create_ui();
    num_process += create_web_server();

    while (num_process) {
        pid = waitpid(-1, &wstatus, 0);
        if (WIFEXITED(wstatus)) {
            printf("\t PID=%d EXITED with EXIT STATUS: %d\n", pid, WEXITSTATUS(wstatus));
            num_process--;
        }
        else if (WIFSIGNALED(wstatus)) {
            printf("\t PID=%d EXITED with SIGNAL NUMBER: %d\n", pid, WTERMSIG(wstatus));
            num_process--;
        }
    }

    return 0;
}