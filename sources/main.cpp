#include <stdio.h>
#include "../headers/text.h"
#include "../headers/sort.h"


int main()
{
    FILE* oneginText = fopen("texts/miniOnegin.txt", "r");
    fseek(oneginText, 0, SEEK_END);

    const size_t oneginSize = ftell(oneginText);

    rewind(oneginText);

    if (oneginSize == (size_t) -1L)
    {
        fprintf("%s: %s(): ERROR in ftell().", __FILE__, __FUNCTION__);
        fclose(oneginText);
        return 1;
    }

    Text oneginText = SetText("texts/miniOnegin.txt");

    rewind(oneginText);
    fclose(oneginText);


    return 0;
}


Text SetText = 