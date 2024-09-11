#include <assert.h>
#include <ctype.h>
#include <stddef.h>
#include "../headers/textSorter.h"
#include "../headers/logPrinter.h"


/**
 * 
 */
struct LineQSortStruct
{
    Text* const  text;
    size_t       leftEdge;
    size_t       rightEdge;
};


/**
 * 
 */
static void SwapLines(char** firstLinePtr, char** secondLinePtr);

            
/**
 * 
 */
static int CompareLines(const char*  firstLine, const char* secondLine);


/**
 * 
 */
static void SkipUselessChars(const char**  linePointer);


/**
 * 
 */
static void LineQSort(LineQSortStruct lineQSortStruct);


/**
 * 
 */
static size_t LineQSortPartition(LineQSortStruct* lineQSortStruct);


/**
 * 
 */
static size_t LineQSortGetPivotNum(LineQSortStruct* lineQSortStruct);


void SortTextLines(Text* text) 
{
    for (size_t firstLineNum = 0; firstLineNum < text->lineCount; firstLineNum++) 
        for (size_t secondLineNum = 0; secondLineNum < text->lineCount; secondLineNum++) 
        {
            char** firstLinePtr  = &(text->linePointers[firstLineNum]);
            char** secondLinePtr = &(text->linePointers[secondLineNum]);

            int compareResult = CompareLines(*firstLinePtr, *secondLinePtr);

            if (compareResult < 0) // firstLine < secondLine
                SwapLines(firstLinePtr, secondLinePtr);
        }
}


static void SwapLines(char** firstLinePtr, char** secondLinePtr) 
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

        if (*firstLine != *secondLine)
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


static void LineQSort(LineQSortStruct lineQSortStruct)
{

}


static size_t LineQSortPartition(LineQSortStruct* lineQSortStruct)
{

}


static size_t LineQSortGetPivotNum(LineQSortStruct* lineQSortStruct) 
{
    
}
