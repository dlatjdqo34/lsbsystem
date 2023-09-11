#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/prctl.h>
#include <pthread.h>
#include <assert.h>
#include <string.h>
#include <unistd.h>

#include <utils.h>
#include <web_server.h>
#include <lsb_timer.h>

int web_server_process()
{
    const char *name = "WEB_SERVER";

    if (prctl(PR_SET_NAME, (unsigned long) name) < 0)
        perror("[%s]\t Process nanme unchanged... Keep going\n");
    printf("\n[%s]\t pid=%d process created!\n\n", name, getpid());

    while(1) {
        lsb_sleep(10, 0);
    }

    return 0;
}

pid_t create_web_server() 
{
    pid_t pid;

    printf("...\t Create web server process...\n");
    switch ((pid = fork()))
    {
    case -1:
        err_exit("WEB SERVER fork error...");
        break;
    case 0: /* Child Process */
        web_server_process();
        _exit(EXIT_SUCCESS);
        break;
    default:
        break;
    }

    return pid;
}