/*
 * Linux 下 C 语言常用的时间类函数的 demo
 *
 * 0. struct tm 中:
 *   年 从 0 起，1900 年为 0 年；
 *   月 从 0 起，1 月 为 0 月；
 *   日 从 1 起；
 *   星期 从 0 开始，0 对应周日，6 对应周六；
 *   一年中第几天 从 0 起；1 月 1 日为 第 0 天；
 * 1. localtime-mktime 是可逆的，只有从 struct tm 本地时间 恢复的 time_t 才能还原；
 */

#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <time.h>
#include <unistd.h>

int main()
{
    time_t seconds;
    struct tm * p, result;

    /* time UTC，从 1970-01-01 00:00:00 到当前的秒数 */
    seconds = time((time_t *)NULL);
    printf("time : seconds = %ld\n\n", seconds);

    ////////////////////////////////////////////////////////////////

    /* localtime，从秒数转换到struct tm，具体时间，local，非线程安全 */
    time(&seconds);
    p = localtime(&seconds);
    printf("localtime : %d-%d-%d , %d:%d:%d\n", 1900 + p->tm_year, 1 + p->tm_mon, p->tm_mday, p->tm_hour, p->tm_min, p->tm_sec);

    sleep(1);

    /* localtime_r，从秒数转换到struct tm，具体时间，local，线程安全 */
    time(&seconds);
    p = localtime_r(&seconds, &result);
//    printf("%d-%d-%d , %d:%d:%d\n", 1900 + p->tm_year, 1 + p->tm_mon, p->tm_mday, p->tm_hour, p->tm_min, p->tm_sec);
    printf("localtime_r : %d-%d-%d , %d:%d:%d\n",
        1900 + result.tm_year, 1 + result.tm_mon, result.tm_mday, result.tm_hour, result.tm_min, result.tm_sec);

    ////////////////////////////////////////////////////////////////

    /* gmtime, asctime，从秒数转换出 struct tm，具体时间，GMT */
    // 沿用上一节的 seconds，以和本地时间做比较 GMT
    printf("gmtime : %s\n", asctime(gmtime(&seconds)));

    ////////////////////////////////////////////////////////////////

    /* ctime */
    // 沿用上一节的 seconds，以和本地时间做比较 local
    printf("ctime : %s\n", ctime(&seconds));


    ////////////////////////////////////////////////////////////////

    sleep(1);
    printf("================================================================ \n");

    struct tm * p_local, result_local;
    struct tm * p_gmt, result_gmt;

    /* mktime */
    // 得到当前的秒数
    time(&seconds);
//    seconds = 1526811503;   // 赋值直接测试
    printf("time : seconds = %ld\n\n", seconds);
    // 从秒数得到本地时间
    p_local = localtime_r(&seconds, &result_local);
    printf("localtime_r (raw) : %d-%d-%d-%d (%d) <%d> %d:%d:%d \n",
            p_local->tm_year, p_local->tm_mon, p_local->tm_mday, p_local->tm_wday, p_local->tm_yday, p_local->tm_isdst,
            p_local->tm_hour, p_local->tm_min, p_local->tm_sec);
    printf("localtime : %s\n", asctime(p_local));
    // 从秒数得到 GMT 时间
    p_gmt   = gmtime_r(&seconds, &result_gmt);
    printf("gmtime_r (raw) : %d-%d-%d-%d (%d) <%d> %d:%d:%d \n",
            p_gmt->tm_year, p_gmt->tm_mon, p_gmt->tm_mday, p_gmt->tm_wday, p_gmt->tm_yday, p_gmt->tm_isdst,
            p_gmt->tm_hour, p_gmt->tm_min, p_gmt->tm_sec);
    printf("gmt time : %s\n", asctime(p_gmt));

    /* localtime-mktime 是可逆的，只有从 struct tm 本地时间 恢复的 time_t 才能还原； */
    // 从 localtime 恢复出秒数
    seconds = mktime(p_local);
    printf("mktime from local : seconds = %ld\n", seconds);
    // 从 GMT 恢复出秒数
    seconds = mktime(p_gmt);
    printf("mktime from gmt : seconds = %ld\n", seconds);

    ////////////////////////////////////////////////////////////////

    printf("================================================================ \n");

    struct timeval tv;
    struct timezone tz;
    gettimeofday(&tv, &tz);
    printf("time of day : %ld . %ld\n", tv.tv_sec, tv.tv_usec);
    printf("time zone :   %d , %d\n", tz.tz_minuteswest, tz.tz_dsttime);

    printf("================================================================ \n");

    time_t t99;
    struct tm *ptm99, tm99;
    tm99.tm_year = 9999 - 1900;
    tm99.tm_mon = 11;
    tm99.tm_mday = 31;
    tm99.tm_hour = 23;
    tm99.tm_min = 59;
    tm99.tm_sec = 59;
    t99 = mktime(&tm99);
    printf("time of 9999 = %ld\n", t99);

    exit(0);
}
