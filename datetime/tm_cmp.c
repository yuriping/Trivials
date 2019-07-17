/*
 * 比较日期时间谁前谁后的一个功能，从 tm 时间转换成 time_t ，只要比较数的大小等于就行了，简单，不用逐个比较 tm 结构中的分量
 * 暂不考虑封装成函数，大部分内容都是指定 tm 分量，mktime 再 比较一下就完了，没必要...
 */

#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <time.h>
#include <unistd.h>

int main()
{
    time_t sec_curr, sec_spot;
    struct tm tm_curr, tm_spot;

    /* 获取当前时间 */
    // 秒形式
    sec_curr = time((time_t *) NULL);
    printf("curr : seconds = %ld\n",  sec_curr);
    // 时间形式
    localtime_r(&sec_curr, &tm_curr);   // NOTE : 不用 localtime ，那个不是线程安全的；这个线程安全
    printf("curr : %d-%d-%d, %d:%d:%d\n",
        1900 + tm_curr.tm_year, 1 + tm_curr.tm_mon, tm_curr.tm_mday,
        tm_curr.tm_hour, tm_curr.tm_min, tm_curr.tm_sec);

    /* 比较点 */   // 当天内，根据当前时间，手工指定时间点，提前、滞后于当前时间，看效果
    // 复制当前时间，并修改时分秒，不修改，就作为正好到了比较点处理
    // 如果比较不在当天的，只要把年月日也改了就行
    tm_spot = tm_curr;
#if 0
    tm_spot.tm_hour = 11;
    tm_spot.tm_min  = 30;
    tm_spot.tm_sec  = 0;
#endif
    printf("curr : %d-%d-%d, %d:%d:%d\n",
        1900 + tm_spot.tm_year, 1 + tm_spot.tm_mon, tm_spot.tm_mday,
        tm_spot.tm_hour, tm_spot.tm_min, tm_spot.tm_sec);
    // 取得比较点的秒形式
    sec_spot = mktime(&tm_spot);
    printf("spot : seconds = %ld\n",  sec_spot);
    // 比较当前时间与时间点
    int flag = 100;
    if (sec_curr < sec_spot)
        flag = -1;
    else if (sec_curr == sec_spot)
        flag = 0;
    else
        flag = 1;
    printf("flag = %d\n", flag);

    exit(0);
}
