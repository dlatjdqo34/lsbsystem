#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <pthread.h>
#include <sys/prctl.h>

#include <utils.h>
#include <ui.h>
#include <lsb_timer.h>

int ui_process()
{
    const char *name = "UI";

    if (prctl(PR_SET_NAME, (unsigned long) name) < 0)
        perror("[%s]\t Process nanme unchanged... Keep going\n");
    printf("\n[%s]\t pid=%d process created!\n\n", name, getpid());

    while(1) {
        lsb_sleep(10, 0);
    }

    return 0;
}

pid_t create_ui() 
{
    pid_t pid;

    printf("...\t Create ui process...\n");
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