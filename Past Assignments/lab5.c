#include <stdio.h>
#include <stdlib.h>

struct node* insert(struct node* list, int d );
struct node* del(struct node* list ,int d );
void print( struct node *list);
void freeList(struct node* list);
void copy ( struct node *q, struct node **s );


struct node
{
    int data;
    struct node *next;
};




int main( ) 
{
 int number = 0;
 int choice = 0;
 struct node *pList = NULL;
 struct node *nList = NULL;

 // Let the user add values until they enter -1.
 while(choice != 4)
 {
    // Get the operation.
    printf("\nDo you want to (1)insert, (2)delete, (3)Copy (4)quit.\n");
    scanf("%d", &choice);
    printf("Your choice is %d\n", choice);

    // Execute the operation.
    if (choice == 1)
    {
        // Get the number.
        printf("Enter the value to insert\n");
        scanf("%d", &number);
        pList = insert(pList, number);

        // Look at the list.
        printf("Items in linked list: ");
        print(pList);
        //printf("\n");
    }
    else if (choice == 2)
    { 
        // Get the number.
        printf("Enter the value to delete.\n");
        scanf("%d", &number);
        pList = del(pList, number);
        
        // Look at the list.
        printf("Items in linked list: ");
        print(pList);
        //printf("\n");
    }
    else if (choice == 3)
    {
        if (nList)
        {
            freeList(nList);
            nList = NULL;
        }
        copy(pList, &nList); //passing reference of nList as it is not returning anything

        // Look at the list.
        printf("Items in NEW linked list: ");
        print(nList);
        // printf("\n");
    }
    else
        break;
 }

 freeList(nList);
 freeList(pList);

 printf("\nBye..\n");
 return 0;

}

// copies a linked list into another
void copy ( struct node *source, struct node **dest )
{
    if(source != NULL)
    {
        *dest = malloc(sizeof(struct node));
        (*dest)->data = source->data;
        (*dest)->next = NULL;

        copy(source->next, &((*dest)->next));
    }
}

// recursivly inserts item into the LL pointed to by list.
struct node* insert(struct node* list, int item )
{
    if(list == NULL || item <= list->data)
    {
        struct node *temp = (struct node*)malloc(sizeof(struct node));
        temp->data = item;
        temp->next = list;
        return temp;
    }
    list->next = insert(list->next, item);
    return list;
}

// recursivly deletes the first node storing item if it exists
struct node* del(struct node* list ,int item )
{
    if(list == NULL)
        return NULL;
    if(list->data == item)
    {
        struct node *temp = list->next;
        free(list);
        return temp;
    }
    list->next = del(list->next, item);
    return list;

}

// prints every item in LL
void print( struct node *list)
{
    if(list == NULL)
        return;

    printf("->%d", list->data);
    print(list->next);
}

// frees each node in LL printed by list
void freeList(struct node* list)
{
    if(list == NULL)
        return;
    if(list->next != NULL)
        freeList(list->next);
    free(list);
}