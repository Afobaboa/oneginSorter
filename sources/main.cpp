#include <stdio.h>
#include "../headers/logPrinter.h"
#include "../headers/textProcessor.h"
#include "../headers/textSorter.h"


int main()
{
    LOG_OPEN();

    const char* inputFile  = "texts/onegin.txt";
    const char* outputFile = "texts/sortedOnegin.txt";
    Text        oneginText = {};

    TextSet(&oneginText, inputFile);
    TextCleanFile(outputFile);

    SortTextLines(ALPHABET, &oneginText);
    TextPrintLines(&oneginText, outputFile);
    TextPrintSeparator(outputFile);

    SortTextLines(RHYME, &oneginText);
    TextPrintLines(&oneginText, outputFile);
    TextPrintSeparator(outputFile);

    TextPrintBuffer(&oneginText, outputFile);

    TextDelete(&oneginText);


    LOG_CLOSE();
    return 0;
}