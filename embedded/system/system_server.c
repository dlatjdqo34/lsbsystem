#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>

#include <utils.h>
#include <system_server.h>

int system_server_process()
{
    return 0;
}

void create_system_server() 
{
    printf("\t Create system server process...\n");
    switch (fork())
    {
    case -1:
        err_exit("[SYSTEM_SERVER]\t fork error...");
        break;
    case 0: /* Child Process */
        system_server_process();
        exit(EXIT_SUCCESS);
        break;
    default:
        break;
    }
}