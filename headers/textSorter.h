#ifndef SORT_H
#define SORT_H


#include "textProcessor.h"


/**
 * 
 */
enum SORTING_MODES
{
    ALPHABET,   /**<  */
    RHYME       /**<  */
};
typedef enum SORTING_MODES sortMode_t;

/**
 * 
 */
void SortTextLines(const sortMode_t sortMode, Text* text);
               

#endif // SORT_H