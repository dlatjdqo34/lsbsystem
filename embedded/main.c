#include <stdio.h>
#include <sys/types.h>
#include <sys/wait.h>

#include <system_server.h>
#include <ui.h>
#include <input.h>

#define NUM_PROCESS 5

int main(int argc, char* argv[])
{
    pid_t pid[NUM_PROCESS];
    int wstatus;

    printf("[Main]\t Main Entry\n");

    /* Create Processes */
    create_system_server();
    create_ui();
    create_input();

    for (int i=0; i<NUM_PROCESS; i++) {
        waitpid(pid[i], &wstatus, 0);
    }

    return 0;
}