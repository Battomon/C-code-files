#include <string.h>
#include <stdio.h>
#include <stdlib.h>

typedef struct StringType {
    char *string;
    int length;
} StringType;

typedef struct Employee {
    StringType *ename;
    double salary;
} Employee;


Employee* createEmployees(char names[][50], double *salaries, int empCount) 
{
    Employee *arrayEmps = malloc(empCount * sizeof(Employee));

    for(int i = 0; i < empCount; i++)
    {
        arrayEmps[i].ename = malloc(sizeof(StringType));
        arrayEmps[i].ename->string = malloc((strlen(names[i])+1) * sizeof(char));
        strcpy(arrayEmps[i].ename->string, names[i]);
        arrayEmps->ename->length = strlen(names[i]);
        arrayEmps[i].salary = salaries[i];
    }

    return arrayEmps;
}



int main() {
    // array of employees' names
    char nameList[][50] = {"Adam", "Josh", "Kyle", "Ali", "Mohammed"};
    
    // array of salaries
    double salaries[5] = {15.80, 13.5, 20.9, 12.99, 10.5};

    int empCount = 5;
    Employee *employees = createEmployees(nameList, salaries, empCount);

    return 0;
}