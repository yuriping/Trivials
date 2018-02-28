#include <stdio.h>
#include <sys/time.h>


int main(void)
{
    printf("char          : %d\n", sizeof(char));
    printf("short         : %d\n", sizeof(short));
    printf("int           : %d\n", sizeof(int));
    printf("long          : %d\n", sizeof(long));
    printf("long int      : %d\n", sizeof(long int));
    printf("long long int : %d\n", sizeof(long long int));

    long long int data;
    data = 1LL << 63 - 1;
    printf("%lld\n", data);

    struct timeval m_time;
    gettimeofday(&m_time, NULL);
    long long int tick;
    printf("%ld.%ld\n", m_time.tv_sec, m_time.tv_usec);
    tick = m_time.tv_sec * 1000LL + m_time.tv_usec / 1000;
    printf("tick = %lld\n", tick);
    tick = (long long int)m_time.tv_sec * 1000 + m_time.tv_usec / 1000;
    printf("tick = %lld\n", tick);
}
