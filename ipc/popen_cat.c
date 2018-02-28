/*
 * 演示功能：
 * popen 的写打开工作方式
 */

#include <stdio.h>
#include <sys/wait.h>

#define CAT	"${CAT:-cat}"

int main(int argc, char * argv[])
{
    FILE *fTxt, *fpCat;

    fTxt = fopen(argv[1], "r");

    fpCat = popen(CAT, "w");

    char buf[1024];

    while (fgets(buf, 1024, fTxt) != NULL)
    {
        fputs(buf, fpCat);
    }

    pclose(fpCat);

    return 0;
}

