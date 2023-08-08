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

void create_ui() 
{
    printf("\t Create ui process...\n");
    switch (fork())
    {
    case -1:
        err_exit("[UI]\t fork error...");
        break;
    case 0: /* Child Process */
        ui_process();
        exit(EXIT_SUCCESS);
        break;
    default:
        break;
    }
}