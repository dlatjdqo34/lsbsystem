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

int create_ui() 
{
    switch (fork())
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

    return 1;
}