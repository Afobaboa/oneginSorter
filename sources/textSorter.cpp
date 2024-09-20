#include <assert.h>
#include <ctype.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <time.h>

#include "../headers/textSorter.h"
#include "../headers/logPrinter.h"


//----------------------------------------------------------------------------------------


/**
 * Function which compare two elems
 * using pointers to this elems.
 * 
 * @param firstElemPtr  Pointer to first elem.
 * @param secondElemPtr Pointer to second elem.
 * 
 * @return Negative int number if first elem is bigger than second elem.
 * @return Zero if first elem is equal to second elem.
 * @return Positive int number if first elem is bigger than second elem.
 */
typedef int (*compareFunc_t) (const void* firstElemPtr, const void* secondELemPtr);


//----------------------------------------------------------------------------------------


/**
 * This function compare two Lines by alphabet order.
 * 
 * This comparator ignores bigger case (change it to lower)
 * letters and any symbols which aren't letters.
 * 
 * @param firstLinePtr  Pointer to first line.
 * @param secondLinePtr Pointer to second line.
 * 
 * @return Negative int number if first line is bigger than second line.
 * @return Zero if first line is equal to second line.
 * @return Positive int number if first line is bigger than second line.
 */
static int LineAlphabetCompare(const void* firstLinePtr, const void* secondLinePtr);


/**
 * This function compare two Lines by rhyme alphabet order.
 * 
 * This comparator ignores bigger case (change it to lower)
 * letters and any symbols which aren't letters.
 * 
 * @param firstLinePtr  Pointer to first line.
 * @param secondLinePtr Pointer to second line.
 * 
 * @return Negative int number if first line is bigger than second line.
 * @return Zero if first line is equal to second line.
 * @return Positive int number if first line is bigger than second line.
 */
static int LineRhymeCompare(const void* firstLinePtr, const void* secondLinePtr);


/**
 * This function skip chars which 
 * aren't letters until lineIterator 
 * isn't iteratingEnd and move lineIterator.
 * 
 * @param lineIteratorPtr Pointer to lineIterator.
 * @param IteratingEnd    Ending value of lineIterator.
 */
static void SkipUselessChars(char** lineIteratorPtr, char* iteratingEnd);


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


//----------------------------------------------------------------------------------------


void SortTextLines(const sortMode_t sortMode, Text* text) 
{
    switch (sortMode) 
    {
    case ALPHABET:
        QSort(text->lineArray, 0, text->lineCount - 1, sizeof(Line), LineAlphabetCompare);
        // SortTest(text->lineArray, text->lineCount, sizeof(Line), LineAlphabetCompare);
        break;

    case RHYME:
        QSort(text->lineArray, 0, text->lineCount - 1, sizeof(Line), LineRhymeCompare);
        // SortTest(text->lineArray, text->lineCount, sizeof(Line), LineRhymeCompare);
        break;

    default:
        LOG_PRINT(ERROR, "Wrong sorting mode.");
        break;
    }
}


//----------------------------------------------------------------------------------------


static int LineAlphabetCompare(const void* firstLinePtr, const void* secondLinePtr) 
{   
    Line  firstLine      = *((Line*) firstLinePtr);
    Line  secondLine     = *((Line*) secondLinePtr);

    char* firstIterator  = firstLine.start;
    char* firstLineEnd   = firstLine.end;

    char* secondIterator = secondLine.start;
    char* secondLineEnd  = secondLine.end;

    while (firstIterator < firstLineEnd && secondIterator < secondLineEnd)
    {
        SkipUselessChars(&firstIterator,  firstLineEnd);
        SkipUselessChars(&secondIterator, secondLineEnd);

        if (tolower(*firstIterator) != tolower(*secondIterator))
            break;

        firstIterator++;
        secondIterator++;
    }

    return tolower(*firstIterator) - tolower(*secondIterator);
}


static int LineRhymeCompare(const void* firstLinePtr, const void* secondLinePtr)
{
    Line  firstLine       = *((Line*) firstLinePtr);
    Line  secondLine      = *((Line*) secondLinePtr);

    char* firstIterator   = firstLine.end;
    char* firstLineStart  = firstLine.start;

    char* secondIterator  = secondLine.end;
    char* secondLineStart = secondLine.start;

    while (firstIterator > firstLineStart && secondIterator > secondLineStart)
    {
        SkipUselessChars(&firstIterator,  firstLineStart);
        SkipUselessChars(&secondIterator, secondLineStart);

        if (tolower(*firstIterator) != tolower(*secondIterator))
            break;

        firstIterator--;
        secondIterator--;
    }

    return tolower(*firstIterator) - tolower(*secondIterator);
}


static void SkipUselessChars(char** lineIteratorPtr, char* iteratingEnd)
{
    if (*lineIteratorPtr < iteratingEnd)
        while (!isalpha(**lineIteratorPtr)  && *lineIteratorPtr  < iteratingEnd)
            (*lineIteratorPtr)++;
    
    else if (*lineIteratorPtr > iteratingEnd)
        while (!isalpha(**lineIteratorPtr)  && *lineIteratorPtr  > iteratingEnd)
            (*lineIteratorPtr)--;

    else 
        LOG_PRINT(WARNING, "You trying to skip chars when line's already end.");
}


static void Swap(void* firstElemPtr, void* secondElemPtr, const size_t elemSize) 
{
    const size_t swapCount_8_byte = elemSize / 8;
    for (size_t swapNum = 0; swapNum < swapCount_8_byte; swapNum++)
    {
        uint64_t tempBuffer = *((uint64_t*) firstElemPtr);

        *((uint64_t*) firstElemPtr)  = *((uint64_t*) secondElemPtr);
        *((uint64_t*) secondElemPtr) = tempBuffer;

        firstElemPtr  += 8;
        secondElemPtr += 8;
    }

    const size_t swapCount_1_byte = elemSize - 8 * swapCount_8_byte;
    for (size_t swapNum = 0; swapNum < swapCount_1_byte; swapNum++)
    {
        uint8_t tempBuffer = *((uint8_t*) firstElemPtr);

        *((uint8_t*) firstElemPtr)  = *((uint8_t*) secondElemPtr);
        *((uint8_t*) secondElemPtr) = tempBuffer;

        firstElemPtr  += 1;
        secondElemPtr += 1;
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
    // LOG_PRINT(INFO, "QSorting CORRECT\n");
    return true;
}
