#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include "../headers/textProcessor.h"


/**
 * This function copy text 
 * from file to struct Text.
 * 
 * @param fromFileName Name of file 
 * ehich text you want to copy.
 * @param toText       Pointer to Text.
 * 
 * @return true if OK,
 * @return false if error.
 */
static bool TextCopy(const char* fromFileName, Text* toText);


/**
 * This function set lineCount and
 * linePointers in your Text.
 * You need to set lineBuffer and
 * lineSize before using.
 * 
 * @param text Pointer to your Text.
 * 
 * @return true of OK,
 * @return false if error.
 */
static bool LineSetCountAndPointers(Text* text);


/**
 * This function set needed size of Text
 * to copy content of file and add analogue
 * of EOF to end of Text.
 * 
 * @param textSize Pointer to variable 
 * of text's size.
 * @param textFile File, you want to copy.
 * 
 * @return true if OK,
 * @return false if error.
 */
static bool TextSetSize(size_t* textSize, FILE* textFile);


/**
 * This function check if Test is 
 * already setted.
 * 
 * @param text text you want to check.
 * 
 * @return true if text is setted.
 * @return flse if it's false.
 */
static bool IsTextNotEmpty(Text* text);


/**
 * This function set count of 
 * Text's lines. You need to set
 * textBuffer before using.
 * 
 * @param text Text which lines 
 * you want to calculate.
 * 
 * @return true if OK,
 * @return false if error.
 */
static void LineSetCount(Text* text);


/**
 * This function create array of
 * ppointers to lines of Text.
 * You need to set other fields
 * of Text before using.
 * 
 * @param text Text you want to 
 * separate to lines.
 * 
 * @return true if OK,
 * @return false if error.
 */
static bool LineSetPointers(Text* text);


/** 
 * This function print one line.
 * 
 * @param line        Line.
 * @param outputFile  File where line 
 * will be printed.
 *
 * @return true if OK,
 * @return false if error. 
 */
static bool LinePrint(const char* linePointer, FILE* outputFile);


/**
 * This function try to find size
 * of text (and set it) and calloc 
 * memory for this text.
 * 
 * @param text Text which textBuffer
 * you want to calloc.
 * @param textFile File which text
 * you want to contain in text.
 * 
 * @return true if OK,
 * @return false if error.
 */
static bool TextBufferCalloc(Text* text, FILE* textFile);


/**
 * This function set textSize and
 * textBuffer and copy text from 
 * textFile to text.
 * 
 * @param text     Text you wan to set.
 * @param textFile File which text 
 * you want to copy.
 * 
 * @return true if OK,
 * @return false if error.
 */
static bool TextBufferAndSizeSet(Text* text, FILE* textFile);


bool TextSet(Text* textToSet, const char* textFileName)
{
    assert(textToSet);
    assert(textFileName);

    if (IsTextNotEmpty(textToSet)) {
        fprintf(stderr, "%s: %s(): ERROR in line %d. Text isn't empty.",
                        __FILE__, __FUNCTION__, __LINE__);
        return false;
    }

    if (!TextCopy(textFileName, textToSet))
        return false;

    if (!LineSetCountAndPointers(textToSet)) 
        return false;

    return true;
}


bool TextPrint(const Text* text, const char* outputFileName)
{   
    assert(text);
    assert(outputFileName);

    FILE*  outputFile = fopen(outputFileName, "w");
    assert(outputFile);
    
    for (size_t lineNum = 0; lineNum < text->lineCount; lineNum++)
    {
        char* linePointer = text->linePointers[lineNum];

        if (!LinePrint(linePointer, outputFile))
        {
            fclose(outputFile);
            return false;
        }    
    }    

    fclose(outputFile);
    return true;
}


static bool TextCopy(const char* fromFileName, Text* toText) // FIXME: too big
{
    FILE* fromFile = fopen(fromFileName, "r");

    if (!TextBufferAndSizeSet(toText, fromFile)) 
    {
        fclose(fromFile);
        return false;
    }

    fclose(fromFile);
    return true;
}


static bool LineSetCountAndPointers(Text* text)
{
    assert(text->textBuffer);

    LineSetCount(text);    

    if (!LineSetPointers(text))
        return false;

    return true;
}


static bool TextSetSize(size_t* textSize, FILE* textFile) 
{
    assert(textSize);
    assert(textFile);

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
    assert(text);

    if (text->textBuffer   != NULL ||
        text->linePointers != NULL ||
        text->textSize     != 0    ||
        text->lineCount    != 0) 
    {
        return true;
    }

    return false;
}


static void LineSetCount(Text* text) 
{
    assert(text->textBuffer);

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


static bool LineSetPointers(Text* text) 
{   
    assert(text);
                                        // CRUTCH: +1 for end of the last line
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
    assert(text);

    free(text->textBuffer);
    free(text->linePointers);

    text->textBuffer   = NULL;
    text->linePointers = NULL;
    text->textSize     = 0;
    text->lineCount    = 0;

}


static bool LinePrint(const char* line, FILE* outputFile) 
{
    assert(line);
    assert(outputFile);

    if (fprintf(outputFile, "%s\n", line) < 0)
    {
        fprintf(stderr, "%s: %s(): ERROR in line %d. Line wasn't printed.",
                        __FILE__, __FUNCTION__, __LINE__);
        return false;
    }

    return true;
}


static bool TextBufferCalloc(Text* text, FILE* textFile)
{
    assert(text);
    assert(textFile);

    if (!TextSetSize(&text->textSize, textFile)) 
    {
        fclose(textFile);
        return false;
    }    
                                        // CRUTCH: +1 for end of the last line
    text->textBuffer = (char*) calloc(text->textSize + 1, sizeof(char));
    if (text->textBuffer == NULL) 
    {
        fprintf(stderr, "%s: %s(): ERROR in line %d. Memory isn't allocated.",
                        __FILE__, __FUNCTION__, __LINE__);
        fclose(textFile);
        return false;
    }

    return true;
}


static bool TextBufferAndSizeSet(Text* text, FILE* textFile)
{
    assert(text);
    assert(textFile);

    if (!TextBufferCalloc(text, textFile))
        return false;

    const size_t readedCharsCount = fread(text->textBuffer, sizeof(char), 
                                          text->textSize, textFile);
    if (readedCharsCount != text->textSize - 1) // -1 needed because the last byte is EOF
    {
        fprintf(stderr, "%s: %s(): ERROR in line %d. fromFile wasn't readed.\n"
                        "readedCharsCount = %zu, toText->textSize = %zu\n",
                        __FILE__, __FUNCTION__, __LINE__,
                        readedCharsCount, text->textSize);

        fclose(textFile);
        return false;
    }

    text->textBuffer[text->textSize - 1] = '\0';

    return true;
}