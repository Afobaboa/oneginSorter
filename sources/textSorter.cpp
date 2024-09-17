#include <assert.h>
#include <ctype.h>
#include <stddef.h>
#include <stdlib.h>
#include <time.h>
#include "../headers/textSorter.h"
#include "../headers/logPrinter.h"


#include <stdio.h>


/**
 * 
 */
struct LineQSortArray
{
    char** lineArray;
    size_t leftEdge;
    size_t rightEdge;
};


/**
 * 
 */
static void LineSwap(char** firstLinePtr, char** secondLinePtr);

            
/**
 * 
 */
static int LineCompare(const char*  firstLine, const char* secondLine);


/**
 * 
 */
static void SkipUselessChars(const char** linePointer);


/**
 * 
 */
static void LineQSort(LineQSortArray lineQSortArray, 
                      int (*LineComparator) (const char* firstLine, const char* secondLine));


/**
 * 
 */
static size_t LineQSortPartition(LineQSortArray lineQSortArray,
                                 int (*LineComparator) (const char* firstLine, 
                                                        const char* secondLine));


/**
 * 
 */
static char* LineQSortGetPivot(const LineQSortArray* lineQSortArray);


void SortTextLines(Text* text) 
{
    // for (size_t firstLineNum = 0; firstLineNum < text->lineCount; firstLineNum++) 
    //     for (size_t secondLineNum = 0; secondLineNum < text->lineCount; secondLineNum++) 
    //     {
    //         char** firstLinePtr  = &(text->linePointers[firstLineNum]);
    //         char** secondLinePtr = &(text->linePointers[secondLineNum]);

    //         int compareResult = CompareLines(*firstLinePtr, *secondLinePtr);

    //         if (compareResult < 0) // firstLine < secondLine
    //             LineSwap(firstLinePtr, secondLinePtr);
    //     }

    LineQSortArray lineQSortArray =
    {
        .lineArray = text->linePointers,
        .leftEdge  = 0,
        .rightEdge = text->lineCount - 1
    };

    LineQSort(lineQSortArray, LineCompare);
}


static void LineSwap(char** firstLinePtr, char** secondLinePtr) 
{
    // LOG_PRINT(INFO, "<%s> is swapping with <%s>\n", *firstLinePtr, *secondLinePtr);

    char* tempLine = *firstLinePtr;

    *firstLinePtr  = *secondLinePtr;
    *secondLinePtr = tempLine;
}


static int LineCompare(const char* firstLine, const char* secondLine) 
{   
    // LOG_PRINT(INFO, "Comparing starts.\n");

    while (*firstLine != '\0' && *secondLine != '\0')
    {
        SkipUselessChars(&firstLine);
        SkipUselessChars(&secondLine);

        if (tolower(*firstLine) != tolower(*secondLine) || *firstLine == '\0')
            break;

        firstLine++;
        secondLine++;
    }
    
    return *firstLine - *secondLine;
}


static void SkipUselessChars(const char**  linePointer) 
{
    // LOG_PRINT(INFO, "Skipping useless chars in <%s>", *linePointer);

    while (**linePointer != '\0' && !isalpha(**linePointer)) 
    { 
        // LOG_PRINT(INFO, "Char <%c = %d> isn't a letter", **linePointer, **linePointer);

        (*linePointer)++;
    }    
}


static void LineQSort(LineQSortArray lineQSortArray, 
                      int (*LineComparator) (const char* firstLine, const char* secondLine))
{
    // LOG_PRINT(INFO, "QSorting lines from %zu to %zu\n",
    //                 lineQSortStruct->leftEdge, lineQSortStruct->rightEdge);

    if (lineQSortArray.leftEdge >= lineQSortArray.rightEdge)
    {
        // LOG_PRINT(INFO, "QSorting is ended.\n");

        return;
    }

    size_t pivotEdge     = LineQSortPartition(lineQSortArray, LineComparator);
    size_t rightEdgeCopy = lineQSortArray.rightEdge;

    // LOG_PRINT(INFO, "leftEdge = %zu, pivotEdge = %zu, rightEdge = %zu\n",
    //                 lineQSortStruct->leftEdge, pivotEdge, lineQSortStruct->rightEdge);

    lineQSortArray.rightEdge = pivotEdge ;
    LineQSort(lineQSortArray, LineComparator);  

    lineQSortArray.leftEdge  = pivotEdge + 1;
    lineQSortArray.rightEdge = rightEdgeCopy;
    LineQSort(lineQSortArray, LineComparator);

}


static size_t LineQSortPartition(LineQSortArray lineQSortArray,
                                 int (*LineComparator) (const char* firstLine, 
                                                        const char* secondLine))
{
    char*  linePivot     = LineQSortGetPivot(&lineQSortArray);
    size_t leftIterator  = lineQSortArray.leftEdge;
    size_t rightIterator = lineQSortArray.rightEdge;

    while (leftIterator < rightIterator)
    {
        while (LineComparator(lineQSortArray.lineArray[leftIterator], linePivot) < 0  &&
               leftIterator < lineQSortArray.rightEdge)
        {
            leftIterator++;
        }
        
        while (LineComparator(lineQSortArray.lineArray[rightIterator], linePivot) >= 0 &&
               rightIterator > lineQSortArray.leftEdge)
        {
            rightIterator--;
        }

        if (leftIterator < rightIterator)
        {
            LineSwap(&lineQSortArray.lineArray[leftIterator], 
                     &lineQSortArray.lineArray[rightIterator]);
        }
    }
    LineSwap(&lineQSortArray.lineArray[lineQSortArray.leftEdge],
             &lineQSortArray.lineArray[lineQSortArray.rightEdge]);

    return rightIterator;
}


static char* LineQSortGetPivot(const LineQSortArray* lineQSortArray) 
{
    // LOG_PRINT(INFO, "LineQSortStruct adress = %p", lineQSortStruct);

    srand(time(NULL));
    size_t LinePivotNum = lineQSortArray->leftEdge + (size_t) rand() % 
                         (lineQSortArray->rightEdge - lineQSortArray->leftEdge);

    // LOG_PRINT(INFO, "LinePivotNum = %zu, leftEdge = %zu, rightEdge = %zu\n",
    //                  LinePivotNum, lineQSortStruct->leftEdge, lineQSortStruct->rightEdge);

    return lineQSortArray->lineArray[LinePivotNum];
}
