#include <stdlib.h>

typedef struct
{
    int top;
    int array[10];
} arrayStack;

typedef struct Node
{
    int data;
    struct Node *next;
} Node;

typedef struct
{
    int top;
    Node *head;
} linkListStack;

typedef struct
{
    Node *front;
    Node *back;
} linkedListQueue;




arrayStack* pushArray(arrayStack *stack, int data)
{
    if(stack->top = 10)
    {
        printf("stack overflow! stack too full.");
        return stack;
    }
    stack->array[stack->top++] = data;
    return stack;
}

int popArray(arrayStack *stack)
{
    if(stack->top < 0)
    {
        printf("stack overflow! Stack empty.");
        return;
    }
    return stack->array[stack->top--];
}


pushLinkList(linkListStack *stack, int data)
{
    if(stack->top == 0)
    {
        stack->head->data = data;
        stack->top++;
        return stack;
    }

    Node *temp = stack->head;

    int index;
    while(temp->next != NULL)
    {
        temp = temp->next;
        index++;
    }
    temp->data = data;
    stack->top = index;
    return stack;
}

popLinkList(linkListStack *stack)
{
    if(stack->top < 0 || stack->head)
    {
        printf("stack overflow! stack empty.");
        return;
    }

    Node *temp = stack->head;

    while(temp->next)
        temp = temp->next;

    int data = temp->data;
    stack->top--;

    free(temp->next);
    return data;
    

}





enquque(int data, linkedListQueue *queue)
{
    Node *newNode = malloc(sizeof(Node));
    if(!newNode)
    {
        printf("memory not allocated properly");
        return -1;
    }

    newNode->data = data;

    if(queue->back == NULL)
    {
        queue->front = newNode;
        queue->back = newNode;
        return;
    }

    queue->back->next = newNode;
    queue->back = newNode;
    return;
}

dequeue(linkedListQueue *queue)
{
    if(queue->front == NULL)
    {
        printf("queue empty");
        return -1;
    }

    Node *temp = queue->front;

    int data = temp->data;

    queue->front = temp->next;
    if(queue->front == NULL)
        queue->back = NULL;
    
    free(temp);
    return data;
}

