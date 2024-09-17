#include <stdio.h>
#include <stdlib.h>
#include <time.h>


typedef int (*compareFunc_t) (const void*, const void*);


static int CompareInt(const void* firstElemPtr, const void* secondElemPtr);


static void QSort(void* array, size_t leftEdge, size_t rightEdge, size_t elemSize, 
                  compareFunc_t Compare);


static size_t Partition(void* array, size_t leftEdge, size_t rightEdge, size_t elemSize, 
                        compareFunc_t Compare);


static void Swap(void* firstElemPtr, void* secondElemPtr, const size_t elemSize);


//------------------------------------------------------------------------------------


static const size_t size      = 10;
static int          arr[size] = {};


void swap(int* a, int* b) {
    int temp = *a;
    *a = *b;
    *b = temp;
}


int partition(int arr[], size_t low, size_t high) {

    // Initialize pivot to be the first element
    srand(time(NULL));
    size_t pivotNum = low + (size_t) rand() % (high - low);
    // printf("Low = %zu, high = %zu, pivotNum = %zu\n", low, high, pivotNum);

    swap(&arr[pivotNum], &arr[low]);

    int p = arr[low];
    
    // printf("Pivot = %d\n", p);

    size_t i = low;
    size_t j = high;

    while (i < j) {

        // Find the first element greater than
        // the pivot (from starting)
        while (CompareInt(&arr[i], &p) < 0 && i < high) {
            i++;
        }

        // Find the first element smaller than
        // the pivot (from last)
        while (CompareInt(&arr[j], &p) >= 0 && j > low) {
            j--;
        }
        if (i < j) {
            Swap(&arr[i], &arr[j], sizeof(int));
        }
    }

    // for (size_t intNum = 0; intNum < size; intNum++)
    //     printf("%d ", arr[intNum]);

    // printf("\n\n");

    return j;
}

void quickSort(int arr[], int low, int high) {
    if (low < high) {

        // call partition function to find Partition Index
        int pi = partition(arr, low, high);

        // Recursively call quickSort() for left and right
        // half based on Partition Index
        if (pi > low)
            quickSort(arr, low, pi);

        if (pi < high)
            quickSort(arr, pi+1, high);
    }
}


//------------------------------------------------------------------------------


static int CompareInt(const void* firstElemPtr, const void* secondElemPtr) 
{
    return -(*((int*) firstElemPtr)) + *((int*) secondElemPtr);
}


static void Swap(void* firstElemPtr, void* secondElemPtr, size_t elemSize) 
{
    for (size_t byteNum = 0; byteNum < elemSize; byteNum++) 
    {
        char tempByte = ((char*) firstElemPtr)[byteNum];

        ((char*) firstElemPtr)[byteNum]  = ((char*) secondElemPtr)[byteNum];
        ((char*) secondElemPtr)[byteNum] = tempByte;
    }
}


static size_t Partition(void* array, size_t leftEdge, size_t rightEdge, size_t elemSize, 
                        compareFunc_t Compare)
{
    srand(time(NULL));
    size_t pivotNum = leftEdge + (size_t) rand() % (rightEdge - leftEdge);

    Swap(arr + pivotNum, arr + leftEdge, elemSize);

    void* pivotPtr = arr + leftEdge;

    size_t leftIterator  = leftEdge;
    size_t rightIterator = rightEdge;

    while (leftIterator < rightIterator) 
    {
        while (Compare(arr + leftIterator, pivotPtr) < 0 && leftIterator < rightEdge) 
            leftIterator++;
        

        while (Compare(arr + rightIterator, pivotPtr) >= 0 && rightIterator > leftEdge) 
            rightIterator--;
        
        if (leftIterator < rightIterator) 
            Swap(arr + leftIterator, arr + rightIterator, elemSize);
    }

    return rightIterator;
}


static void QSort(void* array, size_t leftEdge, size_t rightEdge, size_t elemSize, 
                  compareFunc_t Compare)
{
    if (leftEdge < rightEdge) 
    {
        // size_t middleEdge = Partition(arr, leftEdge, rightEdge, elemSize, Compare);
        size_t middleEdge = Partition(arr, leftEdge, rightEdge, elemSize, Compare);

        if (middleEdge > leftEdge)
            QSort(arr, leftEdge, middleEdge, elemSize, Compare);

        if (middleEdge < rightEdge)
            QSort(arr, middleEdge + 1, rightEdge, elemSize, Compare);
    }
}


//---------------------------------------------------------------------------------


int main() 
{
    printf("Size = %zu\n", size);

    srand(time(NULL));
    for (size_t intNum = 0; intNum < size; intNum++)
        arr[intNum] = rand() % 400;
    
    QSort(arr, 0, size - 1, sizeof(int), CompareInt);

    for (size_t intNum = 0; intNum < size; intNum++)
        printf("%d ", arr[intNum]);

    printf("\n");

    return 0;
}