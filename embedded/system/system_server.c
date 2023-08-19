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

int create_system_server() 
{
    switch (fork())
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

    return 1;
}