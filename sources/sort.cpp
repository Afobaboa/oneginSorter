#include <assert.h>
#include <ctype.h>
#include <stddef.h>
#include "../headers/sort.h"


#include <string.h>
#include <stdio.h>


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
            char* firstLine  = text->linePointers[firstLineNum];
            char* secondLine = text->linePointers[secondLineNum];

            // int compareResult = CompareLines(firstLine, secondLine);
            int compareResult = strcmp(firstLine, secondLine);
            printf("Compare <%s> with <%s>. Result = %d\n", firstLine, secondLine, compareResult);

            if (compareResult < 0) // firstLine < secondLine
                SwapLines(&firstLine, &secondLine);
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