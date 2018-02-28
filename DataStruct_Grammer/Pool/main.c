
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include "osa_pool.h"

#define	MAX_GET_THR_NUM		(8)
#define	ARR_LEN			(32)

OSA_POOL pool;

unsigned long data[MAX_GET_THR_NUM] = {0, 0, 0, 0, 0, 0, 0, 0};

void * PutNode(void *arg)
{
	unsigned char index = 0;
	unsigned char ids[ARR_LEN] = {1,0,7,0,2,1,0,0,4,2,0,6,0,1,2,0,1,0,0,3,2,1,6,5,4,2,0,6,0,1,2,0};

	while(1)
	{
                usleep(5000);

		Node * pnode = (Node *) malloc(sizeof(Node));
		pnode->channelNum = ids[index];
		switch (pnode->channelNum)
		{
		case 0:
			pnode->bufAddr = data[0]++;
			break;
		case 1:
			pnode->bufAddr = data[1]++;
			break;
		case 2:
			pnode->bufAddr = data[2]++;
			break;
		case 3:
			pnode->bufAddr = data[3]++;
			break;
		case 4:
			pnode->bufAddr = data[4]++;
			break;
		case 5:
			pnode->bufAddr = data[5]++;
			break;
		case 6:
			pnode->bufAddr = data[6]++;
			break;
		case 7:
			pnode->bufAddr = data[7]++;
			break;
		default:
			break;
		}

		index = (index + 1) % ARR_LEN;
		pnode->next = NULL;

		OSA_PoolPut(&pool, pnode);
		OSA_PoolPrint(&pool);
	}

	return NULL;
}

void * GetNode(void *arg)
{
	unsigned char id = *((unsigned char *)arg);

	while (1)
	{
                usleep(40000);

                printf("GET-%d--------", id);
		List list = OSA_PoolGet(&pool, id);

		List p = NULL;
		for (p=list->next; p; p=p->next)
		{
			printf("[%d:%1d-%ld], ", id, p->channelNum, p->bufAddr);
		}
		printf("\n");

		p = list;
		while (p)
		{
			Node * q = p;
        		p = p->next;
        		printf("free node id:%d, data : %ld\n", q->channelNum, q->bufAddr);
        		free(q);
		}
	}

	return NULL;
}

int main(void)
{
	pthread_t                 putThr;
	pthread_t                 getThr[MAX_GET_THR_NUM];

	OSA_PoolInit(&pool);

	pthread_create(&putThr, NULL, PutNode ,NULL);

	unsigned char id[MAX_GET_THR_NUM] = {0,1,2,3,4,5,6,7};
	unsigned int i = 0;

	for (i=0; i<MAX_GET_THR_NUM; i++)
	{
		pthread_create(&getThr[i], NULL, GetNode,(void *)&id[i]);
	}

        pthread_join(putThr,NULL);
        for(i = 0;i < MAX_GET_THR_NUM;i++)
        {
    	    pthread_join(getThr[i],NULL);
        }

    return 0;
}

