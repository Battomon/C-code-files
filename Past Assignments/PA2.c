// COP 3502C PA2 This program is written by: Elijah Trees

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_SCORES 5 // size of a cat scores array representing 5 traits
#define MAX_STR 25 // maximum name length of a cat

typedef struct Cat
{
char *name;             // dynamically allocated cat name
char *breed;            // dynamically allocated breed
int scores[MAX_SCORES]; // array of size 5 representing trait scores
int baseScore;          // sum of scores array
} Cat;

typedef struct Rivals
{
Cat* cat1; // pointer to cat1
Cat* cat2; // pointer to cat2
} Rivals;

const float POSITION_BONUS[10] = {3.0, 5.0, 4.0, 6.0, 7.0, 2.0, 8.0, 1.0, 9.0, 1.5}; // bonus percentages that apply to each positionCat *cats;

// manditory global variables
Cat *cats;
Rivals *rivals;
float bestPermScore;
Cat **tracker;

// manditory recursive function prototypes
int countHighPerformersTraits(Cat *Cats, int numCats, int numScores); //to count high performer bonus traits in a team
int synergyBonusApplies(Cat* cats, int numCats, int scoreIndex); // recursive: checks all cats' scores
int rivalPenaltyApplies(Rivals** arrayOfRivalPairs, Cat* Cats, int numCats, int numRivalPairs); // recursive: checks rival adjacency

// needed to calculate rival penalties
int rivalPenaltyApplies_helper(Rivals* r, Cat* Cats, int numCats, int idx, int prevFound);


float teamScore(Cat* Cats, Rivals** arrayOfRivals, int numCats, int numRivalPairs)
{
    float totalPoints = 0.00;
    
    //base score
    for(int i = 0; i < numCats; i++)
        totalPoints += Cats[i].baseScore;
    totalPoints = totalPoints/numCats;

    int diversity = 1; // boolean to track if breed diversity bonus applies

    for(int i = 0; i < numCats; i++)
    {    
        // position bonus
        totalPoints += Cats[i].baseScore * POSITION_BONUS[i]/100;
        
    // check for breed diversity bonus
    for(int j = 0; j < numCats; j++)
        if(i != j && strcmp(Cats[i].breed, Cats[j].breed) == 0)
        {
            diversity = -1;
            break;
        }
    }
    // if breed diversity true, add bonus
    if(diversity == 1)
    {
        totalPoints += 10;
    }
        
     // if breed diversity false, breed duplicate must be true
    else if(diversity == -1)
        totalPoints -= 15;

    // high performer bonus
    totalPoints += countHighPerformersTraits(Cats, numCats, MAX_SCORES);
    
    // synergy bonus
    totalPoints += synergyBonusApplies(Cats, numCats, MAX_SCORES - 1);
    
    // rivals penalty
    totalPoints -= rivalPenaltyApplies(arrayOfRivals, Cats, numCats, numRivalPairs);


    return totalPoints;
}

int countHighPerformersTraits(Cat *Cats, int numCats, int numScores) 
{
    if (numCats <= 0) 
        return 0;

    int currentCatBonus = 0;
    for (int i = 0; i < numScores; i++) {
        if (Cats[numCats-1].scores[i] >= 90)
            currentCatBonus += 5;
    }
    return currentCatBonus + countHighPerformersTraits(Cats, numCats - 1, numScores);
}

int synergyBonusApplies(Cat* cats, int numCats, int scoreIndex)
{
    // if all cats have been checked, and all scores above 85, apply bonus
    if (numCats == 0)
        return 30;

    // if all scores for current cat are checked, and none above 85, fail bonus.
    if (scoreIndex < 0)
        return 0;

    // if current score is above 85, check next cat
    if (cats->scores[scoreIndex] >= 85)
        return synergyBonusApplies(cats + 1, numCats - 1, MAX_SCORES - 1);;

    // Recurse to next score of current cat
    return synergyBonusApplies(cats, numCats, scoreIndex - 1);
}

// helper: recursively scan cats for a single rival pair
// helper: recursively scan cats for a single rival pair
int rivalPenaltyApplies_helper(Rivals* r, Cat* Cats, int numCats, int idx, int prevFound)
{
    if (r == NULL)
        return 0;
    if (idx >= numCats)
        return 0;

    int isMatch = (strcmp(Cats[idx].name, r->cat1->name) == 0) ||
                  (strcmp(Cats[idx].name, r->cat2->name) == 0);

    if (isMatch != 0)
    {
        // FIX: Added (idx > 0) to ensure we don't compare index 0 to prevFound's -1
        if (idx > 0 && prevFound == idx - 1)
            return 1; // penalty applies if rivals are adjacent
        
        prevFound = idx;
    }

    return rivalPenaltyApplies_helper(r, Cats, numCats, idx + 1, prevFound);
}

// recursive over rival pairs; for each rival pair call the helper to scan cats 
int rivalPenaltyApplies(Rivals** arrayOfRivals, Cat* Cats, int numCats, int numRivalPairs)
{
    if (numRivalPairs <= 0)
        return 0;

    Rivals* r = arrayOfRivals[numRivalPairs - 1];
    if (rivalPenaltyApplies_helper(r, Cats, numCats, 0, -1) == 1)
        return 25 + rivalPenaltyApplies(arrayOfRivals, Cats, numCats, numRivalPairs - 1); // number of points this penalty costs + the result of recursing on the remaining rival pairs

    // recurse on remaining rival pairs
    else
        return rivalPenaltyApplies(arrayOfRivals, Cats, numCats, numRivalPairs - 1);
}

Cat *createCat()
{
    // creating the space to store cat info
    Cat* newCat = malloc(sizeof(Cat));

    // capturing name
    char name[MAX_STR];
    scanf("%s", name);
    newCat->name = malloc((strlen(name) + 1) * sizeof(char));
    strcpy(newCat->name, name);


    // capturing breed
    char breed[MAX_STR];
    scanf("%s", breed);
    newCat->breed = malloc((strlen(breed) + 1) * sizeof(char));
    strcpy(newCat->breed, breed);


    // capturing scores
    for (int i = 0; i < MAX_SCORES; i++)
    {
        int score;
        scanf("%d", &score);
        newCat->scores[i] = score;
    }

    // base score
    int sum = 0;
    for(int i = 0; i < MAX_SCORES; i++)
        sum += newCat->scores[i];
    newCat->baseScore = sum;

    return newCat;
}

Cat **createCatsArray(int numTeams, int numCats)
{
    Cat **catArray = malloc(numTeams * sizeof(Cat*));
    for(int i = 0; i < numTeams; i++)
        catArray[i] = malloc(numCats * sizeof(Cat));

    for(int i = 0; i < numTeams; i++)
        for(int j = 0; j< numCats; j++)
        {
            Cat *tempCat = createCat(); 
            catArray[i][j] = *tempCat;
            free(tempCat); 
        }
    return catArray;
}

Rivals *createNewRivalPair(Cat** arrayOfCats, int numTeams, int numCats)
{
    Rivals *newRivals = malloc(sizeof(Rivals));
    char n1[MAX_STR], n2[MAX_STR];
    scanf("%s %s", n1, n2);

    // set to null incase no rivals found
    newRivals->cat1 = NULL;
    newRivals->cat2 = NULL;

    // generate rival pairs
    for(int i = 0; i < numTeams; i++)
        for(int j = 0; j < numCats; j++)
        {
            if(strcmp(n1, arrayOfCats[i][j].name) == 0)
                newRivals->cat1 = &arrayOfCats[i][j];
            if(strcmp(n2, arrayOfCats[i][j].name) == 0)
                newRivals->cat2 = &arrayOfCats[i][j];
        }
   

    return newRivals;
}

Rivals **arrayOfRivalPairs(Cat** arrayOfCats, int numRivals, int numTeams, int numCats)
{
    Rivals **arrayOfRivals = malloc(numRivals * sizeof(Rivals*));
    for(int i = 0; i < numRivals; i++)
        arrayOfRivals[i] = createNewRivalPair(arrayOfCats, numTeams, numCats);
    return arrayOfRivals;
}

void findBestCatOrder(Cat** arrayOfCats, Cat* cats, Cat* perm, Cat** bestArr, Cat** tracker, int* used, int furthest, int numTeams, int numCats, Rivals** arrayOfRivalPairs, int numRivals)
{
    // base case
    int allCats = numTeams*numCats;
    float currentScore = 0;
    if (furthest == allCats) 
    {
        // check if this permutation has the highest score so far
        int count = 0;
        for(int i = 0; i < numTeams; i++)
        {
            // converting the 1d permutation to a 2d array
            for(int j = 0; j < numCats; j++)
                tracker[i][j] = perm[count++];

            // add the score of each team
            currentScore += teamScore(tracker[i], arrayOfRivalPairs, numCats, numRivals);
        }

        if(currentScore > bestPermScore)
        {
            bestPermScore = currentScore;
            for(int i = 0; i < numTeams; i++)
                for(int j = 0; j < numCats; j++)
                    bestArr[i][j] = tracker[i][j];
        }
        return;
    }

    // recusrsive step. Try placing each cat in the current 'furthest' position
    for (int i = 0; i < allCats; i++)
    {
        if (!used[i]) {
            used[i] = 1; // Mark cat as "placed"
            perm[furthest] = cats[i]; // Add to current test order
            
            // Recurse to fill the next position
            findBestCatOrder(arrayOfCats, cats, perm, bestArr, tracker, used, furthest + 1, numTeams, numCats, arrayOfRivalPairs, numRivals);
            
            used[i] = 0; // reset for next permutation
        }
    }
}

int main()
{
    // create teams of cats
    int numTeams, numCats;
    scanf("%d %d", &numTeams, &numCats);
    int allCats = numCats * numTeams;


    // 2d array of cats in their teams
    Cat** arrayOfCats = createCatsArray(numTeams, numCats);

    // 1d array of all cats
    cats = malloc(allCats * sizeof(Cat));
    int count = 0;
        for(int i = 0; i < numTeams; i++)
            for(int j = 0; j < numCats; j++)
                cats[count++] = arrayOfCats[i][j];

    // create rival pairs
    int numRivalPairs = 0;
    scanf("%d", &numRivalPairs);
    Rivals **arrayOfRivals = arrayOfRivalPairs(arrayOfCats, numRivalPairs, numTeams, numCats);

    // Create the 'used' array (initialized to 0)
    int* used = calloc(allCats, sizeof(int));
    
    // Create the inital permutation being tested
    Cat* perm = malloc(allCats * sizeof(Cat));

    // Create space to test the score and keep the best arrangement
    tracker = malloc(numTeams * sizeof(Cat*));
    Cat **bestArr = malloc(numTeams * sizeof(Cat*));
    for(int i = 0; i < numTeams; i++)
    {
        tracker[i] = malloc(numCats * sizeof(Cat));
        bestArr[i] = malloc(numCats * sizeof(Cat));
    }

    // reset the global best score
    bestPermScore = -1;

    // index of furthest permutation within the array
    int furthest = 0;
    findBestCatOrder(arrayOfCats, cats, perm, bestArr, tracker, used, furthest, numTeams, numCats, arrayOfRivals, numRivalPairs);

    // FINAL PRINTOUT
    printf("Best Teams Grouping score: %.2f\n", bestPermScore);
    for(int i = 0; i < numTeams; i++)
    {
        printf("Team %d: ", (i + 1));
        for(int j = 0; j < numCats; j++)
            printf("%s ", bestArr[i][j].name);
        printf("%.2f\n", teamScore(bestArr[i], arrayOfRivals, numCats, numRivalPairs));
    }
    
    // Best Canidate
    printf("Best Candidate: ");
    int bestCanidateScore = 0;
    Cat *bestCanidate = malloc(numCats * sizeof(Cat));
    for(int i = 0; i < numTeams; i++)
        if(teamScore(bestArr[i], arrayOfRivals, numCats, numRivalPairs) > bestCanidateScore)
        {
            bestCanidateScore = teamScore(bestArr[i], arrayOfRivals, numCats, numRivalPairs);
            for(int j = 0; j < numCats; j++)
                bestCanidate[j] = bestArr[i][j];
        }
    for(int i = 0; i < numCats; i++)
            printf("%s ", bestCanidate[i].name);



    //FREE MEMORY
    
    // Free the solver arrays
    free(used);
    free(perm);
    
    // Free the 2D arrays (tracker and bestArr)
    for(int i = 0; i < numTeams; i++) 
    {
        free(tracker[i]);
        free(bestArr[i]);
    }
    free(tracker);
    free(bestArr);

    // Free the cat arrays   
    free(cats); 
    free(bestCanidate);

    // free arrayOfCats
    for(int i = 0; i < numTeams; i++) 
    {
        for(int j = 0; j < numCats; j++) 
        {
            free(arrayOfCats[i][j].name);
            free(arrayOfCats[i][j].breed);
        }
        free(arrayOfCats[i]);
    }
    free(arrayOfCats);

    // Free rivals
    for(int i = 0; i < numRivalPairs; i++)
        free(arrayOfRivals[i]);
    free(arrayOfRivals);

    return 0;
}


