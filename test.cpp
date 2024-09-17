#include <stdio.h>
#include <stdlib.h>
#include <time.h>


//----------------------------------------------------------------------------------------


typedef int (*compareFunc_t) (const void*, const void*);


static int CompareInt(const void* firstElemPtr, const void* secondElemPtr);


static void QSort(void* array, size_t leftEdge, size_t rightEdge, size_t elemSize, 
                  compareFunc_t Compare);


static size_t Partition(void* array, size_t leftEdge, size_t rightEdge, size_t elemSize, 
                        compareFunc_t Compare);


static void Swap(void* firstElemPtr, void* secondElemPtr, const size_t elemSize);


//----------------------------------------------------------------------------------------


static int CompareInt(const void* firstElemPtr, const void* secondElemPtr) 
{
    return *((int*) firstElemPtr) - *((int*) secondElemPtr);
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

    Swap((char*) array + pivotNum * elemSize, 
         (char*) array + leftEdge * elemSize, elemSize);

    void* pivotPtr = (char*) array + leftEdge * elemSize;

    size_t leftIterator  = leftEdge;
    size_t rightIterator = rightEdge;

    while (leftIterator < rightIterator) 
    {
        while (Compare((char*) array + leftIterator * elemSize, pivotPtr) < 0 && 
               leftIterator < rightEdge) 
        {
            leftIterator++;
        }
        

        while (Compare((char*) array + rightIterator * elemSize, pivotPtr) >= 0 && 
               rightIterator > leftEdge) 
        {    
            rightIterator--;
        }

        if (leftIterator < rightIterator) 
            Swap((char*) array + leftIterator  * elemSize, 
                 (char*) array + rightIterator * elemSize, elemSize);
    }

    return rightIterator;
}


static void QSort(void* array, size_t leftEdge, size_t rightEdge, size_t elemSize, 
                  compareFunc_t Compare)
{
    if (leftEdge < rightEdge) 
    {
        // size_t middleEdge = Partition(arr, leftEdge, rightEdge, elemSize, Compare);
        size_t middleEdge = Partition(array, leftEdge, rightEdge, elemSize, Compare);

        if (middleEdge > leftEdge)
            QSort(array, leftEdge, middleEdge, elemSize, Compare);

        if (middleEdge < rightEdge)
            QSort(array, middleEdge + 1, rightEdge, elemSize, Compare);
    }
}


//----------------------------------------------------------------------------------------


static void PrintIntArr(int* arr, size_t size)
{
for (size_t intNum = 1; intNum < size; intNum++)
        printf("%d ", arr[intNum]);
    printf("\n\n");
}


int main() 
{
    const size_t size      = 10;
    int          arr[size] = {};

    printf("Size = %zu\n", size);

    srand(time(NULL));
    for (size_t intNum = 0; intNum < size; intNum++)
        arr[intNum] = rand() % 400;

    PrintIntArr(arr, size);
    QSort(arr, 0, size - 1, sizeof(int), CompareInt);
    PrintIntArr(arr, size);

    return 0;
}