#include <stdlib.h>
#include <stdio.h>

typedef struct Node
{
    int data;
    struct Node *next;
} Node;

void insertBeginning(Node **head, Node *newNode)
{
    newNode->next = *head;
    *head = newNode;
}

void insertEnd(Node **head, Node *newNode)
{
    if((*head) == NULL)
    {
        *head = newNode;
        return;
    }

    Node *temp = *head;
    while(temp->next)
        temp = temp->next;
    temp->next = newNode;
}

void insertMid(Node **head, Node *newNode, int index)
{
    Node *temp = *head;
    for(int i = 0; i < index; i++)
    {
        if(!temp)
        {
            printf("overrun bounds");
            return;
        }
        temp = temp->next;
    }
    newNode->next = temp->next;
    temp->next = newNode;
    return;
}

void printList(Node **head)
{
    if((*head)==NULL)
        printf("list empty");
    Node *temp = *head;
    while(temp)
    {
        printf("%d -> ", temp->data);
        temp = temp->next;
    }
    printf("NULL\n");
}

void freeList(Node **head)
{
    if(!(*head))
        return;
    Node *temp = *head;
    while(*head)
    {
        temp = *head;
        *head = (*head)->next;
        free(temp);
    }
}

int main()
{
    Node *head = NULL;

    int data;
    char pos;
    while(1)
    {
        newNode->next = NULL;
        printf("pos f(front), b(back), m(middle) e(exit): "); scanf(" %c", &pos);
        if(pos == 'e')
            break;
        zz
        Node *newNode = malloc(sizeof(Node));

        printf("data: "); scanf("%d", &data);
        newNode->data = data;

        if(pos == 'f')
            insertBeginning(&head, newNode);
        else if(pos == 'b')
            insertEnd(&head, newNode);
        else if(pos == 'm')
        {
            int index;
            printf("index: "); scanf("%d", &index);
            insertMid(&head, newNode, index);
        }
    }

    printList(&head);
}