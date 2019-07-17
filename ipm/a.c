#include <fcntl.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <unistd.h>

#define SHM_MODE        600
#define SHM_MAT_FLAG      0

#define BUF_LEN         (10)

int main(void)
{
    key_t key = ftok("./rsrc", 1);
printf("%d\n", key);
    if (key == -1)
    {
        fprintf(stderr, "ftok Failed...\n");    exit(-1);
    }

    int shmidMutexRsrc;
    if ((shmidMutexRsrc = shmget(key, sizeof(pthread_mutex_t), SHM_MODE)) == -1)
    {
        fprintf(stderr, "shmget Failed\n"); exit(-2);
    }
printf("shmget = %p\n", shmidMutexRsrc);

    pthread_mutex_t * pMutexAccess;
    if ((pMutexAccess = shmat(shmidMutexRsrc, NULL, SHM_MAT_FLAG)) == (void *) -1)
    {
        fprintf(stderr, "shmat Failed\n");  exit(-3);
    }

    for (;;)
    {
        pthread_mutex_lock(pMutexAccess);

        int  fd;
        char buf[BUF_LEN];
        int  len;
        int  val;

        /* 读出文件内的当前值 */
        if ((fd = open("./cnt", O_RDONLY)) == -1)
        {
            printf("file open to read failed...\n");    exit(-1);
        }
        memset(buf, 0x00, BUF_LEN);
        len = read(fd, buf, BUF_LEN);
        sscanf(buf, "%d", &val);
    printf("[read] val = %d\n", val);
        close(fd);

        /* 写入新值 */
        if ((fd = open("./cnt", O_WRONLY | O_TRUNC)) == -1)
        {
            printf("file open to write failed...\n");    exit(-1);
        }
        memset(buf, 0x00, BUF_LEN);
        val ++;
        len = snprintf(buf, BUF_LEN, "%d", val);
        write(fd, buf, len);
        close(fd);

        pthread_mutex_unlock(pMutexAccess);

        usleep(100000);
    }

    exit(0);
}
