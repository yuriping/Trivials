/*
 * 循环包含的实现：方法一 三个头文件
 *     结构 A 包含 B，结构 B 包含 A；
 *     取任意一个结构 A ，建立其输出头文件，typedef 结构 A 为 别名，供 结构 B 引用，
 *     形成 A0 包含 A，A 包含 B，B 包含 A0 的局面，就可以了
 */


#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "./a.h"
#include "./b.h"


int main()
{
    A   a;
    a.i = 4;

    B   b;
    b.ch = 'c';

    a.pb = &b;
    b.pa = &a;

    printf("%d , %c\n", a.i, a.pb->ch);

    printf("%d\n", a.pb->pa->i);

    exit(0);
}
