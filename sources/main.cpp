#include <stdio.h>
#include "../headers/sort.h"


static const size_t LINE_LENGHT = 8 * 2; // 8 for fast swapping
static const size_t LINE_COUNT  = 5;


static void PrintLines(const char*  lineArray);



int main()
{
    // FILE* oneginText = fopen("miniOnegin.txt", "r");
    // fclose(oneginText);

    char lineArray[LINE_COUNT][LINE_LENGHT] = {"hello, my name",
                                               "My",
                                               "name",
                                               "is Daniil"};

    SortLines((char*) lineArray, LINE_COUNT, LINE_LENGHT);

    PrintLines((char*) lineArray);

    return 0;
}



static void PrintLines(const char*  lineArray) 
{
    for (size_t lineNum = 0; lineNum < LINE_COUNT; lineNum++) 
    {
        puts(lineArray + lineNum * LINE_LENGHT);
    }
}