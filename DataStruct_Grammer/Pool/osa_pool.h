
#include <pthread.h>

#ifndef _OSA_POOL_H_
#define _OSA_POOL_H_

//definition of Pool
typedef struct Node{
	unsigned char channelNum;
	unsigned long bufAddr;
	struct Node  *next;
} Node, * List;

typedef struct OSA_POOL {
	List buf;

	pthread_mutex_t lock;
	pthread_cond_t  condRd;
} OSA_POOL, * OSA_HPOOL;

//extern methods of Pool
void OSA_PoolInit(OSA_HPOOL hpool);
List OSA_PoolPut(OSA_HPOOL hpool, Node * pnode);
List OSA_PoolGet(OSA_HPOOL hpool, unsigned char ch);
void OSA_PoolPrint(OSA_HPOOL hpool);
void OSA_PoolDeinit(OSA_HPOOL hpool);

#endif //_OSA_POOL_H_

