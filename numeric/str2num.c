/*
 * 从字符串得到整数值，32、64位，无、有符号 演示；
 *
 * NOTE :
 *   0. 该程序在 32 位系统编译执行，所以 (signed) int 与 (signed) long int 都是 32 位，取值范围相同；
 *   1. 数类型、使用转换函数、打印时的格式控制符 必须严格按照该 demo 的使用，否则可能混乱；
 *   2. strtoul、strtoull 的 参数1（0起），表征符合条件数字的最后一个位置的变量指针，可以为 NULL，表示对这个不关心；
 */


#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>


int main(void)
{
    char * str, * pEndCh =    NULL;

    /* 无符号-整 */ // [0, 4294967295]
    unsigned           int  ui;
    // 负数，得到正常最大 +1 减去该负数绝对值的正数
    str = "-123";                   ui = strtoul(str, &pEndCh, 10);         printf("%20s - %u\n", str, ui);
    // 可转最大 正常
    str = "4294967295";             ui = strtoul(str, &pEndCh, 10);         printf("%20s - %u\n", str, ui);
    // 超限 得到正常最大
    str = "8589934591";             ui = strtoul(str, &pEndCh, 10);         printf("%20s - %u\n", str, ui);

    printf("--------------------------------\n");

    /* 有符号-整 */ // [-2147483648, 2147483647]
    signed             int  si;
    // 比最大负数还小，但不小很多，得到最大负数
    str = "-2147483650";            si = strtol(str, &pEndCh, 10);          printf("%20s - %d\n", str, si);
    // 可转最大负数，正常
    str = "-2147483648";            si = strtol(str, &pEndCh, 10);          printf("%20s - %d\n", str, si);
    // 可转最大正数，正常
    str = "2147483647";             si = strtol(str, &pEndCh, 10);          printf("%20s - %d\n", str, si);
    // 比最大正数还大，但不超过 32 位 最大无符号数最大值，得到最大正数
    str = "2147483649";             si = strtol(str, &pEndCh, 10);          printf("%20s - %d\n", str, si);

    printf("--------------------------------\n");

    /* 无符号-长整 */    // [0, 4294967295]
    unsigned long      int  uli;
    // 负数，得到正常最大 +1 减去该负数绝对值的正数
    str = "-123";                   uli = strtoul(str, &pEndCh, 10);        printf("%20s - %lu\n", str, uli);
    // 可转最大 正常
    str = "4294967295";             uli = strtoul(str, &pEndCh, 10);        printf("%20s - %lu\n", str, uli);
    // 超限 得到正常最大
    str = "8589934591";             uli = strtoul(str, &pEndCh, 10);        printf("%20s - %lu\n", str, uli);

    printf("--------------------------------\n");

    /* 有符号-长整 */    // [-2147483648, 2147483647]
    signed   long      int  sli;
    // 比最大负数还小，但不小很多，得到最大负数
    str = "-2147483650";            sli = strtol(str, NULL, 10);         printf("%20s - %ld\n", str, sli);
    // 可转最大负数，正常
    str = "-2147483648";            sli = strtol(str, &pEndCh, 10);         printf("%20s - %ld\n", str, sli);
    // 可转最大正数，正常
    str = "2147483647";             sli = strtol(str, &pEndCh, 10);         printf("%20s - %ld\n", str, sli);
    // 比最大正数还大，但不超过 32 位 最大无符号数最大值，得到最大正数
    str = "2147483649";             sli = strtol(str, &pEndCh, 10);         printf("%20s - %ld\n", str, sli);

    printf("--------------------------------\n");

    /* 无符号-长长整 */   // [0, 18446744073709551615]
    unsigned long long int  ulli;
    // 负数，得到正常最大 +1 减去该负数绝对值的正数
    str = "-123";                   ulli = strtoull(str, &pEndCh, 10);      printf("%20s - %llu\n", str, ulli);
    // 可转最大 正常
    str = "18446744073709551615";   ulli = strtoull(str, &pEndCh, 10);      printf("%20s - %llu\n", str, ulli);
    // 超限 得到正常最大
    str = "18446744073709551700";   ulli = strtoull(str, &pEndCh, 10);      printf("%20s - %llu\n", str, ulli);

    printf("--------------------------------\n");

    /* 有符号-长长整 */   // [-9223372036854775808, 9223372036854775807]
    signed   long long int  slli;
    // 比最大负数还小，但不小很多，得到最大负数
    str = "-9223372036854775810";   slli = strtoll(str, &pEndCh, 10);       printf("%20s - %lld\n", str, slli);
    // 可转最大负数，正常
    str = "-9223372036854775808";   slli = strtoll(str, &pEndCh, 10);       printf("%20s - %lld\n", str, slli);
    // 可转最大正数，正常
    str = "9223372036854775807";    slli = strtoll(str, &pEndCh, 10);       printf("%20s - %lld\n", str, slli);
    // 比最大正数还大，但不超过 32 位 最大无符号数最大值，得到最大正数
    str = "9223372036854775810";    slli = strtoll(str, NULL, 10);       printf("%20s - %lld\n", str, slli);

    printf("--------------------------------\n");

    exit(0);
}
