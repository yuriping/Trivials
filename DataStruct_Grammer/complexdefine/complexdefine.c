#include <stdio.h>

#define	CHANNEL_PROMPT(channelClass, sn, count)	\
int i;						\
for (i=0; i<(count); i++)			\
printf("%s-%d\n", (channelClass), (sn));

int main(void)
{
    CHANNEL_PROMPT("Camera", 2, 5);

    return 0;
}

