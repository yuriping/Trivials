#ifndef _MAIN_H_
#define _MAIN_H_

#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

typedef struct THR {
    pthread_t     tid;
    volatile unsigned char run;
} THREAD, P_THREAD;

extern THREAD thr0;

#endif /* _MAIN_H_ */


