#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>

#include <utils.h>
#include <input.h>

int input_process()
{
    return 0;
}

void create_input() 
{
    printf("\t Create input process...\n");
    switch (fork())
    {
    case -1:
        err_exit("[INPUT]\t fork error...");
        break;
    case 0: /* Child Process */
        input_process();
        exit(EXIT_SUCCESS);
        break;
    default:
        break;
    }
}