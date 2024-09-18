#include <assert.h>
#include <ctype.h>
#include <stddef.h>
#include <stdlib.h>
#include <time.h>
#include "../headers/textSorter.h"
#include "../headers/logPrinter.h"


#include <string.h>


/**
 * Function which compare two elems
 * using pointers to this elems.
 * 
 * @param firstElemPtr  Pointer to first elem.
 * @param secondElemPtr Pointer to second elem.
 * 
 * @return Negative int number if first elem
 *         is bigger than second elem.
 * @return Zero if first elem is equal to second elem.
 * @return Positive int number if first elem 
 *         is bigger than second elem.
 */
typedef int (*compareFunc_t) (const void* firstElemPtr, const void* secondELemPtr);


/**
 * This function compare two null-terminated strings.
 * 
 * This comparator ignores bigger case (change it to lower)
 * letters and any symbols which aren't letters.
 * 
 * @param firstLinePtr  Pointer to first line.
 * @param secondLinePtr Pointer to second line.
 * 
 * @return Negative int number if first line
 *         is bigger than second line.
 * @return Zero if first line is equal to second line.
 * @return Positive int number if first line 
 *         is bigger than second line.
 */
static int LineCompare(const void* firstLinePtr, const void* secondLinePtr);


/**
 * This function are moving pointer to 
 * null-terminated line ahead if there 
 * isn't a letter. Moving will stop if
 * there is EOF or '\0' or a letter.
 * 
 * @param linePointer Pointer to line.
 */
static void SkipUselessChars(char** linePointer);


/**
 * Quick sorting recursive function.
 * 
 * @param array     Array of elems.
 * @param leftEdge  Left edge of sorting area.
 * @param rightEdge Right edge of sorting area.
 * @param elemSize  Size of elems in bytes.
 * @param Compare   Function which compares elems.
 */
static void QSort(void* array, size_t leftEdge, size_t rightEdge, size_t elemSize, 
                  compareFunc_t Compare);


/**
 * Function which check if elems
 * of array are sorted in right order.
 * 
 * @param array     Array of sorting elements.
 * @param elemCount Count of sorted elems.
 * @param elemSize  Size of sorting elems in bytes.
 * @param Compare   Function which compares elems.
 * 
 * @return true if elems are in right order,
 * @return false in other situations.
 */
static bool SortTest(void* array, size_t elemCount, size_t elemSize, 
                      compareFunc_t Compare);


/**
 * This function do partition for QSort algorythm.
 * 
 * @param array     Pointer to array of data.
 * @param leftEdge  Left edge of sorting area.
 * @param rightEdge Right edge of sorting area.
 * @param elemSize  Size of sorting elems in bytes.
 * @param Compare   Function for comparing elems.
 * 
 * @return Count of elems wich less than random pivot.
 */
static size_t Partition(void* array, size_t leftEdge, size_t rightEdge, size_t elemSize, 
                        compareFunc_t Compare);


/**
 * This function swapping memory
 * of elements by their pointers.
 * 
 * @param firstElemPtr  Pointer to first elem.
 * @param secondElemPtr Pointer to second elem.
 * @param elemSize      Size of elems in bytes.
 */
static void Swap(void* firstElemPtr, void* secondElemPtr, const size_t elemSize);


void SortTextLines(Text* text) 
{
    QSort(text->linePointers, 0, text->lineCount - 1, sizeof(char*), LineCompare);
    SortTest(text->linePointers, text->lineCount, sizeof(char*), LineCompare);
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


static bool SortTest(void* array, size_t elemCount, size_t elemSize, 
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
