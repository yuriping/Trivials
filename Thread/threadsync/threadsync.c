#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

typedef struct
{
  unsigned int len;
  unsigned int count;
  unsigned int curRd;
  unsigned int curWr;

  unsigned int *queue;

  pthread_mutex_t lock;
  pthread_cond_t  condRd;
  pthread_cond_t  condWr;
} QueHndl;

void queCreate(QueHndl *hndl, unsigned int maxlen)
{
  pthread_mutexattr_t mutex_attr;
  pthread_condattr_t cond_attr;

  hndl->curRd = hndl->curWr = 0;
  hndl->count = 0;
  hndl->len   = maxlen;
  hndl->queue = malloc(sizeof(unsigned int) * hndl->len);

  pthread_mutexattr_init(&mutex_attr);
  pthread_condattr_init(&cond_attr);

  pthread_mutex_init(&hndl->lock, &mutex_attr);
  pthread_cond_init(&hndl->condRd, &cond_attr);
  pthread_cond_init(&hndl->condWr, &cond_attr);

  pthread_condattr_destroy(&cond_attr);
  pthread_mutexattr_destroy(&mutex_attr);
}

void queDelete(QueHndl *hndl)
{
  if(hndl->queue!=NULL)
    free(hndl->queue);

  pthread_cond_destroy(&hndl->condRd);
  pthread_cond_destroy(&hndl->condWr);
  pthread_mutex_destroy(&hndl->lock);
}

void quePut(QueHndl *hndl, unsigned int value)
{
  pthread_mutex_lock(&hndl->lock);

  while(1)
  {
    if( hndl->count < hndl->len )
    {
      hndl->queue[hndl->curWr] = value;
      hndl->curWr = (hndl->curWr + 1) % hndl->len;
      hndl->count ++;

      pthread_cond_signal(&hndl->condRd);
      break;
    }
    else
    {
      pthread_cond_wait(&hndl->condWr, &hndl->lock);
    }
  }

  pthread_mutex_unlock(&hndl->lock);
}

void queGet(QueHndl *hndl, unsigned int *value)
{
  pthread_mutex_lock(&hndl->lock);

  while(1)
  {
    if(hndl->count > 0 )
    {
      *value = hndl->queue[hndl->curRd];
      hndl->curRd = (hndl->curRd + 1) % hndl->len;
      hndl->count --;

      pthread_cond_signal(&hndl->condWr);
      break;
    }
    else
    {
      pthread_cond_wait(&hndl->condRd, &hndl->lock);
    }
  }

  pthread_mutex_unlock(&hndl->lock);
}

QueHndl que;

pthread_t tid_writeque;
pthread_t tid_readque;

//////////////////////////////////////////////////////////////////////////////////////////////////////////////

void * writequeue(void * arg)
{
    static int num = 0;

    while(1)
    {
        quePut(&que, num);
        num ++;
        sleep(2);
    }

    return ((void *)0);
}

void * readqueue(void * arg)
{
    unsigned int value;

    while (1)
    {
        queGet(&que, &value);
        printf("current element is : %d\n", value);
        sleep(1);
    }

    return ((void *)0);
}

int main(void)
{
    queCreate(&que, 10);

    pthread_create(&tid_writeque, NULL, writequeue, NULL);

    sleep(20);

    pthread_create(&tid_readque, NULL, readqueue, NULL);

    while(1);

    return 0;
}

