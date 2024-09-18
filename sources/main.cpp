#include <stdio.h>
#include "../headers/logPrinter.h"
#include "../headers/textProcessor.h"
#include "../headers/textSorter.h"


int main()
{
    LOG_OPEN();

    Text oneginText = {};
    TextSet(&oneginText, "texts/test.txt");

    // SortTextLines(&oneginText);
    TextPrint(&oneginText, "texts/testOutput.txt");

    TextDelete(&oneginText);

    LOG_CLOSE();
    return 0;
}