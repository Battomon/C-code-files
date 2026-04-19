/* COP 3502C PA5
This program is written by: Elijah Trees */
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define MAX_NAME 25 // max name of a cat
#define NUM_TRAITS 5 // number of total traits a cat has
const char *TRAIT_NAMES[NUM_TRAITS] = {"friendly", "grumpy", "playful", "lazy", "curious"};


typedef struct Cat {
char *name; //dynamically allocate space for a cat’s name
char *breed; //dynamically allocate space for a cat’s breed
int charm; //unique charm score for each cat
int traits[NUM_TRAITS]; //scores corresponding to each index in TRAIT_NAMES
} Cat;

typedef struct BST_Node {
Cat *cat;
struct BST_Node *left;
struct BST_Node *right;
int subtree_size; // stores the current size of this subtree’s root (not height)
} BST_Node;



/*
• Efficiently insert and remove cats from the leaderboard
• Maintain subtree metadata
• Look up any cat’s standing or find a current rank
• Analyze breed diversity across score brackets
• Mass disqualifies cats based on trait conditions
*/

/*
• The keys on the tree are the cat names
• uses strcmp to navigate the tree
• subtree_size is the total number of nodes below it in the tree (not height)
• Note: The tree will not be balanced. Its shape depends entirely on the insertion order.
• when deleting the node who's a parent of two children, 
  utilize selecting the successor (minimum of the right) node as necessary.
*/

/*
                        QUERIES:
q = 1: insert a cat
q = 2: delete a cat
q = 3: kth smallest element
q = 4: find all cats of a criteria
q = 5: remove all cats of a criteria
q = 6: print whole tree "in-order" showing subroot sizes for each node in the tree
*/

/*
                        INPUT & OUTPUT
first input is a number q, 1-6 that represent different actions
Input 1:
<name> <breed> <charm> <t0> <t1> <t2> <t3> <t4>
Output:
"Insert: <depth>"

Input 2:
<name>
Output:
"Deletion Complete"

Input 3:
<k>
Output:
"<Kth_cat_name>"

Input 4:
<traitIndex> <traitValue>
Output:
"<trait>: <name1> <name2>..."

Input 5:
<traitIndex> <traitValue>
Output:
"<num_removed_cats>"

Input 6:
<>
Output:
"<name> <charm_score> <subtree_size>"
"<name> <charm_score> <subtree_size>"
"<name> <charm_score> <subtree_size>"
...
*/

/*
                        IMPORTAINT NOTES
• if the name already exists in the tree, only replace the cat there if the new cat has more traits set to 1
• you must only delete a node when q 2 or 5 is called
• deletion code must be3 cases approach
• Make sure to use good variable names, put comments on important blocks of code
• Use DMA for storing strings in the structs, allocating exact memory needed for the string size.
*/

BST_Node *newCatNode(BST_Node *root)
{
    // read in cat info
    char name[MAX_NAME];
    char breed[MAX_NAME];
    int charm;
    scanf(" %s %s %d", name, breed, &charm);

    int traits[NUM_TRAITS];
    for (int i = 0; i < NUM_TRAITS; i++) 
        scanf(" %d", &traits[i]);

    // create new cat node
    Cat* newCat = malloc(sizeof(Cat));
    newCat->name = malloc(strlen(name) + 1);
    strcpy(newCat->name, name);
    newCat->breed = malloc(strlen(breed) + 1);
    strcpy(newCat->breed, breed);
    newCat->charm = charm;
    for (int i = 0; i < NUM_TRAITS; i++) 
        newCat->traits[i] = traits[i];

    // create a new BST_Node and insert it into the tree
    BST_Node *newNode = (BST_Node *)malloc(sizeof(BST_Node));
    newNode->cat = newCat;
    newNode->left = NULL;
    newNode->right = NULL;
    newNode->subtree_size = 1;

    return newNode;
}

int depth(BST_Node *root, char *name)
{
    int depth = 0;
    BST_Node *current = root;
    while (current != NULL) {
        int cmp = strcmp(current->cat->name, name);
        if (cmp == 0)
            break; // Found the new node, stop traversing
        else if (cmp > 0) // new node is smaller, go left
            current = current->left;
        else // new node is larger, go right
            current = current->right;
        depth++; // after going left or right, depth++
    }
    return depth;
}

BST_Node* insertion(BST_Node *root, BST_Node* newNode, int *status)
{
    if (root == NULL || root->cat == NULL) // empty tree
        return newNode;

    int cmp = strcmp(root->cat->name,newNode->cat->name);

    if(cmp > 0) //check left side
    {
        root->left = insertion(root->left, newNode, status);
        if(*status == 0)
            root->subtree_size++;
    }
    else if(cmp < 0) //check right side
    {
        root->right = insertion(root->right, newNode, status);
        if(*status == 0)
            root->subtree_size++;
    }
    else
    {
        //must be identical names, check traits and replace if new cat has more traits than old cat
        int New = 0;
        int Old = 0;
        for(int i =0; i<NUM_TRAITS; i++)
        {
            if(newNode->cat->traits[i])
                New++;
            if(root->cat->traits[i])
                Old++;
        }
        //if new cat has more traits than old cat, replace it and free the old memory
        if(New > Old)
        {
            Cat* temp = root->cat;
            root->cat = newNode->cat;
            free(temp->breed);
            free(temp->name);
            free(temp);
            free(newNode);
            *status = 1;
        }
        else
        {
            // cat being tossed, free its memory
            free(newNode->cat->breed);
            free(newNode->cat->name);
            free(newNode->cat);
            free(newNode);
            *status = 2; // no new node was added
        }
    }

    return root;
}

BST_Node* deletion(BST_Node *root, char *name)
{
    if (root == NULL) // empty tree
        return NULL;

    int cmp = strcmp(root->cat->name, name);
    if (cmp > 0) // go left
        root->left = deletion(root->left, name);
    else if (cmp < 0) // go right
        root->right = deletion(root->right, name);
    else // found the node to delete
    {
        // case 1: no children (leaf node)
        if (root->left == NULL && root->right == NULL) 
        {
            free(root->cat->breed);
            free(root->cat->name);
            free(root->cat);
            free(root);
            return NULL;
        }
        // case 2: one child
        else if (root->left == NULL && root->right != NULL) // only right child
        {
            BST_Node *temp = root->right; //save child node
            free(root->cat->breed);
            free(root->cat->name);
            free(root->cat);
            free(root);
            return temp; // recursivly return child to grandparent after deletion
        }
        else if (root->right == NULL && root->left != NULL) // only left child
        {
            BST_Node *temp = root->left; //save child node
            free(root->cat->breed);
            free(root->cat->name);
            free(root->cat);
            free(root);
            return temp; // recursivly return child to grandparent after deletion
        }
        // case 3: two children
        else 
        {
            // find replacement node
            BST_Node *repParent = root;
            BST_Node *repNode = root->right;
            while (repNode->left != NULL)
            {
                repNode->subtree_size--;
                repParent = repNode;
                repNode = repNode->left;
            }
    
            // free the old cat memory
            free(root->cat->breed);
            free(root->cat->name);
            free(root->cat);

            // replace the cat data with the replacement node's cat data
            root->cat = repNode->cat;

            // delete the replacement node
            // check if replacement node had a right child
            if (repParent != root)
                repParent->left = repNode->right; // if repNode had a right child, connect it to repParent in it's old position
            else
                repParent->right = repNode->right; //if root's right child was the leftmost, connect root to repNode's right child
            free(repNode); // free the replacement node
        }
    }

    // after deletion, update subtree sizes on the way back up the tree
    root->subtree_size = 1; // reset subtree size to 1 for current node
    if (root->left != NULL)
        root->subtree_size += root->left->subtree_size; // add left subtree size if it exists
    if (root->right != NULL)
        root->subtree_size += root->right->subtree_size; // add right subtree size if it exists

    return root;
}

BST_Node* kthSmallest(BST_Node *root, int k)
{
    if (root == NULL || k > root->subtree_size || k <= 0) // empty tree or invalid k
        return NULL;

    // find the number of nodes in the left side
    int leftsize = 0;
    if(root->left != NULL)
        leftsize = root->left->subtree_size;

    if(leftsize > k-1) 
        return kthSmallest(root->left, k);  
    else if( leftsize == k-1 ) 
        return root;
    else
        return kthSmallest(root->right, k - leftsize - 1); 

}

void filterHelper(BST_Node *root, int traitIndex, int traitValue, char **catArray, int *resultSize)
{
    if (root == NULL) // base case
        return;

    filterHelper(root->left, traitIndex, traitValue, catArray, resultSize); // go left
    
    if (root->cat->traits[traitIndex] == traitValue) // recursive root
    {
        catArray[*resultSize] = malloc(strlen(root->cat->name) + 1);
        strcpy(catArray[*resultSize], root->cat->name);
                (*resultSize)++; 
    }

    filterHelper(root->right, traitIndex, traitValue, catArray, resultSize); // go right
}

char** filterByTrait(BST_Node *root, int traitIndex, int traitValue, int *resultSize)
{
    if (root == NULL) // empty tree
        return NULL;

    char **catArray = malloc(root->subtree_size * sizeof(char*)); // max size
    filterHelper(root, traitIndex, traitValue, catArray, resultSize); // fill
    catArray = realloc(catArray, (*resultSize) * sizeof(char*)); // trim excess
    return catArray;
}

BST_Node* traitDelete(BST_Node *root, int traitIndex, int traitValue)
{
    if(root == NULL)
    {
        printf("NONE REMOVED\n");
        return NULL;
    }
    
    int removedCount = 0;
    char **catsToRemove = filterByTrait(root, traitIndex, traitValue, &removedCount); // get cats to remove

    if (removedCount == 0)
        printf("NONE REMOVED\n");
    else
    {
        for(int i = 0; i < removedCount; i++)
            root = deletion(root, catsToRemove[i]); // delete each cat
        printf("%d\n", removedCount);
    }
    for(int i = 0; i < removedCount; i++)
        free(catsToRemove[i]); // free each cat name
    free(catsToRemove); // free the array of cat names
    return root;
}

void printTree(BST_Node *root)
{
    //in-order traversal
    // Only traverse the node if it's not null.
    if(root != NULL) 
    {
        printTree(root->left); // Go Left.
        printf("%s %d %d\n", root->cat->name, root->cat->charm, root->subtree_size); // Print the root.
        printTree(root->right); // Go Right.
    }
}

int main(void)
{
    BST_Node *root = NULL; // initialize the root of the BST
    //get numInputs

    int numInputs; scanf("%d", &numInputs);
    for(int i = 0; i < numInputs; i++) // repeat for total number of requested inputs
    {
        int q; scanf(" %d", &q);
        switch(q) 
        {
            case 1: // INSERTION
            {
                BST_Node *newNode = newCatNode(root);

                int status = 0;
                root = insertion(root, newNode, &status);

                if(status == 1) // replaced nodes get a seperate print statement
                    printf("Replaced\n");
                else if(status == 0)
                {
                    int d = depth(root, newNode->cat->name);
                    printf("Insert: %d\n", d);
                }
                break;
            }

            case 2: // DELETION
            {

                char name[MAX_NAME]; scanf(" %s", name);
                root = deletion(root, name);
                printf("Deletion Complete\n");
                break;
            }

            case 3: // KTH SMALLEST
            {
                int k; scanf(" %d", &k);
                BST_Node *kthNode = kthSmallest(root, k);
                if (kthNode != NULL)
                    printf("%s %s %d\n", kthNode->cat->name, kthNode->cat->breed, kthNode->cat->charm);
                else
                    printf("NO SMALLEST ELEMENT FOUND\n");

                break;
            }

            case 4: // FILTER BY TRAIT
            {
                int traitIndex, traitValue; scanf(" %d %d", &traitIndex, &traitValue);
                int *resultSize = malloc(sizeof(int));
                *resultSize = 0;

                char **filteredCats = filterByTrait(root, traitIndex, traitValue, resultSize);
                if(*resultSize == 0)
                    printf("NONE FOUND\n");
                else
                {
                    // print the filtered cat names
                    printf("%s:", TRAIT_NAMES[traitIndex]);
                    for (int i = 0; i < *resultSize; i++)
                        printf(" %s", filteredCats[i]);
                    printf("\n");
                }
                // free the memory allocated for the filtered cat names
                for (int i = 0; i < *resultSize; i++)
                    free(filteredCats[i]);
                free(filteredCats);
                free(resultSize);
                break;
            }

            case 5: // DELETE BY TRAIT
            {
                int Index, Value; scanf(" %d %d", &Index, &Value);
                root = traitDelete(root, Index, Value);
                break;
            }

            case 6: // PRINT TREE
            {
                if(root == NULL)
                    printf("EMPTY\n");
                else
                    printTree(root);
                break;
            }

            default:
            {
                printf("\nInvalid input\n");
                main();
                break;
            }
        }
    }

}







