/* COP 3502C PA1
This program is written by: Elijah Trees */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
const char STATUS_CAT[3][20] = {"ADOPTED", "PENDING", "AVAILABLE"};

typedef struct 
Cat {
char *name; // dynamically allocated space for the name without wasting space
int age; // specifies the age of the cat
float weight; // stores the decimal value in weight for this specific cat
char *breed; //points to an already allocated breed string (Ragdoll, Siamese, Maincoone, etc) No malloc/calloc for this property
int status; //specifies if a cat is adopted, pending, or available, ranges from 0 to 2, initially set to available for newly added cats
} Cat;

typedef struct
Kennel {
char *location; // dynamically allocated space for the location without wasting space
int occupancy; // stores the current number of cats in the kennel
Cat **cats; // dynamically allocated array of pointers to Cats that reside in the kennel
int maxCapacity; // specifies the max cap to the kennel
} Kennel;

typedef struct
CatStore {
int **capacities; //dynamically allocated double int array stores the breeds constraints for all kennels
int numKennels; //specifies the total number of kennels this store owns
Kennel *kennels; // dynamically allocated array of kennels
} CatStore;

// Function Prototypes
char **readBreeds(int *count);
char *getCharPtrByBreed(char **dictionary, char *breedName, int breedCount);
CatStore *createStore(int kennelCount, int breedCount, char **dictionary);
Kennel *createKennels(int **constraints, int kennelCount, int breedCount, char **dictionary);
Cat **createCats(char **dictionary, int breedCount, int count);
Cat *createSingleCat(char **dictionary, int breedCount);
int canMoveTo(CatStore *s, char *location, char *breed, char **dictionary, int breedCount);
Kennel *getKennelByCat(CatStore *s, Cat *cat);
int getCatPosi(Kennel *home, Cat *cat);
Cat *getCatByName(CatStore *s, char *catName);
void removeCatFromKennel(Kennel *k, Cat *cat);
void runQueries(CatStore *s, char **dictionary, int breedCount, int numQueries);
void freeBreeds(char **dictionary, int breedCount);
void freeStore(int count, CatStore *store);



int main(void)
{
    char **dictionary; //to store array of dynamically allocated strings for breeds types (e.g., {"Ragdoll","Siamese", "Maincoone"})
    int breedCount; //number of breed types
    int kennelCount; //number of kennels in each store
    dictionary = readBreeds(&breedCount);
    scanf("%d", &kennelCount);
    CatStore *store = createStore(kennelCount, breedCount, dictionary);

    // Print all cats in all kennels
    /*
    for(int i = 0; i < kennelCount; i++)
    {
        for(int j = 0; j < store->kennels[i].occupancy; j++)
        {
            Cat *c = store->kennels[i].cats[j];
            printf("%s %d %.2f %s %s\n", c->name, c->age, c->weight, 
                   store->kennels[i].location, STATUS_CAT[c->status]);
        }
    }
    */

    int numQueries = 0;
    scanf("%d", &numQueries);
    runQueries(store, dictionary, breedCount, numQueries);
    
}




// takes in the number of breeds and their respective names, allocates space for them, and returns the array of breen names
char** readBreeds(int *count)
{
    int breedCount;
    scanf("%d", &breedCount);
    *count = breedCount;
    char **dictionary = (char**)malloc(breedCount * sizeof(char*));
    for(int i = 0; i < breedCount; i++)
    {
        dictionary[i] = (char*)malloc(100 * sizeof(char));
        scanf("%s", dictionary[i]);
    }
    return dictionary;
}

// returns a pointer to the location of the breed name in the dictionary
char* getCharPtrByBreed(char **dictionary, char *breedName, int breedCount)
{
    for(int i = 0; i < breedCount; i++)
    {
        if(strcmp(dictionary[i], breedName) == 0)
        {
            return dictionary[i];
        }
    }
    return NULL;
}

// creates and returns a pointer to a CatStore struct with all fields properly allocated and filled in
CatStore *createStore(int kennelCount, int breedCount, char **dictionary)
{
    CatStore *store = (CatStore*)malloc(sizeof(CatStore));

    int **constraints = (int**)malloc(kennelCount * sizeof(int*));
    for(int i = 0; i < kennelCount; i++)
    {

        constraints[i] = (int*)malloc(breedCount * sizeof(int));
        for(int j = 0; j < breedCount; j++)
        {
            int BreedCap;
            scanf("%d", &BreedCap);
            constraints[i][j] = BreedCap;
        }
    }
    (*store).numKennels = kennelCount;
    (*store).capacities = constraints;
    (*store).kennels = createKennels(constraints, kennelCount, breedCount, dictionary);
    return store;
}

// returns a DMA array of kennel sturcts with the proper number of kennels and all apropriate fields filled in 
Kennel* createKennels(int **constraints, int kennelCount, int breedCount, char **dictionary)
{
    // creates an array of kennel structs for each store
    Kennel *kennels = (Kennel*)malloc(kennelCount * sizeof(Kennel));

    // fills in each kennel struct with proper data
    for(int i = 0; i < kennelCount; i++)
    {
        //location
        kennels[i].location = (char*)malloc(100*sizeof(char));
        scanf("%s", kennels[i].location);
        //occupancy
        int occupancy;
        scanf("%d", &occupancy);
        kennels[i].occupancy = occupancy;
        //cats
        kennels[i].cats = createCats(dictionary, breedCount, occupancy);
        
        //maxCapacity
        int maxCapacity = 0;
        for(int j = 0; j < breedCount; j++)
        {
            maxCapacity += constraints[i][j];
        }
        kennels[i].maxCapacity = maxCapacity;
    }
        
    return kennels;
}

Cat* createSingleCat(char **dictionary, int breedCount)
{
    Cat *cat = (Cat*)malloc(sizeof(Cat));
    (*cat).name = (char*)malloc(100 * sizeof(char));
    scanf("%s", (*cat).name);
    scanf("%d", &(*cat).age);
    scanf("%f", &(*cat).weight);
    char breedName[100];
    scanf("%s", breedName);
    (*cat).breed = getCharPtrByBreed(dictionary, breedName, breedCount);
    (*cat).status = 2; // AVAILABLE
    return cat;
}

Cat** createCats(char **dictionary, int breedCount, int count)
{
    Cat **cats = (Cat**)malloc(count * sizeof(Cat*));
    for(int i = 0; i < count; i++)
    {
        cats[i] = createSingleCat(dictionary, breedCount);
    }
    return cats;
}

int canMoveTo(CatStore *s, char *location, char *breed, char **dictionary, int breedCount)
{
    for(int i = 0; i < (*s).numKennels; i++)
    {
        if(strcmp((*s).kennels[i].location, location) == 0)
        {
            Kennel *k = &(*s).kennels[i];
            if((*k).occupancy >= (*k).maxCapacity)
            {
                return 0; // Max capacity exceeded
            }
            int breedIndex = -1;
            for(int j = 0; j < breedCount; j++)
            {
                if(strcmp(dictionary[j], breed) == 0)
                {
                    breedIndex = j;
                    break;
                }
            }
            if(breedIndex == -1)
            {
                return 0; // Breed not found
            }
            int currentBreedCount = 0;
            for(int j = 0; j < (*k).occupancy; j++)
            {
                if(strcmp((*k).cats[j]->breed, breed) == 0)
                {
                    currentBreedCount++;
                }
            }
            if(currentBreedCount < (*s).capacities[i][breedIndex])
            {
                return 1; // Can move
            }
            else
            {
                return 0; // Breed capacity exceeded
            }
        }
    }
    return 0; // Location not found
}

Kennel* getKennelByCat(CatStore *s, Cat *cat)
{
    for(int i = 0; i < (*s).numKennels; i++)
    {
        Kennel *k = &(*s).kennels[i];
        for(int j = 0; j < (*k).occupancy; j++)
        {
            if((*k).cats[j] == cat)
            {
                return k; // Found the kennel
            }
        }
    }
    return NULL; // Cat not found in any kennel
}

int getCatPosi(Kennel *home, Cat *cat)
{
    for(int i = 0; i < (*home).occupancy; i++)
    {
        if((*home).cats[i] == cat)
        {
            return i; // Found the cat
        }
    }
    return -1; // Cat not found
}

Cat *getCatByName(CatStore *s, char *catName)
{
    for(int i = 0; i < (*s).numKennels; i++)
    {
        Kennel *k = &(*s).kennels[i];
        for(int j = 0; j < (*k).occupancy; j++)
        {
            if(strcmp((*k).cats[j]->name, catName) == 0)
            {
                return (*k).cats[j]; // Found the cat
            }
        }
    }
    return NULL; // Cat not found
}

void removeCatFromKennel(Kennel *k, Cat *cat)
{
    int pos = getCatPosi(k, cat);
    if(pos != -1)
    {
        for(int i = pos; i < (*k).occupancy - 1; i++)
        {
            (*k).cats[i] = (*k).cats[i + 1];
        }
        (*k).occupancy--;
    }
}

/*
Preconditions:
• s is a properly allocated and filled CatStore pointer
• dictionary is a strings array storing the words representing possible breeds
• breedCount is a valid int that represents the number of the breed names stored
in the dictionary
• numQueries is a valid int that represents the number of questions the function
will handle
Postconditions:
• Handles each query accordingly (more details below), it is recommended that
you write a separate function for each query and call them from this function
*/
void runQueries(CatStore *s, char **dictionary, int breedCount, int numQueries) 
{
    for(int q = 0; q < numQueries; q++)
    {
        int queryType;
        scanf("%d", &queryType);

        // Query Type 1: This query prints all cats' information of a given breed, displaying their name,
        //weight, age, location, and status.
        if (queryType == 1)
{
    char breedName[100];
    scanf("%s", breedName);
    int found = 0;
    for (int i = 0; i < s->numKennels; i++)
    {
        Kennel *k = &s->kennels[i];
        for (int j = 0; j < k->occupancy; j++)
        {
            Cat *c = k->cats[j];
            if (strcmp(c->breed, breedName) == 0)
            {
                printf("%s %.2f %d %s %s\n", c->name, c->weight, c->age,
                       k->location, STATUS_CAT[c->status]);
                found = 1;
            }
        }
    }
    if (found != 1)
    {
        printf("No cat with breed %s\n", breedName);
    }
}
        else if(queryType == 2)
        {
            // This query updates the status of a specific cat given its name. If the new status of
            //the cat is adopted, the cat needs to be entirely removed from the system, and the cat array must be
            //adjusted accordingly.
            int newStatus;
            scanf("%d", &newStatus);
            char catName[100];
            scanf("%s", catName);
            Cat *cat = getCatByName(s, catName);
            if(cat != NULL)
            {
                printf("%s is now %s!\n", (*cat).name, STATUS_CAT[newStatus]);
                if(newStatus == 0) // ADOPTED
                {   
                    Kennel *k = getKennelByCat(s, cat);
                    removeCatFromKennel(k, cat);
                    free((*cat).name);
                    free(cat);
                }
                else
                {
                    (*cat).status = newStatus;
                }
            }
        }
        else if(queryType == 3)
        {
            // This query attempts to move a cat from one kennel to another, if the transfer is valid
            //based on capacity constraints, the cat is removed from the source kennel, the cats array in the
            //kennel is resized to one less cat, the destination kennel’s cats array is resized to one more, then the
            //moved cat is added to the destination kennel’s cats array. Occupancies are adjusted accordingly.
            //Note: a transfer is only valid if it doesn’t break any of the maximum capacity and the breed specific
            //capacity constraints.
            char catName[100];
            char destLocation[100];
            scanf("%s %s", catName, destLocation);
            Cat *cat = getCatByName(s, catName);
            if(cat != NULL)
            {
                int canMove = canMoveTo(s, destLocation, (*cat).breed, dictionary, breedCount);
                if(canMove)
                {
                    Kennel *sourceKennel = getKennelByCat(s, cat);
                    removeCatFromKennel(sourceKennel, cat);
                    // Add cat to destination kennel
                    for(int i = 0; i < (*s).numKennels; i++)
                    {
                        Kennel *k = &(*s).kennels[i];
                        if(strcmp((*k).location, destLocation) == 0)
                        {
                            // Resize cats array in destination kennel
                            k->cats = (Cat**)realloc(k->cats, (k->occupancy + 1) * sizeof(Cat*));
                            k->cats[k->occupancy] = cat;
                            k->occupancy++;
                            printf("%s moved successfully to %s\n", (*cat).name, destLocation);
                            break;
                        }
                    }
                }
                else
                {
                    printf("%s cannot take a %s cat!\n", destLocation, (*cat).breed);
                }
            }
        }
    }
}

void freeBreeds(char **dictionary, int breedCount)
{
    for(int i = 0; i < breedCount; i++)
    {
        free(dictionary[i]);
    }
    free(dictionary);
}

void freeStore(int count, CatStore *store)
{
    for(int i = 0; i < count; i++)
    {
        // Free each kennel
        Kennel *k = &(*store).kennels[i];
        for(int j = 0; j < (*k).occupancy; j++)
        {
            // Free each cat
            Cat *c = (*k).cats[j];
            free((*c).name);
            free(c);
        }
        free((*k).cats);
        free((*k).location);
    }
    free((*store).kennels);

    // Free capacities
    for(int i = 0; i < count; i++)
    {
        free((*store).capacities[i]);
    }
    free((*store).capacities);

    free(store);
}
