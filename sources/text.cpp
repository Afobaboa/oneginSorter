#include <stdio.h>
#include "../headers/text.h"


/**
 * 
 */
static bool CopyText(FILE* fromTextFile, char* toTextBuffer);


/**
 * 
 */
static bool SetLineCountAndPointers(Text* text);


bool SetText(Text* textToSet, const char* textFileName)
{
    FILE* textFile = fopen(textFileName, "r");

    if (!CopyText(textFile, textToSet->textBuffer))
    {
        fclose(textFile);
        return false;
    }

    if (!SetLineCountAndPointers(textToSet)) 
    {
        fclose(textFile);
        return false;
    }

    fclose(textFile);
    return true;
}


void PrintText(const Text* text, const char* outputFileName)
{

}


static bool CopyText(FILE* fromTextFile, char* toTextBuffer)
{
    
}


static bool SetLineCountAndPointers(Text* text)
{

}