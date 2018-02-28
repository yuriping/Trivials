#include <stdio.h>
#include <stdlib.h>


struct ABC
{
    int  i;
    char ch;
} __attribute__((packed));

typedef struct ABC * ABC;

int main(void)
{
    printf("sizeof struct ABC = %d, sizeof ABC = %d\n", sizeof(struct ABC), sizeof(ABC));

    ABC pabc = malloc(sizeof(struct ABC));
    pabc->i = 10;
    pabc->ch = 'a';
    printf("i = %d, ch = %c\n", pabc->i, pabc->ch);

    struct ABC abc;
    abc.i = 6;
    abc.ch = 'c';
    printf("i = %d, ch = %c\n", abc.i, abc.ch);

    return 0;
}

