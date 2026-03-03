#include <stdlib.h>
#include <stdio.h>
#include <string.h>

typedef struct Node
{
    int data;
    struct Node *next;

} Node;

typedef struct Queue
{
    Node *front;
    Node *back;
} Queue;

void enqueue(Node **list, int num)
{
    Node *temp = malloc(sizeof(temp));
    temp->data = num;
    temp->next = NULL;
    
    if(*list == NULL)
    {
        *list = temp;
        return;
    }
    
    Node *curr = *list;

    // put the node at the end of the list
    while(curr->next)
        curr = curr->next;
    curr->next = temp;

    return;
}

int main()
{
    Node *list = NULL;

    int input;
    while(1)
    {
        scanf("%d", &input);
        if(input == -1)
            break;
        enqueue(&list, input);
    }

    Node *curr = list;
    while(curr)
    {
        printf("%d ", curr->data);
        curr = curr->next;
    }

}