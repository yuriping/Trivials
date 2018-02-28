#include <stdio.h>

#define TEST(str, ...)	printf(__VA_ARGS__);	\
			printf("%s\n", str);		\

int main(void)
{
    TEST("yuriping", "%d\n", 23);

    return 0;
}


