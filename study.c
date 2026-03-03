#include <stdlib.h>
#include <stdio.h>

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

int enqueue(Queue *q, int data)
{
    // create a new node
    Node *temp = malloc(sizeof(Node));
    temp->data = data;
    temp->next = NULL;

    // if there is a tail, connect that tail to this new node
    if(q->back != NULL)
        q->back->next = temp;
    q->back = temp;

    // make sure the head still makes sense
    if(q->front == NULL)
        q->front = temp;
    
    free(temp);
    return;
}

int deQueue(Queue *q)
{
    int result;

    if(q == NULL) // check if q even exists
        return;

    if(q->front == NULL) // check if there is a node to take
        return;

    Node *temp = q->front;
    
    result = temp->data;
    q->front = q->front->next;
    if(q->front == NULL)
        q->back == NULL; // if front reaches the end, make sure the back is there!
    
    free(temp);
    return result;
}



int main()
{

}
