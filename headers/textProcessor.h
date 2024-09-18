#ifndef TEXT_PROCESSOR_H
#define TEXT_PROCESSOR_H


#include <stddef.h>

/**
 * This struct help you to
 * contain information about
 * lines.
 */
struct Line 
{
    char* start;    /**< Pointer to first char of line.               */
    char* end;      /**< Pointer to last char of line (this is '\n'). */
};


/**
 * This struct help you
 * to contain your text 
 * and provides an interface.
 */
struct Text
{
    char*  textBuffer;    /**< Pointer to first char of text. */
    size_t textSize;      /**< Count of text's chars.         */
    Line*  lineArray;     /**< Array of lines.                */
    size_t lineCount;     /**< Count of lines.                */
};


/**
 * This function set text.
 * 
 * @param textToSet    Text you want to set.
 * @param textFileName Name of file where you want to get text.
 * 
 * @return true if OK,
 * @return false if error.
 */
bool TextSet(Text* textToSet, const char* textFileName);


/**
 * This function delete Text.
 * Use it for safe memory cleaning.
 * 
 * @param text Text you want to delete.
 */
void TextDelete(Text* text);


/**
 * This function print text.
 * 
 * @param text           Text you want to print.
 * @param outputFileName Name of output file.
 * 
 * @return true if OK,
 * @return false if error.
 */
bool TextPrint(const Text* text, const char* outputFileName);


#endif // TEXT_PROCESSOR_H