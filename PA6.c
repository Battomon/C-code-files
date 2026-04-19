/* 
COP 3502C PA6
This program is written by: Elijah Trees 
*/

#include <stdio.h>
#include <stdlib.h>

/* ---------- Global Modes ---------- */
//If you don’t know enumeration in C, I have added an example to the end of this
//document for typedef enum
typedef enum {
MODE_ADOPTION = 0, /* Max-heap by adoption_key (higher is better) */
MODE_TRIAGE = 1 /* Min-heap by triage_key (lower is more urgent)*/
} Mode;
/* ---------- Core Cat Record ---------- */
typedef struct Cat {
char *name; /* dynamically allocated, unique string (<= 25 chars +
'\0') */
char *breed; /* dynamically allocated string */
int age; /* years */
int friendliness; /* 0..100 */
int health; /* 0..100 (higher means healthier) */
int arrival_id; /* strictly increasing when ADDed */
int quarantine; /* 0 or 1 (1 => cannot be adopted) */
double key; /* cached priority value for the **active** mode */
} Cat;
/* ---------- Array-Based Binary Heap of Cat* ---------- */
typedef struct {
Cat **arr; /* array of Cat* implementing the heap */
int size; /* current number of elements */
int capacity; /* allocated capacity */
Mode mode; /* controls comparator semantics (minheap/maxheap)*/
} CatHeap;
/* ---------- Global Shelter State ---------- */
typedef struct {
Mode mode; /* MODE_ADOPTION or MODE_TRIAGE */
char *featured_breed; /* NULL => none; else dynamically allocated breed string
*/
double alpha; /* multiplier for featured breed (>= 1.0; default 1.0)*/
int next_arrival_id; /* increments on each ADD */
CatHeap heap; /* priority structure */
} Shelter;
/* Returns heap index of cat with given name, or -1 if not found */
int find_cat_index(const CatHeap *heap, const char *name);
/* Returns the current adoption-mode key for cat c using S->featured_breed and S-
>alpha. */
double compute_adoption_key(const Cat *c, const Shelter *S);
/* Returns the current triage-mode key for cat c. */
double compute_triage_key(const Cat *c);
/* Recomputes all keys for the active mode and rebuilds heap in O(n) using bottom-
up heapify. */
void recompute_all_keys_and_build(Shelter *S);
/* ========== Command Handlers (I/O-Free Logic) ========== */
/* Allocates a new Cat, initializes fields, computes key for active mode,
ensures no duplicate name exists (linear scan), and inserts into the heap. */
void cmd_add (Shelter *S, const char *name, const char *breed, int age, int
friendl, int health);
/* Locate the cat by name using a linear scan of the heap array.
If found, update the requested field.
For AGE/FRIEND/HEALTH: recompute key for active mode and restore heap order.
For QUARANTINE: update the flag only (numeric key unchanged), then restore heap
order. */
void cmd_update(Shelter *S, const char *name, const char *field, int new_value);
/* Locate the cat by name using a linear scan of the heap array.
Remove it from the heap, restore heap order, and free the Cat. */
void cmd_remove(Shelter *S, const char *name);
/* Prints the current top cat for the active mode (does not modify heap). */
void cmd_peek (const Shelter *S);
/* Serves the highest-priority cat based on the active mode.
ADOPTION: adopt highest-priority non-quarantined cat.
TRIAGE: treat most urgent cat. */
void cmd_serve(Shelter *S);
/* Sets S->mode, sets heap mode, then recompute_all_keys_and_build(S). */
void cmd_mode (Shelter *S, const char *mode_str);
/* Sets (or clears) featured breed and alpha, then
recompute_all_keys_and_build(S). */
void cmd_featured(Shelter *S, const char *breed, double alpha);
/* Non-destructive: print top k according to active mode.
Recommended: copy heap array into a temp heap and extract k from the copy. */
void cmd_print (const Shelter *S, int k);






typedef enum 
{
    MODE_ADOPTION = 0, /* Max-heap by adoption_key (higher is better) */
    MODE_TRIAGE = 1 /* Min-heap by triage_key (lower is more urgent)*/
} Mode;
// A function that uses the 'Mode' type as a parameter
void set_heap_strategy(Mode current_mode) 
{
    printf("Initializing system... ");
    if (current_mode == MODE_ADOPTION) 
        printf("Setting up MAX-HEAP for Adoption Mode.\n");
    
    else if (current_mode == MODE_TRIAGE)
        printf("Setting up MIN-HEAP for Triage Mode.\n");
}
int main() 
{
    // 1. Declare and initialize using your alias 'Mode'
    Mode activeMode = MODE_TRIAGE;

    // 2. Pass it to a function
    set_heap_strategy(activeMode);

    // 3. Use it in a switch statement (common for enums)
    switch (activeMode) 
    {
        case MODE_ADOPTION:
            printf("Priority: Higher keys first.\n");
            break;
        case MODE_TRIAGE:
            printf("Priority: Lower keys (urgent) first.\n");
            break;
    }
    return 0;
}