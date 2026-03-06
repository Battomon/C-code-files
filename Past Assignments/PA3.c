/* COP 3502C PA3 This program is written by: Elijah Trees */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>



typedef struct Cat_s {
 int arrival; // Minutes since clinic opened (Unique)
 char *name; // Unique cat name
 int duration; // Required treatment time in minutes
} Cat;

typedef struct SLLNode_s {
 Cat *cat; // Pointer to a cat
 struct SLLNode_s *next;
} SLLNode;

// Stack structure for Dr. Dos's treated cats
typedef struct Stack_s {
    SLLNode *top;
} Stack;

/*
This code will use singly linked lists to model a dynamic waiting queue between several doctors. When one is busy, the other must adress the next cat in line. 
The program will read in a list of cats, and then simulate the clinic's operations, printing out the state of the waiting queue at each minute. 
The program will also print out the total time each cat spent waiting before being treated.
*/

int operatingHours; // The clinic is open for 8 hours, which is 480 minutes. (start_time + duration ≤ 480)


/*
If one or more cats were treated by Dr. Dos, print the names of exposed cats in reverse chronological order of treatment (most recently treated first)
in the format:
Exposed Cats
<name>
<name>
...

If no cats were treated by Dr. Dos, print:
No Exposed Cats
*/

Cat* createCat(int arrival, char *name, int duration)
{
    Cat *newCat = (Cat*)malloc(sizeof(Cat));
    newCat->arrival = arrival;
    newCat->name = (char*)malloc((strlen(name) + 1) * sizeof(char));
    strcpy(newCat->name, name);
    newCat->duration = duration;

    return newCat;
}


void enqueue(SLLNode **front, SLLNode **rear, Cat *cat)
{
    SLLNode *newNode = (SLLNode*)malloc(sizeof(SLLNode));
    newNode->cat = cat;
    newNode->next = NULL;
    if (*rear == NULL) 
    {
        *front = newNode;
        *rear = newNode;
    } 

    // If the new cat arrives before the current front cat, it becomes the new front
    else if(cat->arrival < (*front)->cat->arrival)
    {
        newNode->next = *front;
        *front = newNode;
    }
    else
    {
        // Traverse the queue until the current cat's arrival time is less than the new cat's arrival time
        SLLNode *curr = *front;
    SLLNode *prev = NULL;

    // walk until we find a cat with a larger arrival time or reach end
    while (curr != NULL && curr->cat->arrival <= cat->arrival) {
        prev = curr;
        curr = curr->next;
    }

    if (prev == NULL) {
        // this should not happen since we handled front insertion above, but
        // we'll treat it as inserting at front just in case
        newNode->next = *front;
        *front = newNode;
    } else {
        // insert after prev
        prev->next = newNode;
        newNode->next = curr;
        if (prev == *rear) {
            // inserted at end of queue
            *rear = newNode;
        }
    }
    }
}

Cat* dequeue(SLLNode **front, SLLNode **rear)
{
    // check if the queue is empty
    if (*front == NULL)
        return NULL;

    // create a temporary node to hold the front of the queue, then move the front pointer to the next node
    SLLNode *temp = *front;
    *front = (*front)->next;

    // if the queue is now empty, set the rear pointer to NULL as well
    if (*front == NULL)
        *rear = NULL;
    
    // store the cat to return before freeing the node
    Cat *cat = temp->cat;
    free(temp);
    return cat;
}

Cat* peek(SLLNode **front, SLLNode **rear)
{
    // return the cat at the front of the queue without removing it
    if (*front == NULL) {
        return NULL;
    }
    return (*front)->cat;
}

int queueIsEmpty(SLLNode **front, SLLNode **rear)
{
    // return 1 if the queue is empty, 0 otherwise
    return (*front == NULL)? 1 : 0;
}

int main()
{
    int arrival = 0; // Minutes since clinic opened
    char name[26]; // max name length is 25 characters (plus the null terminator)
    int duration; // Required treatment time in minutes
    int numCats = 0; // Number of cats that will be treated (max 100)
    operatingHours = 480; // The clinic is open for 8 hours (480 minutes)

    // initialize the waiting queue
    SLLNode *front;
    SLLNode *rear;
    front = NULL;
    rear = NULL;

    // take input until -1 is entered for arrival time
    while (1) {
        scanf("%d", &arrival);
        if (arrival == -1)
            break;

        scanf("%s", name);
        scanf("%d", &duration);
        if(duration == 0)
            continue; // skip cats that require no treatment time
        Cat *newCat = createCat(arrival, name, duration);
        numCats++;

        // sort cat in order of arrival time (if two cats have the same arrival time, sort them in order of input)
        enqueue(&front, &rear, newCat);
    }

    int drUnoAv = 0; // Dr Uno's next avaialable appointment time
    int drDosAv = 0; // Dr Dos's next avaialable appointment time

    // create an array for the names of the cats that Dr. Dos treated
    char **drDosCats = (char**)malloc(numCats * sizeof(char*));
    for(int i = 0; i < numCats; i++)
        drDosCats[i] = (char*)malloc(26 * sizeof(char)); // max name length is 25 characters (plus the null terminator)
    int ddCnt = 0; // number of cats treated by Dr. Dos

    while (queueIsEmpty(&front, &rear) == 0) 
    {
         Cat *cat = dequeue(&front, &rear);

        if (cat != NULL && cat->arrival + cat->duration <= operatingHours)
        {
            if(cat->arrival >= drUnoAv) 
            {
                printf("Doctor Uno treated %s at %d\n", cat->name, cat->arrival);
                drUnoAv = cat->arrival + cat->duration;
            } 
            else if(cat->arrival < drUnoAv && drUnoAv <= drDosAv)
            {
                printf("Doctor Uno treated %s at %d\n", cat->name, drUnoAv);
                drUnoAv = drUnoAv + cat->duration;
            }
            else if(cat->arrival >= drDosAv)
            {
                printf("Doctor Dos treated %s at %d\n", cat->name, cat->arrival);
                drDosAv = cat->arrival + cat->duration;
                strcpy(drDosCats[ddCnt++], cat->name);
            }
            else if(cat->arrival < drDosAv && drDosAv <= drUnoAv)
            {
                printf("Doctor Dos treated %s at %d\n", cat->name, drDosAv);
                drDosAv = drDosAv + cat->duration;
                strcpy(drDosCats[ddCnt++], cat->name);
            }

        }
        else if (cat != NULL)
        {
            printf("Cannot accommodate %s\n", cat->name);
        }
        if (cat != NULL) 
        {
            free(cat->name); // Free the inner string first
            free(cat);       // Then free the struct
        }
    }

    // print the names of the cats treated by Dr. Dos in reverse chronological order
    if (ddCnt > 0) 
    {
        printf("Exposed Cats\n");
        for (int i = ddCnt - 1; i >= 0; i--)
            printf("%s\n", drDosCats[i]);
    } 
    else
        printf("No Exposed Cats\n");

    for(int i = 0; i < numCats; i++) {
        free(drDosCats[i]);
    }
    free(drDosCats);
    return 0;
}
