#include "parser.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define N 100

int main()
{

    FILE *f;
    f = fopen("input.txt", "r");
    // TOKEN_LIST TokenList= Scan(f);
    // PrintTokenList(TokenList);
    Parse(f);

    fclose(f);

   

    return 0;
}
