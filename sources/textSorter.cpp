#include <assert.h>
#include <ctype.h>
#include <stddef.h>
#include <stdlib.h>
#include <time.h>
#include "../headers/textSorter.h"
#include "../headers/logPrinter.h"


#include <string.h>


/**
 * 
 */
typedef int (*compareFunc_t) (const void*, const void*);


/**
 * 
 */
static int LineCompare(const void*  firstLine, const void* secondLine);


/**
 * 
 */
static void SkipUselessChars(char** linePointer);


/**
 * 
 */
static void QSort(void* array, size_t leftEdge, size_t rightEdge, size_t elemSize, 
                  compareFunc_t Compare);


/**
 * 
 */
static bool QSortTest(void* array, size_t elemCount, size_t elemSize, 
                      compareFunc_t Compare);


/**
 * 
 */
static size_t Partition(void* array, size_t leftEdge, size_t rightEdge, size_t elemSize, 
                        compareFunc_t Compare);


/**
 * 
 */
static void Swap(void* firstElemPtr, void* secondElemPtr, const size_t elemSize);


void SortTextLines(Text* text) 
{
    QSort(text->linePointers, 0, text->lineCount - 1, sizeof(char*), LineCompare);
    QSortTest(text->linePointers, text->lineCount, sizeof(char*), LineCompare);
}


static int LineCompare(const void* firstLinePtr, const void* secondLinePtr) 
{   
    char* firstLine  = *((char**) firstLinePtr);
    char* secondLine = *((char**) secondLinePtr);

    // while (*firstLine != '\0' && *secondLine != '\0')
    // {
    //     SkipUselessChars(&firstLine);
    //     SkipUselessChars(&secondLine);

    //     if (tolower(*firstLine) != tolower(*secondLine) || *firstLine == '\0')
    //         break;

    //     firstLine++;
    //     secondLine++;
    // }
    
    // return *firstLine - *secondLine;

    return strcmp(firstLine, secondLine);
}


static void SkipUselessChars(char**  linePointer) 
{
    // LOG_PRINT(INFO, "Skipping useless chars in <%s>", *linePointer);

    while (**linePointer != '\0' && !isalpha(**linePointer)) 
    { 
        // LOG_PRINT(INFO, "Char <%c = %d> isn't a letter", **linePointer, **linePointer);

        (*linePointer)++;
    }    
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
         (char*) array + leftEdge * elemSize, 
         elemSize);

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
                 (char*) array + rightIterator * elemSize, 
                 elemSize);
    }

    return rightIterator;
}


static void QSort(void* array, size_t leftEdge, size_t rightEdge, size_t elemSize, 
                  compareFunc_t Compare)
{
    if (leftEdge < rightEdge) 
    {
        size_t middleEdge = Partition(array, leftEdge, rightEdge, elemSize, Compare);

        if (middleEdge > leftEdge)
            QSort(array, leftEdge, middleEdge, elemSize, Compare);

        if (middleEdge < rightEdge)
            QSort(array, middleEdge + 1, rightEdge, elemSize, Compare);
    }
}


static bool QSortTest(void* array, size_t elemCount, size_t elemSize, 
                      compareFunc_t Compare)
{
    for (size_t elemNum = 1; elemNum < elemCount; elemNum++)
    {
        void* firstElemPtr  = (char*) array + elemSize *  elemNum;
        void* secondElemPtr = (char*) array + elemSize * (elemNum - 1);

        if (Compare(firstElemPtr, secondElemPtr) < 0)
        {
            LOG_PRINT(ERROR, "QSorting WRONG! <%s> < <%s>\n",
                      *((char**) firstElemPtr), *((char**) secondElemPtr));
            return false;
        }
    }
    LOG_PRINT(INFO, "QSorting CORRECT\n");
    return true;
}
