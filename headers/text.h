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
    char*  linePointer;   /**<  */
};


/**
 * 
 */
bool SetText(Text* textToSet, const char* textFileName);


/**
 * 
 */
void PrintText(const Text* text, const char* outputFileName);


#endif // TEXT_H