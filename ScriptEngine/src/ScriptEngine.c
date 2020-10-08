#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ScriptEngine.h"
#include "scanner.h"
#include "parse_table.h"



void Parse(char* str)
{
    TOKEN_LIST Stack = NewTokenList();
    TOKEN_LIST MatchedStack = NewTokenList();

    TOKEN CurrentIn;
    TOKEN TopToken;

    int NonTerminalId;
    int TerminalId;
    int RuleId;
    char c;
    char t;

    

    //
    // End of File Token
    //
    TOKEN EndToken = NewToken();
    EndToken->Type = END_OF_STACK;
    strcpy(EndToken->Value, "$");

    //
    // Start Token
    //
    TOKEN StartToken = NewToken();
    strcpy(StartToken->Value, START_VARIABLE);
    StartToken->Type = NON_TERMINAL;

    Push(Stack, EndToken);
    Push(Stack, StartToken);


    
    c = sgetc(str);
    CurrentIn = Scan(str, &c);
   


    do
    {

        TopToken = Pop(Stack);



        printf("\nTop Token :\n");
        PrintToken(TopToken);
        printf("\nCurrent Input :\n");
        PrintToken(CurrentIn);
        printf("\n");

        if (TopToken->Type == NON_TERMINAL)
        {
            NonTerminalId = GetNonTerminalId(TopToken);
            if (NonTerminalId == -1)
            {
                printf("Error in Nonterminal ID\n");
                return;
            }
            TerminalId = GetTerminalId(CurrentIn);
            if (TerminalId == -1)
            {
                printf("Error in Terminal ID\n");
                return;
            }
            RuleId = ParseTable[NonTerminalId][TerminalId];
            if (RuleId == -1)
            {
                printf("Error in Parse Table\n");
                return;
            }

            // printf("Rule ID = %d\n", RuleId);


            // Push RHS Reversely into stack
            for (int i = RhsSize[RuleId] - 1; i >= 0; i--)
            {
                TOKEN Token = &Rhs[RuleId][i];


                if (Token->Type == EPSILON)
                    break;
                Push(Stack, Token);



            }


        }
        else if (TopToken->Type == SEMANTIC_RULE)
        {
            if (!strcmp(TopToken->Value, "@PUSH"))
            {
                TopToken = Pop(Stack);
                Push(MatchedStack, CurrentIn);
                CurrentIn = Scan(str, &c);

                // char t = getchar();
            }
            else
            {
                Push(MatchedStack, TopToken);
            }

        }
        else
        {
            if (!IsEqual(TopToken, CurrentIn))
            {
                printf("Error: Not Matched\n");
                return;
            }
            else
            {
                CurrentIn = Scan(str, &c);
                printf("matched...\n");

            }


        }
        PrintTokenList(Stack);
        printf("\n");



    } while (TopToken->Type != END_OF_STACK);

    PrintTokenList(MatchedStack);
}

char IsNoneTerminal(TOKEN Token)
{
    if (Token->Value[0] >= 'A' && Token->Value[0] <= 'Z')
        return 1;
    else
        return 0;
}
char IsSemanticRule(TOKEN Token)
{
    if (Token->Value[0] == '@')
        return 1;
    else 
        return 0;
}



int GetNonTerminalId(TOKEN Token)
{
    for(int i = 0; i < NONETERMINAL_COUNT; i++ )
    {
        if(!strcmp(Token->Value, NoneTerminalMap[i]))
            return i;
    }
    return -1;
}
int GetTerminalId(TOKEN Token)
{

    for(int i = 0; i < TERMINAL_COUNT; i++ )
    {
        if (Token->Type == HEX ) // TODO : Chack All Cases for this function
        {
            if(!strcmp("_hex", TerminalMap[i]))
                return i;
        }
        else if(Token->Type == ID)
        {
            if(!strcmp("_id", TerminalMap[i]))
            {
                return i;
            }
        }
        else if(Token->Type == REGISTER)
        {
            if(!strcmp("_register", TerminalMap[i]))
            {
                return i;
            }
        }
        else if(Token->Type == PSEUDO_REGISTER)
        {
            if(!strcmp("_pseudo_register", TerminalMap[i]))
            {
                return i;
            }
        }
        else if(Token->Type == DECIMAL)
        {
            if(!strcmp("_decimal", TerminalMap[i]))
            {
                return i;
            }
        }
        else if(Token->Type == FLOAT)
        {
            if(!strcmp("_float", TerminalMap[i]))
            {
                return i;
            }
        }
        else if(Token->Type == BINARY)
        {
            if(!strcmp("_binary", TerminalMap[i]))
            {
                return i;
            }
        }
        else if(Token->Type == OCTAL)
        {
            if(!strcmp("_octal", TerminalMap[i]))
            {
                return i;
            }
        }
       
        else // Keyword
        {
            if(!strcmp(Token->Value, TerminalMap[i]))
                return i;

            
        }
       
    }
    return -1;

}


char IsEqual(const TOKEN Token1, const TOKEN Token2)
{
    if(Token1->Type == Token2->Type)
    {
        if(Token1->Type == SPECIAL_TOKEN)
        {
            if(!strcmp(Token1->Value, Token2->Value))
            {
                return 1;
            }
        }
        else 
        {
            return 1;
        }
    }
    return 0;
    
}
