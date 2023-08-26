#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <string.h>

#include <mq.h>
#include <utils.h>

#define MQ_LIST_NUM 10

static struct mq_attr lsb_mq_attr;
static lsb_mq lsb_all_mq_list[MQ_LIST_NUM];

void lsb_mq_attr_init(long num, long size)
{
    lsb_mq_attr.mq_maxmsg = num; 
    lsb_mq_attr.mq_msgsize = size; 
    lsb_mq_attr.mq_curmsgs = 0;
}

mqd_t lsb_mq_create(char *name)
{
    mqd_t mqd;

    mqd = mq_open(name, O_CREAT | O_RDWR | O_EXCL,  0666, &lsb_mq_attr);
    if (mqd == -1) {
        err_exit("mq_open() in lsb_mq_create");
    }

    return mqd;
}

/* Send message to mq with */
int lsb_mq_send(mqd_t mqd, void *msg_ptr)
{
    lsb_msg *msg = (lsb_msg *)msg_ptr;
    int ret;

    ret = mq_send(mqd, (char *)msg, sizeof(lsb_msg), 0);
    if (ret == -1) {
        fprintf(stderr, "[%s]\t mq_send() in lsb_mq_send", strerror(errno));
    }

    return ret;
}

int lsb_mq_recv(mqd_t mqd, void *msg_ptr)
{
    lsb_msg *msg = (lsb_msg *)msg_ptr;
    int ret;

    ret = mq_receive(mqd, (char *)msg, sizeof(lsb_msg), 0);
    if (ret == -1) {
        fprintf(stderr, "[%s]\t mq_receive() in lsb_mq_recv", strerror(errno));
    }
}


void lsb_mq_destroy(char *name)
{
    int ret = mq_unlink(name);
    if (ret) {
        if (errno == EACCES) 
            err_exit("The caller does not have permission to unlink this message queue.");
        else if (errno == ENAMETOOLONG)
            err_exit("name was too long");
        else 
            err_exit("There is no message queue with the given name");
    }
}

void lsb_mq_destroy_all()
{
    int i;

    for (i = 0; i < MQ_LIST_NUM; i++) {
        lsb_mq_destroy(lsb_all_mq_list[i].name);
    }
}


