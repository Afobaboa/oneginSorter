#ifndef SORT_H
#define SORT_H


//----------------------------------------------------------------------------------------


#include "textProcessor.h"


//----------------------------------------------------------------------------------------


/**
 * There are modes of line sorting.
 */
enum SORTING_MODES
{
    ALPHABET,   /**< Sort lines by alphabet order. */
    RHYME       /**< Sort lines by rhyme order.    */
};
typedef enum SORTING_MODES sortMode_t;


//----------------------------------------------------------------------------------------


/**
 * This function sort your text in
 * order wich you choose.
 * 
 * @param sortMode Mode of sorting.
 * @param text     Pointr to your text.
 */
void SortTextLines(const sortMode_t sortMode, Text* text);


//----------------------------------------------------------------------------------------
               

#endif // SORT_H