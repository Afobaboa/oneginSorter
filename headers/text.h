#ifndef TEXT_H
#define TEXT_H


#include <stddef.h>

/**
 * 
 */
struct Text
{
    char*  textBuffer;    /**<  */
    size_t textSize;      /**<  */
    size_t lineCount;     /**<  */
    char** linePointers;  /**<  */
};


/**
 * 
 */
bool TextSet(Text* textToSet, const char* textFileName);


/**
 * 
 */
void TextDelete(Text* text);


/**
 * 
 */
bool TextPrint(const Text* text, const char* outputFileName);


#endif // TEXT_H