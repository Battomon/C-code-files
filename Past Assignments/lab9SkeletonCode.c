/* Lab 9 Skeleton Code
 *
 * Created by Adam Betinsky on 3/6/2025 to help save time in the lab
 *
 * Cleaned up by Adam Betinsky on 10/23/2025
 * 
 * Insertion sort updated on 3/24/2026
 *
 * NOTE: This program will be slow for some of the algorithms! It's normal for nothing to be outputted for minutes at a time when you hit run.
 */

#include <stdio.h>

#include <stdlib.h>

#include <time.h>

#define MAXARRAYS 6

#define MAXVALUE 1000000

void randArray(int arr[], int size, int maxValue);

void bubbleSort(int arr[], int n);

void insertionSort(int arr[], int l, int r);

void selectionSort(int arr[], int n);

void merge(int arr[], int l, int m, int r);

void mergeSort(int arr[], int l, int r);

void arrayCopy(int original[], int copy[], int size);

long timediff(clock_t t1, clock_t t2);

void swap(int *xp, int *yp);

int partition(int *vals, int low, int high);

void quickSort(int *numbers, int low, int high);

void mergeInsertionSort(int arr[], int l, int r);

int main()
{
    int sizes[MAXARRAYS] = {10000, 20000, 30000, 40000, 50000, 100000};

    int *origionalArray, *sortedArray;

    clock_t start, end;

    long elapsed;

    for(int i = 0; i < MAXARRAYS; i++)
    {
        printf("\nSize: %d\n", sizes[i]);
        origionalArray = malloc(sizeof(int) * sizes[i]);
        sortedArray = malloc(sizeof(int) * sizes[i]);

        randArray(origionalArray, sizes[i], MAXVALUE);
        arrayCopy(origionalArray, sortedArray, sizes[i]);

        start = clock();
        bubbleSort(sortedArray, sizes[i]);
        end = clock();
        elapsed = timediff(start, end);
        printf("\nSorting %d takes %ld ms for Bubble Sort\n", sizes[i], elapsed);

        arrayCopy(origionalArray, sortedArray, sizes[i]);
        start = clock();
        insertionSort(sortedArray, 0, sizes[i] - 1);
        end = clock();
        elapsed = timediff(start, end);
        printf("\nSorting %d takes %ld ms for Insertion Sort\n", sizes[i], elapsed);

        arrayCopy(origionalArray, sortedArray, sizes[i]);
        start = clock();
        selectionSort(sortedArray, sizes[i]);
        end = clock();
        elapsed = timediff(start, end);
        printf("\nSorting %d takes %ld ms for Selection Sort\n", sizes[i], elapsed);

        arrayCopy(origionalArray, sortedArray, sizes[i]);
        start = clock();
        mergeSort(sortedArray, 0, sizes[i] - 1);
        end = clock();
        elapsed = timediff(start, end);
        printf("\nSorting %d takes %ld ms for Merge Sort\n", sizes[i], elapsed);

        arrayCopy(origionalArray, sortedArray, sizes[i]);
        start = clock();
        quickSort(sortedArray, 0, sizes[i] - 1);
        end = clock();
        elapsed = timediff(start, end);
        printf("\nSorting %d takes %ld ms for Quick Sort\n", sizes[i], elapsed);

        arrayCopy(origionalArray, sortedArray, sizes[i]);
        start = clock();
        mergeInsertionSort(sortedArray, 0, sizes[i] - 1);
        end = clock();
        elapsed = timediff(start, end);
        printf("\nSorting %d takes %ld ms for Merge Insertion Sort\n", sizes[i], elapsed);

        free(origionalArray);
        free(sortedArray);
    }

    return 0;
}

long timediff(clock_t t1, clock_t t2)
{
    //calculate the elapsed time of the passed clock_t variables using the formula below and return the value
    long elapsed = ((double) t2-t1) / CLOCKS_PER_SEC * 1000;

    return elapsed;
}

void swap(int *xp, int *yp)
{
    //swap the passed integers using pass by reference
    int temp = *xp;
    *xp = *yp;
    *yp = temp;
}

void bubbleSort(int arr[], int n)
{
    int i, j,temp;

    for (i = 0; i < n - 1; i++)
    {
        for (j = 0; j < n - i - 1; j++)
        {
            if (arr[j] > arr[j + 1])
            {
                swap(&arr[j], &arr[j+1]);
            }
        }
    }
}

void insertionSort(int arr[], int l, int r)
{
    int i, item, j;

    for (i = l + 1; i <= r; i++)
    {
        item = arr[i];

        for(j= i - 1; j >= l; j--)
        {
            if(arr[j]>item)
                arr[j + 1] = arr[j];
            else
                break;
        }
        arr[j + 1] = item;
    }
}

void mergeInsertionSort(int arr[], int l, int r)
{
    if (r - l <= 10)
    {
        insertionSort(arr, l, r);
    }
    else
    {
        int m = l + (r - l) / 2;
        mergeInsertionSort(arr, l, m);
        mergeInsertionSort(arr, m + 1, r);
        merge(arr, l, m, r);
    }

}

void arrayCopy(int original[], int copy[], int size)
{
    for(int i = 0; i < size; i++)
        copy[i] = original[i];
}

void randArray(int arr[], int size, int maxValue)
{
    for(int i = 0; i < size; i++)
        arr[i] = rand() % maxValue + 1;
}


void merge(int arr[], int l, int m, int r)
{
    int i, j, k;

    int n1 = m - l + 1;
    int n2 = r - m;

    int *L = malloc(sizeof(int) * n1);
    int *R = malloc(sizeof(int) * n2);

    for (i = 0; i < n1; i++)
        L[i] = arr[l + i];
    for (j = 0; j < n2; j++)
        R[j] = arr[m + 1 + j];

    i = 0;
    j = 0;
    k = l;

    while (i < n1 && j < n2)
    {
        if (L[i] <= R[j])
        {
            arr[k] = L[i];
            i++;
        }
        else
        {
            arr[k] = R[j];
            j++;
        }
        k++;
    }

    while (i < n1)
    {
        arr[k] = L[i];
        i++;
        k++;
    }

    while (j < n2)
    {
        arr[k] = R[j];
        j++;
        k++;
    }

    free(L);
    free(R);
}

void mergeSort(int arr[], int l, int r)
{
    if (l < r)
    {
        int m = l + (r - l) / 2;
        mergeSort(arr, l, m);
        mergeSort(arr, m + 1, r);
        merge(arr, l, m, r);
    }
}

int partition(int *vals, int low, int high)
{
    int pivot = vals[high];
    int i = low - 1;

    for (int j = low; j < high; j++)
    {
        if (vals[j] < pivot)
        {
            i++;
            swap(&vals[i], &vals[j]);
        }
    }
    swap(&vals[i + 1], &vals[high]);
    return i + 1;
}

void quickSort(int* numbers, int low, int high)
{
    if (low < high)
    {
        int pi = partition(numbers, low, high);

        quickSort(numbers, low, pi - 1);
        quickSort(numbers, pi + 1, high);
    }
}

void selectionSort(int arr[], int n)
{
    int i, j, min_idx;

    for (i = 0; i < n - 1; i++)
    {
        min_idx = i;
        for (j = i + 1; j < n; j++)
            if (arr[j] < arr[min_idx])
                min_idx = j;

        swap(&arr[min_idx], &arr[i]);
    }
}

