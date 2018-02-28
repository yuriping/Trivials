
#include <stdio.h>
#include <malloc.h>
#include <stdlib.h>
#include <pthread.h>
#include "osa_pool.h"

void OSA_PoolInit(OSA_HPOOL hpool)
{
	hpool->buf = (List) malloc(sizeof(Node));
	hpool->buf->next = NULL;

	pthread_mutexattr_t mutex_attr;
	pthread_condattr_t  cond_attr;
	pthread_mutexattr_init(&mutex_attr);
	pthread_condattr_init(&cond_attr);
	pthread_mutex_init(&hpool->lock, &mutex_attr);
	pthread_cond_init(&hpool->condRd, &cond_attr);
	pthread_condattr_destroy(&cond_attr);
	pthread_mutexattr_destroy(&mutex_attr);
}

List OSA_PoolPut(OSA_HPOOL hpool, Node * pnode)
{
	pthread_mutex_lock(&hpool->lock);

	pnode->next = hpool->buf->next;
	hpool->buf->next = pnode;

	pthread_cond_signal(&hpool->condRd);

	pthread_mutex_unlock(&hpool->lock);

	return hpool->buf;
}

List OSA_PoolGet(OSA_HPOOL hpool, unsigned char ch)
{
	pthread_mutex_lock(&hpool->lock);
	List list = (List) malloc(sizeof(Node));
	list->channelNum   = 0;
        list->bufAddr = 0;
	list->next = NULL;

	List s = NULL;
	List p = NULL;

	while(1)
	{
		if (hpool->buf->next)
		{
			for (p=hpool->buf->next, s=hpool->buf; p; )
			{
				if (ch == p->channelNum)
				{
					s->next = p->next;
					p->next = list->next;
					list->next = p;
					if (s)
						p=s->next;
					else
						p = NULL;
				}
				else
				{
					s=p;
					p=p->next;
				}
			}
			break;
		}
		else
		{
			pthread_cond_wait(&hpool->condRd, &hpool->lock);
		}
	}
	pthread_mutex_unlock(&hpool->lock);

	return list;
}

void OSA_PoolPrint(OSA_HPOOL hpool)
{
        pthread_mutex_lock(&hpool->lock);

        printf("\n   PUT {");
	List p = NULL;
	for (p=hpool->buf->next; p; p=p->next)
	{
		printf("(%2d-%ld),", p->channelNum, p->bufAddr);
	}
	printf(" }\n");

        pthread_mutex_unlock(&hpool->lock);
}

void OSA_PoolDeinit(OSA_HPOOL hpool)
{
        Node * p = hpool->buf;
        while (p)
        {
            Node * q = p;
            p = p->next;
            free(q);
        }
        hpool->buf = NULL;

	pthread_cond_destroy(&hpool->condRd);
	pthread_mutex_destroy(&hpool->lock);
}


