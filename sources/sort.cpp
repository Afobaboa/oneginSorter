#include <ctype.h>
#include <stddef.h>
#include "../headers/sort.h"


/**
 * 
 */
static void SwapLines(char*        firstLine, char* secondLine, 
                      const size_t LINE_LENGTH);

            
/**
 * 
 */
static signed char CompareLines(const char*  firstLine, const char* secondLine,
                                const size_t LINE_LENGTH);


/**
 * 
 */
static char* GetLine(const char*  lineArray,  const size_t lineNum,
                     const size_t LINE_COUNT, const size_t LINE_LENGTH);


/**
 * 
 */
static void SwapElements_8(void* firstElement, void* secondElement);



void SortLines(char*        lineArray,
               const size_t LINE_COUNT, const size_t LINE_LENGTH) 
{
    for (size_t firstLineNum = 0; firstLineNum < LINE_COUNT; firstLineNum++) 
    {
        for (size_t secondLineNum = 0; secondLineNum < LINE_COUNT - firstLineNum; 
                                                                 secondLineNum++) 
        {
            char* firstLine  = GetLine(lineArray,  firstLineNum, 
                                       LINE_COUNT, LINE_LENGTH);
            char* secondLine = GetLine(lineArray,  secondLineNum,
                                       LINE_COUNT, LINE_LENGTH);

            signed char compareResult = CompareLines(firstLine, secondLine,
                                                     LINE_LENGTH);

            if (compareResult < 0) // firstLine < secondLine
                SwapLines(firstLine, secondLine,
                          LINE_LENGTH);
        }
    }    
}



static void SwapLines(char*        firstLine, char* secondLine, 
                      const size_t LINE_LENGTH) 
{
    const size_t swappingPartSize = sizeof(long long); // 8 byte
    size_t iterationsCount = LINE_LENGTH / swappingPartSize;

    for (size_t iterator = 0; iterator < iterationsCount; iterator++)
    {
        SwapElements_8((void*) firstLine, (void*) secondLine);

        firstLine  += swappingPartSize;
        secondLine += swappingPartSize;
    }
}



static signed char CompareLines(const char*  firstLine, const char* secondLine,
                                const size_t LINE_LENGTH) 
{
    for (size_t charNum = 0; charNum < LINE_LENGTH; charNum++) 
    {
        const char firstChar  = firstLine[charNum];
        const char secondChar = secondLine[charNum];

        if (firstChar == '\0')
            return (signed char) (firstChar - secondChar);

        else if (firstChar == secondChar)
            return (signed char) 0;

        else 
            return (signed char) (firstChar - secondChar);
    }
}



static char* GetLine(const char*  lineArray,  const size_t lineNum,
                     const size_t LINE_COUNT, const size_t LINE_LENGTH)
{
    if (lineNum < LINE_COUNT)
        return (char*) lineArray + lineNum * LINE_LENGTH;

    return NULL; 
}



static void SwapElements_8 (void* firstElement, void* secondELement) 
{
    long long*   tempElement   = (long long*) firstElement;

    firstElement  = (long long*) secondELement;
    secondELement =              tempElement;
}