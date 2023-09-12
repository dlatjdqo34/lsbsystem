/* 
    This file is a module for functions and structures 
    for manage POSIX message queue. 
    We need -lrt link option while compiling.
 */

#ifndef _LSB_MQ_H
#define _LSB_MQ_H

#include <fcntl.h>           /* For O_* constants */
#include <sys/stat.h>        /* For mode constants */
#include <mqueue.h>

#define MAX_NAME_LEN 32

typedef struct {
    int op;
} lsb_msg_t;

typedef struct {
    int op;
} watchdog_msg_t;

mqd_t create_mq(const char *name, long num, long size);
int close_mq(mqd_t mqd);
void destroy_mq(char *name);
void destroy_all_mq();

#endif