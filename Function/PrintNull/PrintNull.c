#include <stdio.h>

int main(void)
{
    FILE * file = fopen("out", "a+");

    fprintf(file, "%d\n", 123);
    
    fclose(file);

    file = fopen("out", "a+");

    fprintf(file, "%s\n", "yuriping");

    fclose(file);

    return 0;
}


