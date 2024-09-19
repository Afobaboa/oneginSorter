#include <stdio.h>
#include "../headers/logPrinter.h"
#include "../headers/textProcessor.h"
#include "../headers/textSorter.h"


int main()
{
    LOG_OPEN();

    Text oneginText = {};
    TextSet(&oneginText, "texts/onegin.txt");

    SortTextLines(RHYME, &oneginText);
    TextPrint(&oneginText, "texts/oneginOutput.txt");

    TextDelete(&oneginText);

    LOG_CLOSE();
    return 0;
}