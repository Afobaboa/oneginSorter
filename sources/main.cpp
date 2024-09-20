#include <stdio.h>

#include "../headers/logPrinter.h"
#include "../headers/textProcessor.h"
#include "../headers/textSorter.h"


//----------------------------------------------------------------------------------------


int main()
{
    LOG_OPEN();

    const char* inputFileName  = "texts/onegin.txt";
    const char* outputFileName = "texts/sortedOnegin.txt";
    Text        oneginText     = {};

    if (!TextSet(&oneginText, inputFileName) ||
        !TextCleanFile(outputFileName))
    {
        LOG_PRINT(ERROR, "Your file <%s> is broken. Maybe you wrote wrong file name",
                         outputFileName);

        TextDelete(&oneginText);
        return 1;
    }

    SortTextLines(ALPHABET, &oneginText);
    if (!TextPrintLines(&oneginText, outputFileName) ||
        !TextPrintSeparator(outputFileName))
    {
        LOG_PRINT(ERROR, "Your file <%s> is broken. Maybe you wrote wrong file name",
                         outputFileName);

        TextDelete(&oneginText);
        return 1;
    }

    SortTextLines(RHYME, &oneginText);
    if (!TextPrintLines(&oneginText, outputFileName) ||
        !TextPrintSeparator(outputFileName))
    {
        LOG_PRINT(ERROR, "Your file <%s> is broken. Maybe you wrote wrong file name",
                         outputFileName);

        TextDelete(&oneginText);
        return 1;
    }

    if (!TextPrintBuffer(&oneginText, outputFileName))
    {
        LOG_PRINT(ERROR, "Your file <%s> is broken. Maybe you wrote wrong file name",
                         outputFileName);

        TextDelete(&oneginText);
        return 1;
    }

    TextDelete(&oneginText);


    LOG_CLOSE();
    return 0;
}