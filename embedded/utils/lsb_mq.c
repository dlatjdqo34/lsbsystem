#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#include <lsb_mq.h>
#include <utils.h>

#define MAX_MQ_LIST_NUM 10

typedef struct {
    char name[MAX_NAME_LEN];
    mqd_t mqd;
} lsb_mq_t;

static lsb_mq_t mq_list[MAX_MQ_LIST_NUM];
static int cur_mq_index = 0;

mqd_t create_mq(const char *name, long num, long size)
{
    mqd_t mqd;

    struct mq_attr mq_attr;
    memset(&mq_attr, 0, sizeof(mq_attr));
    mq_attr.mq_maxmsg = num;
    mq_attr.mq_msgsize = size; 

    if (cur_mq_index == MAX_MQ_LIST_NUM) {
        err_exit("mq_list is full in create_mq");
    }

    if (strlen(name) >= MAX_NAME_LEN) {
        fprintf(stderr, "NAME TOO LONG!! Name for mqueue should be less than %d", MAX_NAME_LEN);
        err_exit("strlen() in create_mq");
    }

    mq_unlink(name);
    mqd = mq_open(name, O_CREAT | O_RDWR | O_EXCL | O_CLOEXEC, 0666, &mq_attr);
    if (mqd == -1) {
        if (errno == EEXIST) { // mqueue name "name" already exist, try open again
            mqd = mq_open(name, O_RDWR);
            assert(mqd != -1);
        }
        else {
            fprintf(stderr, "%s", strerror(errno));
            err_exit("mq_open() in create_mq");
        }
    }

    /* Insert created mqueue in mq_list */
    strncpy(mq_list[cur_mq_index].name, name, strlen(name));
    mq_list[cur_mq_index].mqd = mqd;
    cur_mq_index++;

    return mqd;
}

int close_mq(mqd_t mqd)
{
    int ret;

    ret = mq_close(mqd);
    if (ret == -1) {
        fprintf(stderr, "mq_close() in close_mq(%d): %s\n", mqd, strerror(errno));
    }

    return ret;
}

void destroy_mq(char *name)
{
    int ret = mq_unlink(name);
    if (ret) {
        fprintf(stderr, "mq_unlink() in destroy_mq(%s): %s\n", name, strerror(errno));
    }
}

void destroy_all_mq()
{
    int i;

    for (i = 0; i < cur_mq_index; i++) {
        destroy_mq(mq_list[i].name);
    }
}


