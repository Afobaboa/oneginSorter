#include <stdio.h>
#include "../headers/text.h"


/**
 * 
 */
static bool CopyText(const char* fromFileName, Text* toText);


/**
 * 
 */
static bool SetLineCountAndPointers(Text* text);


/**
 * 
 */
static bool SetTextSize(size_t* textSize, FILE* textFile);


/**
 * 
 */
static bool IsTextNotEmpty(Text* text);


bool SetText(Text* textToSet, const char* textFileName)
{
    if (IsTextNotEmpty(textToSet)) {
        fprintf(stderr, "%s: %s(): ERROR in line %d. Text isn't empty.",
                        __FILE__, __FUNCTION__, __LINE__);
        return false;
    }

    if (!CopyText(textFileName, textToSet))
        return false;

    if (!SetLineCountAndPointers(textToSet)) 
        return false;

    return true;
}


void PrintText(const Text* text, const char* outputFileName)
{
    FILE*  outputFile        = fopen(outputFileName, "w");
    size_t writtenCharsCount = fwrite(text->textBuffer, sizeof(char), text->textSize, outputFile);
    
    if (writtenCharsCount < text->textSize) 
        fprintf(stderr, "%s: %s(): ERROR in line %d, cant't write all text to output.",
                         __FILE__, __FUNCTION__, __LINE__);
    
    fclose(outputFile);
}


static bool CopyText(const char* fromFileName, Text* toText)
{
    FILE* fromFile = fopen(fromFileName, "r");

    if (!SetTextSize(&toText->textSize, fromFile)) 
        return false;

    
}


static bool SetLineCountAndPointers(Text* text)
{
    
}


static bool SetTextSize(size_t* textSize, FILE* textFile) 
{
    fseek(textFile, 0L, SEEK_END);
    *textSize = ftell(textFile);

    if (*textSize == (size_t) -1L) 
    {
        fprintf(stderr, "%S: %s(): ERROR in line %d. Can't set text size.",
                        __FILE__, __FUNCTION__, __LINE__);

        *textSize = 0;
        return false;
    }

    rewind(textFile);
    return true;
}


static bool IsTextNotEmpty(Text* text)
{
    if (text->textBuffer  != NULL ||
        text->linePointer != NULL ||
        text->textSize    != 0    ||
        text->lineCount   != 0) 
    {
        return true;
    }

    return false;
}