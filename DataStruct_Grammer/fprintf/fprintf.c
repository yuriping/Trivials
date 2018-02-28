#include <stdio.h>

#define LOGFILE	"abc"

#define PUT_LOG(str, ...)	flog = fopen(str, "a+");	\
				fprintf(flog, __VA_ARGS__);	\
				fclose(flog);			\

int main(void)
{
    FILE * flog;

    PUT_LOG(LOGFILE, "%d\n", 23);

    PUT_LOG(LOGFILE, "yuriping \n");

    return 0;
}


