#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>

#include <utils.h>
#include <web_server.h>

int web_server_process()
{
    return 0;
}

int create_web_server() 
{
    printf("\t Create web server process...\n");
    switch (fork())
    {
    case -1:
        err_exit("[WEB SERVER]\t fork error...");
        break;
    case 0: /* Child Process */
        web_server_process();
        exit(EXIT_SUCCESS);
        break;
    default:
        break;
    }

    return 1;
}