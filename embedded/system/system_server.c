#define  _GNU_SOURCE
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/prctl.h>
#include <pthread.h>
#include <assert.h>
#include <string.h>

#include <utils.h>
#include <system_server.h>


void *disk_thread(void *arg)
{
    return NULL;
}

int system_server_process()
{
    const char *name = "SYSTEM_SERVER";
    int tid;
    void *status;
    pthread_t shell_tid;

    if (prctl(PR_SET_NAME, (unsigned long) name) < 0)
        perror("[%s]\t Process nanme unchanged... Keep going\n");
    printf("\n[%s]\t %s process created!\n\n", name, name);
    
    /* Create shell thread for built-in command */
    tid = pthread_create(&shell_tid, NULL, disk_thread, "SBSH");
    assert(tid==0);

    pthread_join(shell_tid, &status);

    return 0;
}

pid_t create_system_server() 
{
    pid_t pid;

    printf("...\t Create system server process...\n");
    switch ((pid = fork()))
    {
    case -1:
        err_exit("SYSTEM_SERVER fork error...");
        break;
    case 0: /* Child Process */
        system_server_process();
        _exit(EXIT_SUCCESS);
        break;
    default:
        break;
    }

    return pid;
}