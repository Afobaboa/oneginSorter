#include <assert.h>
#include <ctype.h>
#include <stddef.h>
#include <stdlib.h>
#include "../headers/textSorter.h"
#include "../headers/logPrinter.h"


#include <stdio.h>


/**
 * 
 */
struct LineQSortStruct
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
static int CompareLines(const char*  firstLine, const char* secondLine);


/**
 * 
 */
static void SkipUselessChars(const char** linePointer);


/**
 * 
 */
static void LineQSort(LineQSortStruct* lineQSortStruct, 
                      int (*LineComparator) (const char* firstLine, const char* secondLine));


/**
 * 
 */
static size_t LineQSortPartition(LineQSortStruct* lineQSortStruct,
                                 int (*LineComparator) (const char* firstLine, 
                                                        const char* secondLine));


/**
 * 
 */
static size_t LineQSortGetPivotNum(LineQSortStruct* lineQSortStruct);


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

    LineQSortStruct lineQSortStruct
    {
        .lineArray = text->linePointers,
        .leftEdge  = 0,
        .rightEdge = text->lineCount
    };

    LineQSort(&lineQSortStruct, CompareLines);
}


static void LineSwap(char** firstLinePtr, char** secondLinePtr) 
{
    LOG_PRINT(INFO, "<%s> is swapping with <%s>\n", *firstLinePtr, *secondLinePtr);

    char* tempLine = *firstLinePtr;

    *firstLinePtr  = *secondLinePtr;
    *secondLinePtr = tempLine;
}


static int CompareLines(const char* firstLine, const char* secondLine) 
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


static void LineQSort(LineQSortStruct* lineQSortStruct, 
                      int (*LineComparator) (const char* firstLine, const char* secondLine))
{
    LOG_PRINT(INFO, "QSorting lines from %zu to %zu\n",
                    lineQSortStruct->leftEdge, lineQSortStruct->rightEdge);

    if (lineQSortStruct->rightEdge <= lineQSortStruct->leftEdge + 1)
    {
        LOG_PRINT(INFO, "QSorting is ended.\n");

        return;
    }

    size_t pivotEdge     = LineQSortPartition(lineQSortStruct, LineComparator);
    size_t rightEdgeCopy = lineQSortStruct->rightEdge;

    LOG_PRINT(INFO, "leftEdge = %zu, pivotEdge = %zu, rightEdge = %zu\n",
                    lineQSortStruct->leftEdge, pivotEdge, lineQSortStruct->rightEdge);

    lineQSortStruct->rightEdge = pivotEdge - 1;
    LineQSort(lineQSortStruct, LineComparator);  

    lineQSortStruct->leftEdge  = pivotEdge;
    lineQSortStruct->rightEdge = rightEdgeCopy;
    LineQSort(lineQSortStruct, LineComparator);

}


static size_t LineQSortPartition(LineQSortStruct* lineQSortStruct,
                                 int (*LineComparator) (const char* firstLine, 
                                                        const char* secondLine))
{
    size_t lineForSwapNum = lineQSortStruct->leftEdge;
    size_t linePivotNum   = LineQSortGetPivotNum(lineQSortStruct);

    LOG_PRINT(INFO, "linePivotNum = %d", linePivotNum);
    // LOG_PRINT(INFO, "linePivotPointer = %p\n", linePivotPointer);

    for (size_t lineNum = lineQSortStruct->leftEdge; 
                lineNum < lineQSortStruct->rightEdge; lineNum++)
    {
        char** linePtr        = &(lineQSortStruct->lineArray[lineNum]);
        char** lineForSwapPtr = &(lineQSortStruct->lineArray[lineForSwapNum]);

        LOG_PRINT(INFO, "linePtr = %p and points to %p,\n\t<%s>\n"
                        "\tlineForSwapPtr = %p and points to %p.\n\t<%s>\n"
                        "leftEdge = %zu, rightEdge = %zu\n",
                         linePtr,                  *linePtr,        *linePtr,
                         lineForSwapPtr,           *lineForSwapPtr, *lineForSwapPtr,
                         lineQSortStruct->leftEdge, lineQSortStruct->rightEdge);

        if (LineComparator(*linePtr, lineQSortStruct->lineArray[linePivotNum]) < 0)
        {
            if (lineForSwapNum == linePivotNum)
                linePivotNum = lineForSwapNum;

            if (lineNum != lineForSwapNum)
                LineSwap(linePtr, lineForSwapPtr);

            lineForSwapNum++;
        }
    }

    return linePivotNum;
}


static size_t LineQSortGetPivotNum(LineQSortStruct* lineQSortStruct) 
{
    // LOG_PRINT(INFO, "LineQSortStruct adress = %p", lineQSortStruct);

    size_t LinePivotNum = lineQSortStruct->leftEdge + (size_t) rand() % 
                         (lineQSortStruct->rightEdge - lineQSortStruct->leftEdge);

    // LOG_PRINT(INFO, "LinePivotNum = %zu, leftEdge = %zu, rightEdge = %zu\n",
    //                  LinePivotNum, lineQSortStruct->leftEdge, lineQSortStruct->rightEdge);

    return LinePivotNum;
}
