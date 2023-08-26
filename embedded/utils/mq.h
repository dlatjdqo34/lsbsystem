/* 
 *  This file is a module for functions and structures 
 *  for manage POSIX message queue. 
 *  We need -lrt link option while compiling.
 */

#ifndef _MQ_H
#define _MQ_H

#include <fcntl.h>           /* For O_* constants */
#include <sys/stat.h>        /* For mode constants */
#include <mqueue.h>

typedef struct {
    int op;
} lsb_msg;


typedef struct {
    char name[16];
    mqd_t mqd;
} lsb_mq;




void lsb_mq_init(long num, long size);
mqd_t lsb_mq_create(char *name);

void lsb_mq_destroy(char *name);
void lsb_mq_destroy_all();

#endif