#include <stdio.h>
#include <stdlib.h>
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


/**
 * 
 */
static void SetLineCount(Text* text);


/**
 * 
 */
static bool SetLinePointers(Text* text);


/** 
 * 
 */
static bool PrintLine(const char* lineStart, const char* lineEnd, FILE* outputFile);


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
    
    
    
    fclose(outputFile);
}


static bool CopyText(const char* fromFileName, Text* toText)
{
    FILE* fromFile = fopen(fromFileName, "r");
    
    if (!SetTextSize(&toText->textSize, fromFile)) 
        return false;

    toText->textBuffer = (char*) calloc(toText->textSize, sizeof(char));
    if (toText->textBuffer == NULL) 
    {
        fprintf(stderr, "%s: %s(): ERROR in line %d. Memory isn't allocated.",
                        __FILE__, __FUNCTION__, __LINE__);
        return false;
    }

    toText->textBuffer[toText->textSize - 1] = '\0';

    return true;
}


static bool SetLineCountAndPointers(Text* text)
{
    SetLineCount(text);    

    if (!SetLinePointers(text))
        return false;

    return true;
}


static bool SetTextSize(size_t* textSize, FILE* textFile) 
{
    fseek(textFile, 0L, SEEK_END);
    *textSize = ftell(textFile) + 1;

    if (*textSize == (size_t) (-1L + 1)) 
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


static void SetLineCount(Text* text) 
{
    size_t lineCount = 0;
    for (size_t charNum = 0; charNum < text->lineCount; charNum++) 
    {
        char textChar = text->textBuffer[charNum];

        if (textChar == '\0')
            lineCount++;

        if (textChar == '\n') 
        {
            text->textBuffer[charNum] = '\0';
            lineCount++;
        }
    }
    text->lineCount   = lineCount;
}


static bool SetLinePointers(Text* text) 
{
    text->linePointer = (char**) calloc(text->lineCount + 1, sizeof(char*)); // +1 for end of the last line
    if (text->linePointer == NULL)
    {
        fprintf(stderr, "%s: %s(): ERROR in line %d. Memory can't be allocated.",
                        __FILE__, __FUNCTION__, __LINE__);
        return false;
    }

    text->linePointer[0] = text->textBuffer;
    size_t lineNum       = 1;

    for (size_t charNum = 0; charNum < text->textSize; charNum++) 
        if (text->textBuffer[charNum] == '\0')
        {
            text->linePointer[lineNum] = &(text->textBuffer[charNum]) + 1;
            lineNum++;
        }

    return true;
}


void DeleteText(Text* text) {
    free(text->textBuffer);
    free(text->linePointer);

    text->textBuffer  = NULL;
    text->linePointer = NULL;
    text->textSize    = 0;
    text->lineCount   = 0;

}


static bool PrintLine(const char* lineStart, const char* lineEnd, FILE* outputFile) 
{
    
}