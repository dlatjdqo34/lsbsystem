#include <stdio.h>
#include <time.h>

/* POSIX high resolution sleep */
int lsb_sleep(unsigned int sec, long nsec)
{
    struct timespec sleep_time;
    sleep_time.tv_sec = sec;
    sleep_time.tv_nsec = nsec;

    return nanosleep(&sleep_time, NULL);
}