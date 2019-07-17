
#ifndef ____B_H____
#define ____B_H____


#include "./a_a.h"


typedef struct B
{
    char    ch;

    AAA *   pa;   // 只能用结构 A 的别名访问 A

} B;


#endif /* ____B_H____ */
