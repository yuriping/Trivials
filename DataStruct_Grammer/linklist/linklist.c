#include <stdio.h>
#include <stdlib.h>

typedef struct Node{
	int id;
	struct Node * next;
} NODE, * LIST;

LIST List_Init()
{
    LIST list = (LIST) malloc(sizeof(NODE));
    list->id   = 0x00;
    list->next = NULL;

    return list;
}

LIST List_Add(LIST list, int i)
{
    NODE * node = (NODE *) malloc(sizeof(NODE));
    node->id = i;
    node->next = NULL;

    node->next = list->next;
    list->next = node;

    return list;
}

void List_Print(LIST list)
{
    LIST p = NULL;
    for (p=list->next; p; p=p->next)
    {
        printf("%5d", p->id);
    }
    printf("\n");
}

LIST List_Deinit(LIST list)
{
    NODE * p = list;
    while (p)
    {
        NODE * q = p;
        p = p->next;
        printf("free node data : %d\n", q->id);
        free(q);
    }
    list = NULL;

    return list;
}

int main(void)
{
    LIST list = List_Init();

    int i;
    for (i=0; i<10; i++)
    {
        List_Add(list, i);
    }

    List_Print(list);

    list = List_Deinit(list);

    printf("\n0x%x\n", (unsigned int)list);

    return 0;
}
//*/

