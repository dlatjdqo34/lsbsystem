#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>

#include <utils.h>
#include <ui.h>

int ui_process()
{
    return 0;
}

pid_t create_ui() 
{
    pid_t pid;

    printf("\t Create ui process...\n");
    switch ((pid = fork()))
    {
    case -1:
        err_exit("UI fork error...");
        break;
    case 0: /* Child Process */
        ui_process();
        _exit(EXIT_SUCCESS);
        break;
    default:
        break;
    }

    return pid;
}