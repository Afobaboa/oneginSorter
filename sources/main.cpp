#include <stdio.h>
#include "../headers/textProcessor.h"
#include "../headers/sort.h"


int main()
{
    Text oneginText = {};
    TextSet(&oneginText, "texts/miniOnegin.txt");

    TextPrint(&oneginText, "oneginText.txt");

    SortTextLines(&oneginText);
    TextPrint(&oneginText, "miniOneginOutput.txt");

    TextDelete(&oneginText);
    return 0;
}