/*
 * 2018-09-11
 *
 * 现在遇到的问题：
 * 0. 在属性中写入调度方式与优先级，创建线程的方式，不起作用，线程的调度方式与优先级依然是 OTHER 与 0 级；
 * 1. 但在线程创建后，动态改变调度方式和优先级是可以的；
 * 2. 两个线程，如果都是 0 级，则可以交替运行；
 *    如果不是 0 级，即使两个同级，都不能交替运行，先创建的将始终占用，另一个得不到执行；
 *    即使先创建的休眠一段时间，一段时间以后，先创建的仍然会一直运行；
 *
 * 规律：
 * 0. 以属性创建线程时赋予策略、优先级，不起作用，还是 OTHER 策略，优先级 0 （无用）；
 *    只有在线程执行后，运行时改变才行，而且不能创建后立即改变，要延时一会儿；
 *    TODO ：或者等一个线程已运行的信号量，未测试；
 * 1. 优先级：OTHER（0），RR（1-99）、FIFO（1-99），0 无效；1-99，值越大优先级越高；
 * 2. 只要指定为 SCHED_OTHER，就会交替运行，而且间隔很短；
 * 3. 当指定策略为 FIFO ，同优先级线程，先运行的会一直运行，不会释放 CPU，另一个无法运行；
 * 4. 当策略为 RR ，同优先级线程，会交替运行，不过这个时间片较长，比 OTHER 时，几秒吧，
 *    这可能是当前只有 2 个线程的缘故，可能是总时间段是固定的，由多个线程平均分配；
 *    但也发现忽长忽短的时候；
 * 5. 当策略为 FIFO 或 RR 时，高优先级的线程是会抢占低优先级的；
 *    多个同优先级的线程，第一次运行者，为先创建的线程；
 * 6. 如果多种调度方式同时存在，OTHER 得不到执行，FIFO、RR 谁执行看优先级，优先级高的一直执行，
 *    同优先级的，FIFO 属性的一直执行；
 *    当然，如果各个线程中有类似sleep操作自动释放CPU占用，则不管哪种调度，各个线程均能得到执行；
 * 7. 修改优先级，不是必须在被修改线程外，线程自身也可以自行修改，
 *    只要使用 pthread_setschedparam(pthread_self(), policyset, &paramset);
 */


#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>


void * thr0(void * pPrm)
{
    sleep(1);
    int cnt = 0;

    for (;;)
    {
        printf("[thr0] cnt = %d\n", cnt ++);
//        sleep(1);
    }

    pthread_exit((void *) 0);
}

void * thr1(void * pPrm)
{
    sleep(1);
    int cnt = 0;

    for (;;)
    {
        printf("[thr1] cnt = %d\n", cnt ++);
//        sleep(1);
    }

    pthread_exit((void *) 0);
}

int main(void)
{
    printf("SCHED_OTHER = %d, SCHED_RR = %d, SCHED_FIFO = %d\n", SCHED_OTHER, SCHED_RR, SCHED_FIFO);

    int min = sched_get_priority_min(SCHED_OTHER);
    int max = sched_get_priority_max(SCHED_OTHER);
    printf("[OTHER] min = %d, max = %d\n", min, max);
    min = sched_get_priority_min(SCHED_RR);
    max = sched_get_priority_max(SCHED_RR);
    printf("[RR] min = %d, max = %d\n", min, max);
    min = sched_get_priority_min(SCHED_FIFO);
    max = sched_get_priority_max(SCHED_FIFO);
    printf("[FIFO] min = %d, max = %d\n", min, max);

    printf("================================\n");

    pthread_attr_t attrset;
    int policyset, policyget;
    struct sched_param paramset, paramget;
    pthread_t tid0, tid1;

    /* Thread 0 */
    // 配置创建属性
    pthread_attr_init(&attrset);
    pthread_attr_setschedpolicy(&attrset, SCHED_RR);
    paramset.sched_priority = 4;
    pthread_attr_setschedparam(&attrset, &paramset);//*/
    // 创建线程
    pthread_create(&tid0, &attrset, thr0, NULL);
    // 看优先级
    pthread_getschedparam(tid0, &policyget, &paramget);
    printf("[thr 0] : policy = %d, priority = %d\n", policyget, paramget.sched_priority);
    // 运行时设置优先级，并查看
    policyset = SCHED_RR; // SCHED_FIFO;
    paramset.sched_priority = 15;
    pthread_setschedparam(tid0, policyset, &paramset);//*/
    pthread_getschedparam(tid0, &policyget, &paramget);
    printf("[thr 0] : policy = %d, priority = %d\n", policyget, paramget.sched_priority);

    /* Thread 1 */
    pthread_create(&tid1, NULL, thr1, NULL);
    // 运行时设置优先级，并查看
    policyset = SCHED_RR; // SCHED_FIFO;
    paramset.sched_priority = 15;
    pthread_setschedparam(tid1, policyset, &paramset);//*/
    pthread_getschedparam(tid1, &policyget, &paramget);
    printf("[thr 1] : policy = %d, priority = %d\n", policyget, paramget.sched_priority);
    // 等待运行后 2 秒，提高线程 1 的优先级，看在 FIFO 策略下是否会抢占；
    sleep(3);
    policyset = SCHED_RR; // SCHED_FIFO;
    paramset.sched_priority = 16;
    pthread_setschedparam(tid1, policyset, &paramset);//*/

    // 等待运行后 4 秒，提高线程 0 的优先级，看在 FIFO 策略下是否会抢占；
    sleep(2);
    policyset = SCHED_RR; // SCHED_FIFO;
    paramset.sched_priority = 16;   // 如果设置为与 另一线程相同，则在 RR 策略下，应该会轮转
    pthread_setschedparam(tid0, policyset, &paramset);//*/

    for (;;)
        sleep(60);

    exit(0);
}
