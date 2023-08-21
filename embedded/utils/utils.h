#ifndef _UTILS_H
#define _UTILS_H

#define err_log(fmt, ...) \
    printf("[%s: %d][%s]\t " fmt "\t\t\t (%s, %s)\n", \
    __FILE__, __LINE__, __func__, __DATE__, __TIME__);

void err_exit(char* msg);


#endif