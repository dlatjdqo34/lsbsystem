#include <stdio.h>
#include <errno.h>
#include <stdlib.h>

#include <utils.h>

void err_exit(char* msg)
{
    perror(msg);
    exit(EXIT_FAILURE);
}