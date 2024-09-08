#include <stdio.h>
#include "../headers/text.h"
#include "../headers/sort.h"


int main()
{
    Text oneginText = {};
    TextSet(&oneginText, "texts/miniOnegin.txt");

    TextPrint(&oneginText, "miniOneginOutput.txt");

    TextDelete(&oneginText);
    return 0;
}