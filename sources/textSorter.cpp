#include <assert.h>
#include <ctype.h>
#include <stddef.h>
#include <stdlib.h>
#include "../headers/textSorter.h"
#include "../headers/logPrinter.h"


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
    char* tempLine = *firstLinePtr;

    *firstLinePtr  = *secondLinePtr;
    *secondLinePtr = tempLine;
}


static int CompareLines(const char* firstLine, const char* secondLine) 
{
    while (*firstLine != '\0' && *secondLine != '\0')
    {
        SkipUselessChars(&firstLine);
        SkipUselessChars(&secondLine);

        if (*firstLine != *secondLine || *firstLine == '\0')
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

    if (lineQSortStruct->rightEdge - lineQSortStruct->leftEdge <= 0)
        return;

    size_t middleEdge    = LineQSortPartition(lineQSortStruct, LineComparator);
    size_t rightEdgeCopy = lineQSortStruct->rightEdge;

    LOG_PRINT(INFO, "leftEdge = %zu, middlEdge = %zu, rightEdge = %zu\n",
                    lineQSortStruct->leftEdge, middleEdge, lineQSortStruct->rightEdge);

    lineQSortStruct->rightEdge = middleEdge - 1;
    LineQSort(lineQSortStruct, LineComparator);

    lineQSortStruct->leftEdge  = middleEdge;
    lineQSortStruct->rightEdge = rightEdgeCopy;
    LineQSort(lineQSortStruct, LineComparator);

}


static size_t LineQSortPartition(LineQSortStruct* lineQSortStruct,
                                 int (*LineComparator) (const char* firstLine, 
                                                        const char* secondLine))
{
    size_t linePivotNum     = LineQSortGetPivotNum(lineQSortStruct);
    char** linePivotPointer = &(lineQSortStruct->lineArray[linePivotNum]);

    for (size_t lineNum = lineQSortStruct->leftEdge; 
                lineNum < lineQSortStruct->rightEdge; lineNum++)
    {
        char** linePointer = &(lineQSortStruct->lineArray[lineNum]);

        if (LineComparator(*linePointer, *linePivotPointer) < 0)
        {
            LOG_PRINT(INFO, "<%s> is swapping with <%s>\n", *linePointer, *linePivotPointer);

            LineSwap(linePointer, linePivotPointer);
        }
    }

    return linePivotNum;
}


static size_t LineQSortGetPivotNum(LineQSortStruct* lineQSortStruct) 
{
    LOG_PRINT(INFO, "LineQSortStruct adress = %p", lineQSortStruct);

    size_t LinePivotNum = (lineQSortStruct->leftEdge + lineQSortStruct->rightEdge) / 2;

    LOG_PRINT(INFO, "LinePivotNum = %zu, leftEdge = %zu, rightEdge = %zu\n",
                     LinePivotNum, lineQSortStruct->leftEdge, lineQSortStruct->rightEdge);

    return LinePivotNum;
}
