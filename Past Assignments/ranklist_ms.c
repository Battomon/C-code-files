/* COP 3502C PA4
This program is written by: Elijah Trees */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define MAXSIZE         12 //  is the maximum size of all strings in the problem, including both cat names and trait names.
#define NUMTRAITS       7 // is the total number of possible keys for sorting (six traits plus one for the total score).
#define TOTAL           6 // is the index into the scores array indicating the total score for a cat
#define BASECASESIZE    30 //is the size at which the recursive sorting algorithm must switch to insertion sort.

typedef struct Cat {
 char *name;
 int scores[NUMTRAITS];
}Cat;

const char TRAITS[NUMTRAITS][MAXSIZE+1] = {"Cuteness", "Fluffiness", "Agility", "Friendliness", "Intelligence", "Laziness", "Total"};

/*
when the size of a subarray becomes less than or equal to a threshold
value of 30, the sorting method must switch to a quadratic-time sorting algorithm, insertion sort

must also be implemented using a wrapper function that initiates the sorting
process. The wrapper function will call the actual recursive sorting function that performs the divide-andconquer steps.

In addition, you must implement a comparison function that determines the ordering between two cats.
This function will compare cats based on the selected trait and break ties alphabetically using the cat's
name. Both sorting algorithms will use this comparison function when determining ordering.

Memory for the cat structures and their names must be dynamically allocated. The array of cat pointers
must be dynamically allocated based on the value of n. Each cat name must also be dynamically allocated
using exactly the amount of memory required for that string, and you must use the same strategy of
dynamic memory allocation of strings that we have learned in class. Any other approach may result in 
-100 in the class.
*/

//                          ~input~
// num of cats

// name cuteScore fluffScore agileScore friendlyScore smartScore lazyScore (xNUMCATS)
// (all scores between 1-1000)

// g (trait to sort all cats by)

/*
g | Trait
--|----------
0 | Cuteness
1 | Fluffiness
2 | Agility
3 | Friendliness
4 | Intelligence
5 | Laziness
6 | Total score across all traits
*/


Cat* createCat()
{
    char name[MAXSIZE+1]; 
    int scores[NUMTRAITS];
    //printf("cat name: "); 
    scanf("%s", name);
    
    Cat *cat = malloc(sizeof(Cat));
    cat->name = malloc(strlen(name)+1);
    strcpy(cat->name, name);
    cat->scores[NUMTRAITS-1] = 0;
    for(int i = 0; i < NUMTRAITS-1; i++)
    {
        scanf("%d", &(scores[i]));
        cat->scores[i] = scores[i];
        cat->scores[NUMTRAITS-1] += scores[i];
    }

    return cat;
}

int compareTo(Cat* ptrC1, Cat* ptrC2, int key)
{
    if(ptrC1->scores[key] == ptrC2->scores[key])
        return strcmp(ptrC1->name, ptrC2->name); // tie breaker

    else if(ptrC1->scores[key] > ptrC2->scores[key])
        return -1; // if comes before, -1
    else
        return 1; // comes after, 1
}

void insertionSort(Cat** list, int low, int high, int key)
{
    for(int i = low + 1; i <= high; i++)
    {
        Cat* temp = list[i];
        int j = i - 1;
        while(j >= low && compareTo(list[j], temp, key) > 0)
        {
            list[j+1] = list[j];
            j--;
        }
        list[j+1] = temp;
    }
}

// Merges two sorted subarrays
void merge(Cat** list, int low, int mid, int high, int key)
{
    int i = low;
    int j = mid+1;
    int k = 0;

    Cat** temp = malloc(sizeof(Cat*) * (high - low + 1));

    while(i <= mid && j <= high)
    {
        if(compareTo(list[i], list[j], key) <= 0)
        {
            temp[k] = list[i];
            i++;
        }
        else
        {
            temp[k] = list[j];
            j++;
        }
        k++;
    }

    while(i <= mid)
    {
        temp[k] = list[i];
        i++;
        k++;
    }

    while(j <= high)
    {
        temp[k] = list[j];
        j++;
        k++;
    }

    for(i = 0; i < high - low + 1; i++)
        list[low + i] = temp[i];

    free(temp);
}

// Performs a Merge Sort on list[low..high] according to the trait
// indicated by the integer key.
void mergeSortRec(Cat** list, int low, int high, int key)
{
    if(high - low + 1 <= BASECASESIZE)
    {
        insertionSort(list, low, high, key);
        return;
    }

    int mid = low + (high - low) / 2;
    mergeSortRec(list, low, mid, key);
    mergeSortRec(list, mid+1, high, key);
    merge(list, low, mid, high, key);
}

// Merge Sorts the array list of size n according to the trait
// indicated by the integer key.
void mergeSort(Cat** list, int n, int key)
{
    mergeSortRec(list, 0, n-1, key);
}



//                        ~output~
// {trait} Ranklist:
//x. {name} {score} (xNUMCATS)
//(use printf("%d. %-15s %d\n", rank, name, score))
void printList(Cat** list, int numCats, int key)
{
    printf("%s Ranklist\n", TRAITS[key]);
    for(int i = 0 ; i < numCats; i++)
        printf("%d. %-15s %d\n", (i+1), list[i]->name, list[i]->scores[key]);
}

int main()
{

    //printf("Enter the number of Cats. "); 
    int numCats; scanf("%d", &numCats); // inital input

    Cat **list = malloc(sizeof(Cat*) * numCats);
    for(int i = 0 ; i < numCats; i++)
        list[i] = createCat();
    
    //printf("trait to sort: "); 
    int key; scanf("%d", &key);

    mergeSort(list, numCats, key);



    printList(list, numCats, key);

        // BE FREEEEEEEE KITTIIIES
    for(int i = 0; i < numCats; i++) {
        free(list[i]->name);
        free(list[i]);
    }
    free(list);

    return 0;
}
