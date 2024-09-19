#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include "../headers/logPrinter.h"
#include "../headers/textProcessor.h"


//----------------------------------------------------------------------------------------


/**
 * This function set needed size of Text
 * to copy content of file.
 * 
 * @param text     Pointer to text.
 * @param textFile File, you want to copy.
 * 
 * @return true if OK,
 * @return false if error.
 */
static bool TextSetSize(Text* text, const char* textFileName);


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
 * @param text Text which lines you want to calculate.
 * 
 * @return true if OK,
 * @return false if error.
 */
static void TextSetLineCount(Text* text);


/**
 * This function create array of text
 * lines. You need to set other fields
 * of Text before using.
 * 
 * Every line ends by '\n'.
 * 
 * @param text Text you want to separate to lines.
 * 
 * @return true if OK,
 * @return false if error.
 */
static bool TextSetLineArray(Text* text);


/** 
 * This function print one line.
 * 
 * @param line       Line.
 * @param outputFile File where line will be printed.
 *
 * @return true if OK,
 * @return false if error. 
 */
static bool LinePrint(const Line line, FILE* outputFile);


/**
 * This function try to calloc 
 * memory for this text. You
 * must set size of text before 
 * using this function.
 * 
 * @param text     Text which textBuffer you want to calloc.
 * 
 * @return true if OK,
 * @return false if error.
 */
static bool TextBufferCalloc(Text* text);


/** 
 * This function copy content of file 
 * to your text and chang every '\0' 
 * and '\r' chars to '\n'.
 * 
 * @param text     Text which buffer you want to set.
 * @param textFile File which content you want to copy.
 * 
 * @return true if OK, 
 * @return false if error.
 */
static bool TextSetBuffer(Text* text, FILE* textFile);


/**
 * This function set lineArray and
 * lineCount of your text.
 * 
 * @param text Text you want to set.
 * 
 * @return true if OK,
 * @return false if error.
 */
static bool TextSetLineArrayAndCount(Text* text);


/**
 * 
 */
static bool TextOpenFile(FILE** fileBuffer, const char* fileName, const char* openMode);


/**
 * 
 */
static bool TextPrintEmptyLines(FILE* outputFile);


/**
 * 
 */
static bool TextPrintSeparatingLine(FILE* outputFile);


//----------------------------------------------------------------------------------------


bool TextSet(Text* textToSet, const char* textFileName)
{
    if (IsTextNotEmpty(textToSet)) 
    {
        LOG_PRINT(ERROR, "Text isn't empty.");
        return false;
    }

    FILE* textFile = NULL;
    if (!TextOpenFile(&textFile, textFileName, "r"))
		return false;

    if (!TextSetSize(textToSet, textFileName) ||
        !TextSetBuffer(textToSet, textFile)   ||
        !TextSetLineArrayAndCount(textToSet))
    {
        LOG_PRINT(ERROR, "Text wasn't set using file <%s>", textFileName);
        fclose(textFile);
        return false;
    }

    fclose(textFile);
    return true;
}


bool TextPrintLines(const Text* text, const char* outputFileName)
{   
    assert(text);
    assert(outputFileName);

    FILE* outputFile = NULL;
    if (!TextOpenFile(&outputFile, outputFileName, "a"))
		return false;
    
    for (size_t lineNum = 0; lineNum < text->lineCount; lineNum++)
    {
        if (!LinePrint(text->lineArray[lineNum], outputFile))
        {
            fclose(outputFile);
            return false;
        }    
    }    

    fclose(outputFile);
    return true;
}


bool TextPrintBuffer(const Text* text, const char* outputFileName)
{
    assert(text);
    assert(outputFileName);

    FILE* outputFile = NULL;
    if (!TextOpenFile(&outputFile, outputFileName, "a"))
		return false;

    const size_t printedCharsCount = fwrite(text->textBuffer, sizeof(char), 
                                            text->textSize, outputFile);
    if (printedCharsCount != text->textSize)
    {
        LOG_PRINT(ERROR, "TextBuffer printing isn't complete.");
        fclose(outputFile);
        return false;
    }

    fclose(outputFile);
    return true;
}


bool TextPrintSeparator(const char* outputFileName)
{
    LOG_PRINT(INFO, "Separating starts.");

    assert(outputFileName);

    FILE* outputFile = NULL;
    if (!TextOpenFile(&outputFile, outputFileName, "a"))
    {
        LOG_PRINT(ERROR, "File <%s> can't be opened.", outputFileName);
		return false;
    }
	
    if (!TextPrintEmptyLines(outputFile)     ||
        !TextPrintSeparatingLine(outputFile) ||
        !TextPrintEmptyLines(outputFile))
    {
        fclose(outputFile);
        return false;
    }

    fclose(outputFile);
    return true;
}


bool TextCleanFile(const char* outputFileName)
{
    assert(outputFileName);

    FILE* outputFile = NULL;
    if (!TextOpenFile(&outputFile, outputFileName, "w"))
        return false;

    fclose(outputFile);
    return true;
}


void TextDelete(Text* text) 
{
    assert(text);

    free(text->textBuffer);
    free(text->lineArray);

    text->textBuffer = NULL;
    text->lineArray  = NULL;
    text->textSize   = 0;
    text->lineCount  = 0;

}


//----------------------------------------------------------------------------------------


static bool TextSetSize(Text* text, const char* textFileName) 
{
    struct stat fileStat = {};
    if (stat(textFileName, &fileStat) == -1)
    {
        LOG_PRINT(ERROR, "Text's size wasn't set.");
        return false;
    }

    text->textSize = fileStat.st_size;

    return true;
}


static bool IsTextNotEmpty(Text* text)
{
    assert(text);

    if (text->textBuffer != NULL ||
        text->lineArray  != NULL ||
        text->textSize   != 0    ||
        text->lineCount  != 0) 
    {
        return true;
    }

    return false;
}


static void TextSetLineCount(Text* text) 
{
    size_t lineCount = 0;

    for (size_t charNum = 0; charNum < text->textSize; charNum++) 
    {
        char* textCharPtr = &(text->textBuffer[charNum]);

        if (*textCharPtr == '\0' ||
			*textCharPtr == '\r')
		{
			*textCharPtr = '\n';
            lineCount++;
		}

        if (*textCharPtr == '\n') 
            lineCount++;
    }

    text->lineCount = lineCount;
}


static bool TextSetLineArray(Text* text) 
{   
    text->lineArray = (Line*) calloc(text->lineCount, sizeof(Line)); 
    if (text->lineArray == NULL)
    {
        LOG_PRINT(ERROR, "Memory can't be allocated.");
        return false;
    }

    text->lineArray->start = text->textBuffer;
    size_t lineNum         = 0;

    for (size_t charNum = 0; charNum < text->textSize; charNum++) 
	{
		char* charPtr = &(text->textBuffer[charNum]);

        if (*charPtr == '\n')
        {
            (text->lineArray[lineNum]).end = charPtr;
			lineNum++;

			if (lineNum < text->lineCount)
				(text->lineArray[lineNum]).start = ++charPtr;
        }
	}

    return true;
}


static bool LinePrint(Line line, FILE* outputFile) 
{
    for (char* charPtr = line.start; charPtr <= line.end; charPtr++)
		if (fputc(*charPtr, outputFile) == EOF)
		{
			LOG_PRINT(ERROR, "Char <%c> on adress = %p can't be printed.",
								   *charPtr,        charPtr);
			return false;
		}

    return true;
}


static bool TextBufferCalloc(Text* text)
{
    if (text->textBuffer != NULL)
    {
        LOG_PRINT(WARNING, "You try to set buffer which has already set.");
        return false;
    }
                                                  // + 1 just in case
    text->textBuffer = (char*) calloc(text->textSize + 1, sizeof(char));
    if (text->textBuffer == NULL)
    {
        LOG_PRINT(ERROR, "Memory can't be allocated.");
        return false;
    }

    return true;
}


static bool TextSetBuffer(Text* text, FILE* textFile)
{
    if (!TextBufferCalloc(text))
        return false;
    
    const size_t readedChars = fread(text->textBuffer, sizeof(char), 
                                     text->textSize,   textFile);

    if(readedChars != text->textSize) 
    {
        LOG_PRINT(ERROR, "File can't be read correct. Try to use Linux.");
        return false;
    }

    return true;
}


static bool TextSetLineArrayAndCount(Text* text)
{
	TextSetLineCount(text);

	if (!TextSetLineArray(text))
		return false;

	return true;
}


static bool TextOpenFile(FILE** fileBuffer, const char* fileName, const char* openMode)
{
    assert(fileBuffer);
    assert(fileName);

    *fileBuffer = fopen(fileName, openMode);
    if (*fileBuffer == NULL)
	{
		LOG_PRINT(ERROR, "outputFile can't be opened");
		return false;
	}

    return true;
}


static bool TextPrintEmptyLines(FILE* outputFile)
{
    LOG_PRINT(INFO, "Empty lines printing starts.");

    const size_t EMPTY_LINE_COUNT = 30;

    for (size_t lineNum = 0; lineNum < EMPTY_LINE_COUNT; lineNum++)
        if (fputc('\n', outputFile) == EOF)
		{
			LOG_PRINT(ERROR, "Empty line can't be printed.");
			return false;
		}
    
    return true;
}


static bool TextPrintSeparatingLine(FILE* outputFile)
{
    LOG_PRINT(INFO, "Searating line printing starts.");

    const size_t SEPARATING_LINE_LENGTH = 90;

    for (size_t charNum = 0; charNum < SEPARATING_LINE_LENGTH; charNum++)
        if (fputc('~', outputFile) == EOF)
		{
			LOG_PRINT(ERROR, "Separate line's char can't be printed.");
			return false;
		}
    
    if (fputc('\n', outputFile) == EOF)
		{
			LOG_PRINT(ERROR, "Empty line can't be printed.");
			return false;
		}

    return true;
}