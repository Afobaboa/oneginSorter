#include <assert.h>
#include <ctype.h>
#include <stddef.h>
#include "../headers/sort.h"


/**
 * 
 */
static void SwapLines(char** firstLinePtr, char** secondLinePtr);

            
/**
 * 
 */
static int CompareLines(const char*  firstLine, const char* secondLine);


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


static int CompareLines(const char*  firstLine, const char* secondLine) 
{
    while (*firstLine != '\0' && *secondLine != '\0') 
    {
        if (*firstLine != *secondLine)
            return *firstLine - *secondLine;

        firstLine++;
        secondLine++;
    }
    
    return *firstLine - *secondLine;
}