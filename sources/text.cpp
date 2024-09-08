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


bool TextSet(Text* textToSet, const char* textFileName)
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


bool TextPrint(const Text* text, const char* outputFileName)
{
    FILE*  outputFile = fopen(outputFileName, "w");
    
    for (size_t lineNum = 0; lineNum < text->lineCount; lineNum++)
    {
        char* lineStart = text->linePointers[lineNum];
        char* lineEnd   = text->linePointers[lineNum + 1];

        if (!PrintLine(lineStart, lineEnd, outputFile))
        {
            fclose(outputFile);
            return false;
        }    
    }    

    fclose(outputFile);
    return true;
}


static bool CopyText(const char* fromFileName, Text* toText)
{
    FILE* fromFile = fopen(fromFileName, "r");
    
    if (!SetTextSize(&toText->textSize, fromFile)) 
        return false;
                                        // CRUTCH: +1 for end of the last line
    toText->textBuffer = (char*) calloc(toText->textSize + 1, sizeof(char));
    if (toText->textBuffer == NULL) 
    {
        fprintf(stderr, "%s: %s(): ERROR in line %d. Memory isn't allocated.",
                        __FILE__, __FUNCTION__, __LINE__);
        fclose(fromFile);
        return false;
    }

    const size_t readedCharsCount = fread(toText->textBuffer, sizeof(char), 
                                          toText->textSize, fromFile);
    if (readedCharsCount != toText->textSize - 1) // -1 needed because the last byte is EOF
    {
        fprintf(stderr, "%s: %s(): ERROR in line %d. fromFile wasn't readed.\n"
                        "readedCharsCount = %zu, toText->textSize = %zu\n",
                        __FILE__, __FUNCTION__, __LINE__,
                        readedCharsCount, toText->textSize);

        fclose(fromFile);
        return false;
    }

    toText->textBuffer[toText->textSize - 1] = '\0';

    fclose(fromFile);
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
    *textSize = ftell(textFile) + 1; // +1 for last '\0'

    if (*textSize == (size_t) (-1L + 1)) 
    {
        fprintf(stderr, "%s: %s(): ERROR in line %d. Can't set text size.",
                        __FILE__, __FUNCTION__, __LINE__);

        *textSize = 0;
        return false;
    }

    rewind(textFile);
    return true;
}


static bool IsTextNotEmpty(Text* text)
{
    if (text->textBuffer   != NULL ||
        text->linePointers != NULL ||
        text->textSize     != 0    ||
        text->lineCount    != 0) 
    {
        return true;
    }

    return false;
}


static void SetLineCount(Text* text) 
{
    size_t lineCount = 0;
    for (size_t charNum = 0; charNum < text->textSize; charNum++) 
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
{                                       // CRUTCH: +1 for end of the last line
    text->linePointers = (char**) calloc(text->lineCount + 1, sizeof(char*)); 
    if (text->linePointers == NULL)
    {
        fprintf(stderr, "%s: %s(): ERROR in line %d. Memory can't be allocated.",
                        __FILE__, __FUNCTION__, __LINE__);
        return false;
    }

    text->linePointers[0] = text->textBuffer;
    size_t lineNum        = 1;

    for (size_t charNum = 0; charNum < text->textSize; charNum++) 
        if (text->textBuffer[charNum] == '\0')
        {
            text->linePointers[lineNum] = &(text->textBuffer[charNum]) + 1;
            lineNum++;
        }

    return true;
}


void TextDelete(Text* text) {
    free(text->textBuffer);
    free(text->linePointers);

    text->textBuffer   = NULL;
    text->linePointers = NULL;
    text->textSize     = 0;
    text->lineCount    = 0;

}


static bool PrintLine(const char* lineStart, const char* lineEnd, FILE* outputFile) 
{
    const size_t charCount = (size_t) (lineEnd - lineStart - 1);

    if (fwrite(lineStart, sizeof(char), charCount, outputFile) != charCount) 
    {
        fprintf(stderr, "%s: %s(): ERROR in line %d. Full line can't be printed.",
                        __FILE__, __FUNCTION__, __LINE__);
        return false;
    }

    if (fputc('\n', outputFile) == EOF)
    {
        fprintf(stderr, "%s: %s(): ERROR in line %d. End of line can't be printed.",
                        __FILE__, __FUNCTION__, __LINE__);
        return false;
    }

    return true;
}