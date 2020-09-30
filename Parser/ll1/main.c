#include "parse_table.h"
#include <stdio.h>
int main()
{
    // for(int i = 0; i < RULES_COUNT; i++)
    // {
    //     printf("%s\n",Variables[i]);
    // }
    for (int i = 0; i < RULES_COUNT; i++)
    {
        for(int j = 0; j < MAX_RHS_LEN; j++)
            if( Rhs[i][j] != NULL) 
            {
                printf("%s ", Rhs[i][j]);
            }
        printf("\n");
    }
}