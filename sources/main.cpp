#include <stdio.h>
#include "../headers/logPrinter.h"
#include "../headers/textProcessor.h"
#include "../headers/textSorter.h"


int main()
{
    LOG_OPEN();

    LOG_PRINT(INFO, "Main testing");

    Text oneginText = {};
    TextSet(&oneginText, "texts/miniOnegin.txt");

    SortTextLines(&oneginText);
    TextPrint(&oneginText, "texts/oneginQSortTest.txt");

    TextDelete(&oneginText);

    LOG_CLOSE();
    return 0;
}