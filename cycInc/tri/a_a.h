/*
 * 这个头文件可以看做结构 A 的输出头文件，仅供循环包含的结构 B 使用，其他地方不用
 * 将结构 A 说明为别名 AAA
 * 文件名：a_a.h, short for : a_alias.h，别名文件
 */


#ifndef ____A__A_H____
#define ____A__A_H____


#include "./a.h"


typedef struct A    AAA;


#endif /* ____A__A_H____ */

