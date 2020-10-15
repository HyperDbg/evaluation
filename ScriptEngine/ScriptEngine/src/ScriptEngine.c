#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include "ScriptEngine.h"
#include "scanner.h"
#include "parse_table.h"

/**
*
*
*
*/
void ScriptEngineParse(char *str)
{
    TOKEN_LIST Stack = NewTokenList();
    TOKEN_LIST MatchedStack = NewTokenList();

    TOKEN CurrentIn;
    TOKEN TopToken;
    TOKEN Op0; 
    TOKEN Op1;
    TOKEN Temp;

    int NonTerminalId;
    int TerminalId;
    int RuleId;
    char c;
    char t;

    TempValueCounter = 0;


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

#ifdef _SCRIPT_ENGINE_DBG_EN
        printf("\nTop Token :\n");
        PrintToken(TopToken);
        printf("\nCurrent Input :\n");
        PrintToken(CurrentIn);
        printf("\n");
#endif
        

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
                // Push(MatchedStack, TopToken);
                Op0 = Pop(MatchedStack);
                Op1 = Pop(MatchedStack);

                if (!strcmp(TopToken->Value, "@MOV"))
                {
                    PSYMBOL Op0Symbol = ToSymbol(Op0);
                    PSYMBOL Op1Symbol = ToSymbol(Op1);
                    PrintToken(Op0);
                    PrintSymbol(Op0Symbol);
                    // PrintSymbol(Op1Symbol);


                    printf("%s %s, %s\n", TopToken->Value, Op0->Value, Op1->Value);
                }
                else
                {
                    Temp = NewToken();
                    strcpy(Temp->Value, "t");
                    Temp->Type = TEMP;
                    Push(MatchedStack, Temp);
                    printf("%s t, %s, %s\n", TopToken->Value, Op0->Value, Op1->Value);
                }
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
                printf("\nCurrent Input :\n");
                PrintToken(CurrentIn);
                printf("\n");

#ifdef _SCRIPT_ENGINE_DBG_EN
                printf("matched...\n");
#endif
            }
        }
#ifdef _SCRIPT_ENGINE_DBG_EN
        PrintTokenList(Stack);
        printf("\n");
#endif

    } while (TopToken->Type != END_OF_STACK);

    PrintTokenList(MatchedStack);
}

/**
*
*
*
*/
char IsNoneTerminal(TOKEN Token)
{
    if (Token->Value[0] >= 'A' && Token->Value[0] <= 'Z')
        return 1;
    else
        return 0;
}

/**
*
*
*
*/
char IsSemanticRule(TOKEN Token)
{
    if (Token->Value[0] == '@')
        return 1;
    else
        return 0;
}

/**
*
*
*
*/
int GetNonTerminalId(TOKEN Token)
{
    for (int i = 0; i < NONETERMINAL_COUNT; i++)
    {
        if (!strcmp(Token->Value, NoneTerminalMap[i]))
            return i;
    }
    return -1;
}

/**
*
*
*
*/
int GetTerminalId(TOKEN Token)
{

    for (int i = 0; i < TERMINAL_COUNT; i++)
    {
        if (Token->Type == HEX)
        {
            if (!strcmp("_hex", TerminalMap[i]))
                return i;
        }
        else if (Token->Type == ID)
        {
            if (!strcmp("_id", TerminalMap[i]))
            {
                return i;
            }
        }
        else if (Token->Type == REGISTER)
        {
            if (!strcmp("_register", TerminalMap[i]))
            {
                return i;
            }
        }
        else if (Token->Type == PSEUDO_REGISTER)
        {
            if (!strcmp("_pseudo_register", TerminalMap[i]))
            {
                return i;
            }
        }
        else if (Token->Type == DECIMAL)
        {
            if (!strcmp("_decimal", TerminalMap[i]))
            {
                return i;
            }
        }
        else if (Token->Type == BINARY)
        {
            if (!strcmp("_binary", TerminalMap[i]))
            {
                return i;
            }
        }
        else if (Token->Type == OCTAL)
        {
            if (!strcmp("_octal", TerminalMap[i]))
            {
                return i;
            }
        }

        else // Keyword
        {
            if (!strcmp(Token->Value, TerminalMap[i]))
                return i;
        }
    }
    return -1;

}

/**
*
*
*
*/
char IsEqual(const TOKEN Token1, const TOKEN Token2)
{
    if (Token1->Type == Token2->Type)
    {
        if (Token1->Type == SPECIAL_TOKEN)
        {
            if (!strcmp(Token1->Value, Token2->Value))
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

/**
*
*
*
*/
PSYMBOL NewSymbol(void)
{
    PSYMBOL Symbol;
    Symbol = (PSYMBOL)malloc(sizeof(*Symbol));
    Symbol->Value = 0;
    Symbol->Type = 0;
    return Symbol;
}

/**
*
*
*
*/
void RemoveSymbol(PSYMBOL Symbol)
{
    free(Symbol);
    return;
}

/**
*
*
*
*/
SYMBOL_BUFFER NewSymbolBuffer(void)
{
    SYMBOL_BUFFER SymbolBuffer = (SYMBOL_BUFFER)malloc(SYMBOL_BUFFER_INIT_SIZE);
    return SymbolBuffer;
}

/**
* 
*
*
*/
void RemoveSymbolBuffer(SYMBOL_BUFFER SymbolBuffer)
{
    // TODO: When size of SymbolBuffer is more than 0, it must be freed
    free(SymbolBuffer);
    return;
}

/**
*
*
*
*/
SYMBOL_BUFFER PushSymbol(SYMBOL_BUFFER SymbolBuffer, const PSYMBOL Symbol)
{
    

    return SymbolBuffer;
}

/**
*
*
*
*/
PSYMBOL PopSymbol(SYMBOL_BUFFER SymbolBuffer)
{
   
    PSYMBOL ReadAddr = 0;

    return ReadAddr;
}

/**
*
*
*
*/
void PrintSymbol(PSYMBOL Symbol)
{
    printf("Type:%llx, Value:$llx\n", Symbol->Type, Symbol->Value);
}

/**
*
*
*
*/
void PrintSymbolBuffer(const SYMBOL_BUFFER SymbolBuffer)
{
    PSYMBOL Symbol;
    return;
}

PSYMBOL ToSymbol(TOKEN Token)
{
    static unsigned int IdCounter = 0;
    PSYMBOL Symbol = NewSymbol();
    switch (Token->Type)
    {
    case ID:
        Symbol->Value = IdCounter;
        SetType(&Symbol->Type, SYMBOL_ID_TYPE);
        return Symbol;
    case DECIMAL:
        Symbol->Value = DecimalToInt(Token->Value);
        SetType(&Symbol->Type, SYMBOL_NUM_TYPE);
        return Symbol;
    case HEX:
        Symbol->Value = HexToInt(Token->Value);
        SetType(&Symbol->Type, SYMBOL_NUM_TYPE);
        return Symbol;
    case OCTAL:
        Symbol->Value = OctalToInt(Token->Value);
        SetType(&Symbol->Type, SYMBOL_NUM_TYPE);
        return Symbol;

    case BINARY:
        Symbol->Value = BinaryToInt(Token->Value);
        SetType(&Symbol->Type, SYMBOL_NUM_TYPE);
        return Symbol;

    case REGISTER:
        Symbol->Value = RegisterToInt(Token->Value); // TODO: Implement RegisterToInt(char* str)
        SetType(&Symbol->Type, SYMBOL_REGISTER_TYPE);
        return Symbol;

    case PSEUDO_REGISTER:
        Symbol->Value = PseudoRegToInt(Token->Value); // TODO: Implement PseudoRegToInt(char* str)
        SetType(&Symbol->Type, SYMBOL_PSEUDO_REG_TYPE);
        return Symbol;

    case SEMANTIC_RULE:
        Symbol->Value = SemanticRuleToInt(Token->Value); // TODO: Implement SemanticRuleToInt(char* str)
        SetType(&Symbol->Type, SYMBOL_SEMANTIC_RULE_TYPE);
        return Symbol;
    case TEMP:
        Symbol->Value = TempValueCounter++; // TODO: Change it to an acceptable value  
        SetType(&Symbol->Type, SYMBOL_TEMP);
        return Symbol;

    default:
        // Raise Error
        printf("Error in Converting Token to Symbol!\n"); // TODO: Handle Error in a Error Handler Funtion
    }
}
void SetType(unsigned long long *Val, unsigned char Type)
{
    *Val = (unsigned long long int)Type;
}

unsigned long long int DecimalToInt(char *str)
{
    unsigned long long int acc = 0;
    for (int i = strlen(str) - 1; i >= 0; i--)
    {
        acc += (str[i] - '0');
        acc *= 10;
    }
    return acc;
}
unsigned long long int HexToInt(char *str)
{
    unsigned long long int acc = 0;
    for (int i = strlen(str) - 1; i >= 0; i--)
    {
        acc += (str[i] >= '0' && str[i] <= '9') ? str[i] - '0' : (str[i] >= 'a' && str[i] <= 'f') ? str[i] - 'a' : str[i] - 'A';
        acc <<= 4;
    }
    return acc;
}
unsigned long long int OctalToInt(char *str)
{
    unsigned long long int acc = 0;
    for (int i = strlen(str) - 1; i >= 0; i--)
    {
        acc += (str[i] - '0');
        acc <<= 3;
    }
    return acc;
}
unsigned long long int BinaryToInt(char *str)
{
    unsigned long long int acc = 0;
    for (int i = strlen(str) - 1; i >= 0; i--)
    {
        acc += (str[i] - '0');
        acc <<= 1;
    }
    return acc;
}

unsigned long long int RegisterToInt(char *str)
{

    if (!strcmp(str, "rcx"))
    {
        return RCX_MNEMONIC;
    }
    else if (!strcmp(str, "r0"))
    {
        return R0_MNEMONIC;
    }
    // ...
    // TODO: Handle all Registers
    return INVALID;
}
unsigned long long int PseudoRegToInt(char *str)
{
    // ...
    // TODO: Handle all Pseudo Registers
    return 0;
}
unsigned long long int SemanticRuleToInt(char *str)
{
    if (!strcmp(str, "@OR"))
    {
        return (unsigned long long int)FUNC_OR;
    }
    else if (!strcmp(str, "@XOR"))
    {
        return (unsigned long long int)FUNC_XOR;
    }
    else if (!strcmp(str, "@AND"))
    {
        return (unsigned long long int)FUNC_OR;
    }
    else if (!strcmp(str, "@ASR"))
    {
        return (unsigned long long int)FUNC_ASR;
    }
    else if (!strcmp(str, "@ASL"))
    {
        return (unsigned long long int)FUNC_ASR;
    }
    else if (!strcmp(str, "@ADD"))
    {
        return (unsigned long long int)FUNC_ADD;
    }
    else if (!strcmp(str, "@SUB"))
    {
        return (unsigned long long int)FUNC_SUB;
    }
    else if (!strcmp(str, "@MUL"))
    {
        return (unsigned long long int)FUNC_MUL;
    }
    else if (!strcmp(str, "@DIV"))
    {
        return (unsigned long long int)FUNC_DIV;
    }
    else if (!strcmp(str, "@MOD"))
    {
        return (unsigned long long int)FUNC_MOD;
    }
    else if (!strcmp(str, "@POI"))
    {
        return (unsigned long long int)FUNC_POI;
    }
    else if (!strcmp(str, "@DB"))
    {
        return (unsigned long long int)FUNC_DB;
    }
    else if (!strcmp(str, "@DD"))
    {
        return (unsigned long long int)FUNC_DD;
    }
    else if (!strcmp(str, "@DQ"))
    {
        return (unsigned long long int)FUNC_DQ;
    }
    else if (!strcmp(str, "@STR"))
    {
        return (unsigned long long int)FUNC_STR;
    }
    else if (!strcmp(str, "@WSTR"))
    {
        return (unsigned long long int)FUNC_WSTR;
    }
    else if (!strcmp(str, "@SIZEOF"))
    {
        return (unsigned long long int)FUNC_SIZEOF;
    }
    else if (!strcmp(str, "@NOT"))
    {
        return (unsigned long long int)FUNC_NOT;
    }
    else if (!strcmp(str, "@NEG"))
    {
        return (unsigned long long int)FUNC_NEG;
    }
    else if (!strcmp(str, "@HI"))
    {
        return (unsigned long long int)FUNC_HI;
    }
    else if (!strcmp(str, "@LOW"))
    {
        return (unsigned long long int)FUNC_LOW;
    }
}
