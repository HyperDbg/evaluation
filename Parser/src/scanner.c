#include "scanner.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
/**
 * 
 */
TOKEN NewToken()
{
    TOKEN Token;
    Token = (TOKEN)malloc(sizeof(*Token));
    Token->Value = (char *)calloc(TOKEN_VALUE_MAX_LEN, sizeof(char));
    strcpy(Token->Value, "");
    Token->Type = UNKNOWN;
    return Token;
}
/**
 * 
 */
TOKEN_LIST NewTokenList(void)
{
    TOKEN Token;
    TOKEN_LIST TokenList;
    TokenList = (TOKEN_LIST)malloc(sizeof(*TokenList));
    TokenList->Pointer = 0;
    TokenList->Size = TOKEN_LIST_INIT_SIZE;
    TokenList->Head = (TOKEN *)malloc(TokenList->Size * sizeof(TOKEN));
}
/**
 * 
 */
TOKEN_LIST Push(TOKEN_LIST TokenList, TOKEN Token)
{
    //
    // Calculate address to write new token
    //
    uintptr_t Head = (uintptr_t)TokenList->Head;
    uintptr_t Pointer = (uintptr_t)TokenList->Pointer;
    TOKEN *WriteAddr = (TOKEN *)(Head + Pointer * sizeof(TOKEN));

    //
    // Write Token to appropriate address in TokenList
    //
    *WriteAddr = Token;

    //
    // Update Pointer
    //
    TokenList->Pointer++;

    //
    // Handle overflow
    //
    if (Pointer == TokenList->Size - 1)
    {
        //
        // Allocate a new buffer for string list with doubled length
        //
        TOKEN *NewHead = (TOKEN *)malloc(2 * TokenList->Size * sizeof(TOKEN));

        //
        // Copy old buffer to new buffer
        //
        memcpy(NewHead, TokenList->Head, TokenList->Size * sizeof(TOKEN));

        //
        // Free old buffer
        //
        free(TokenList->Head);

        //
        // Update Head and size of TokenList
        //
        TokenList->Size = TokenList->Size * 2;
        TokenList->Head = NewHead;
    }

    return TokenList;
}
/**
 * 
 */
 
TOKEN Pop(TOKEN_LIST TokenList)
{
    //
    // Calculate address to read most recent token
    //
    if (TokenList->Pointer > 0)
        TokenList->Pointer--;
    uintptr_t Head = (uintptr_t)TokenList->Head;
    uintptr_t Pointer = (uintptr_t)TokenList->Pointer;
    TOKEN *ReadAddr = (TOKEN *)(Head + Pointer * sizeof(TOKEN));

    
    return *ReadAddr;
}

TOKEN Top(TOKEN_LIST TokenList)
{
    //
    // Calculate address to read most recent token
    //
    uintptr_t Head = (uintptr_t)TokenList->Head;
    uintptr_t Pointer = (uintptr_t)TokenList->Pointer-1;
    TOKEN *ReadAddr = (TOKEN *)(Head + Pointer * sizeof(TOKEN));

    return *ReadAddr;
}
/**
 * 
 */
void PrintTokenList(TOKEN_LIST TokenList)
{
    TOKEN Token = NewToken();
    for (uintptr_t i = 0; i < TokenList->Pointer; i++)
    {
        Token = *(TokenList->Head + i);
        PrintToken(Token);
    }
}

/**
 * 
 * 
 */
void PrintToken(TOKEN Token)
{
    if (Token->Type == WHITE_SPACE)
    {
        printf("< :");
    }
    else
    {
        printf("<'%s' : ", Token->Value);
    }

    switch (Token->Type)
    {
    case ID:
        printf(" ID>\n");
        break;
    case DECIMAL:
        printf(" DECIMAL>\n");
        break;
    case FLOAT:
        printf(" FLOAT>\n");
        break;
    case HEX:
        printf(" HEX>\n");
        break;
    case OCTAL:
        printf(" OCTAL>\n");
        break;
    case BINARY:
        printf(" BINARY>\n");
        break;
    case SPECIAL_TOKEN:
        printf(" SPECIAL_TOKEN>\n");
        break;
    case KEYWORD:
        printf(" KEYWORD>\n");
        break;
    case WHITE_SPACE:
        printf(" WHITE_SPACE>\n");
        break;
    case COMMENT:
        printf(" COMMENT>\n");
        break;
    case REGISTER:
        printf(" REGISTER>\n");
        break;
    case PSEUDO_REGISTER:
        printf(" PSEUDO_REGISTER>\n");
        break;
    case SEMANTIC_RULE:
        printf(" SEMANTIC_RULE>\n");
        break;
    case NON_TERMINAL:
        printf(" NON_TERMINAL>\n");
        break;
    case END_OF_STACK:
        printf(" END_OF_STACK>\n");
        break;
    case UNKNOWN:
        printf(" UNKNOWN>\n");
        break;

    default:
        printf(" ERROR>\n");
        break;
    }
}
/**
 * 
 */
void Append(TOKEN Token, char c)
{
    strncat(Token->Value, &c, 1);
}
/**
 * 
 */
TOKEN GetToken(char *c, FILE *f)
{
    static unsigned char WaitForID = 1;
    TOKEN Token = NewToken();

    switch (*c)
    {
    case '+':
        *c = fgetc(f);
        if (*c == '+')
        {
            strcpy(Token->Value, "++");
            Token->Type = SPECIAL_TOKEN;
            *c = fgetc(f);
            return Token;
        }
        else if (*c == '=')
        {
            strcpy(Token->Value, "+=");
            Token->Type = SPECIAL_TOKEN;
            *c = fgetc(f);
            return Token;
        }
        else
        {
            strcpy(Token->Value, "+");
            Token->Type = SPECIAL_TOKEN;
            return Token;
        }
    case '-':
        *c = fgetc(f);
        if (*c == '-')
        {
            strcpy(Token->Value, "--");
            Token->Type = SPECIAL_TOKEN;
            *c = fgetc(f);
            return Token;
        }
        else if (*c == '=')
        {
            strcpy(Token->Value, "-=");
            Token->Type = SPECIAL_TOKEN;
            *c = fgetc(f);
            return Token;
        }
        else
        {
            strcpy(Token->Value, "-");
            Token->Type = SPECIAL_TOKEN;
            return Token;
        }
    case '*':
        *c = fgetc(f);
        if (*c == '=')
        {
            strcpy(Token->Value, "*=");
            Token->Type = SPECIAL_TOKEN;
            *c = fgetc(f);
            return Token;
        }
        else
        {
            strcpy(Token->Value, "*");
            Token->Type = SPECIAL_TOKEN;
            return Token;
        }
    case '/':
        *c = fgetc(f);
        if (*c == '=')
        {
            strcpy(Token->Value, "/=");
            Token->Type = SPECIAL_TOKEN;
            *c = fgetc(f);

            return Token;
        }
        else if (*c == '/')
        {
            do
            {
                *c = fgetc(f);
            } while (*c != '\n' && *c != EOF);

            Token->Type = COMMENT;
            *c = fgetc(f);
            return Token;
        }
        else if (*c = '*')
        {
            do
            {
                *c = fgetc(f);
                if (*c == '*')
                {
                    *c = fgetc(f);
                    if (*c == '/')
                    {
                        Token->Type = COMMENT;
                        *c = fgetc(f);
                        return Token;
                    }
                }
                if (*c == EOF)
                    break;
            } while (1);

            Token->Type = UNKNOWN;
            *c = fgetc(f);
            return Token;
        }
        else
        {
            strcpy(Token->Value, "/");
            Token->Type = SPECIAL_TOKEN;
            return Token;
        }

    case '=':
        strcpy(Token->Value, "=");
        Token->Type = SPECIAL_TOKEN;
        *c = fgetc(f);
        return Token;

    case ',':
        strcpy(Token->Value, ",");
        Token->Type = SPECIAL_TOKEN;
        *c = fgetc(f);
        return Token;

    case ';':
        strcpy(Token->Value, ";");
        Token->Type = SPECIAL_TOKEN;
        WaitForID = 1;
        *c = fgetc(f);
        return Token;

    case ':':
        strcpy(Token->Value, ":");
        Token->Type = SPECIAL_TOKEN;
        *c = fgetc(f);
        return Token;

    case '(':
        strcpy(Token->Value, "(");
        Token->Type = SPECIAL_TOKEN;
        *c = fgetc(f);
        return Token;
    case ')':
        strcpy(Token->Value, ")");
        Token->Type = SPECIAL_TOKEN;
        *c = fgetc(f);
        return Token;
    case '{':
        strcpy(Token->Value, "{");
        Token->Type = SPECIAL_TOKEN;
        *c = fgetc(f);
        return Token;
    case '}':
        strcpy(Token->Value, "}");
        Token->Type = SPECIAL_TOKEN;
        *c = fgetc(f);
        return Token;
    case '|':
        strcpy(Token->Value, "|");
        Token->Type = SPECIAL_TOKEN;
        *c = fgetc(f);
        return Token;
    case '@':
        *c = fgetc(f);
        if (IsLetter(*c))
        {
            while (IsLetter(*c) || IsDecimal(*c))
            {
                Append(Token, *c);
                *c = fgetc(f);
            }
            Token->Type = REGISTER;
            return Token;
        }

    case '$':
        *c = fgetc(f);
        if (IsLetter(*c))
        {
            while (IsLetter(*c) || IsDecimal(*c))
            {
                Append(Token, *c);
                *c = fgetc(f);
            }
            Token->Type = PSEUDO_REGISTER;
            return Token;
        }

    case '.':
        *c = fgetc(f);
        if (IsHex(*c))
        {
        }
        else
        {
        }

    case ' ':
    case '\t':
        strcpy(Token->Value, "");
        Token->Type = WHITE_SPACE;
        *c = fgetc(f);
        return Token;
    case '\n':
        strcpy(Token->Value, "");
        Token->Type = WHITE_SPACE;
        *c = fgetc(f);
        WaitForID = 1;
        return Token;

    case '0':
        *c = fgetc(f);
        if (*c == 'x')
        {
            *c = fgetc(f);
            while (IsHex(*c) || *c == '`')
            {
                if (*c != '`')
                    Append(Token, *c);
                *c = fgetc(f);
            }
            Token->Type = HEX;
            return Token;
        }
        else if (*c == 'o')
        {
            *c = fgetc(f);
            while (IsOctal(*c) || *c == '`')
            {
                if (*c != '`')
                    Append(Token, *c);
                *c = fgetc(f);
            }
            Token->Type = OCTAL;
            return Token;
        }
        else if (*c == 'n')
        {
            *c = fgetc(f);
            while (IsDecimal(*c) || *c == '`')
            {
                if (*c != '`')
                    Append(Token, *c);
                *c = fgetc(f);
            }
            Token->Type = DECIMAL;
            return Token;
        }
        else if (*c == 'y')
        {
            *c = fgetc(f);
            while (IsBinary(*c) || *c == '`')
            {
                if (*c != '`')
                    Append(Token, *c);
                *c = fgetc(f);
            }
            Token->Type = BINARY;
            return Token;
        }

        else if (IsHex(*c))
        {
            do
            {
                if (*c != '`')
                    Append(Token, *c);
                *c = fgetc(f);
            } while (IsHex(*c) || *c == '`');
            if (*c == '.')
            {
                Append(Token, *c);
                *c = getc(f);
                do
                {
                    if (*c != '`')
                        Append(Token, *c);
                    *c = fgetc(f);
                } while (IsHex(*c) || *c == '`');

                Token->Type = FLOAT;
                return Token;
            }
            else
            {
                Token->Type = HEX;
                return Token;
            }
        }
        else if (*c == '.')
        {
            Append(Token, *c);
            *c = getc(f);
            do
            {
                if (*c != '`')
                    Append(Token, *c);
                *c = fgetc(f);
            } while (IsHex(*c) || *c == '`');

            Token->Type = FLOAT;
            return Token;
        }
        else
        {
            strcpy(Token->Value, "0");
            Token->Type = HEX;
            return Token;
        }

    default:
        if (IsLetter(*c) || *c == '_')
        {
            while (IsLetter(*c) || *c == '_' || IsDecimal(*c))
            {
                Append(Token, *c);
                *c = fgetc(f);
            }

            if (WaitForID)
            {
                Token->Type = ID;
                WaitForID = 0;
            }
            else
            {
                Token->Type = KEYWORD;
            }

            return Token;
        }
        else if (IsHex(*c))
        {
            do
            {
                if (*c != '`')
                    Append(Token, *c);
                *c = fgetc(f);
            } while (IsHex(*c) || *c == '`');
            Token->Type = HEX;
            return Token;
        }

        Token->Type = UNKNOWN;
        *c = fgetc(f);
        return Token;
    }
    return Token;
}
/**
 * 
 */
TOKEN Scan(FILE* f)
{
    char c = fgetc(f);
    TOKEN Token = NewToken();

    while (1)
    {
        Token = GetToken(&c, f);
        if (c == EOF)
        {
            Token->Type = END_OF_STACK;
            strcpy(Token->Value,"$");
            return Token;
        }            
            
        else if (Token->Type == WHITE_SPACE)
        {
            continue;
        }  
        else 
        {
            return Token;
        }
        //
    }
}

char IsHex(char c)
{
    if ((c >= '0' && c <= '9') || (c >= 'a' && c <= 'f') || (c >= 'A' && c <= 'F'))
        return 1;
    else
        return 0;
}
char IsDecimal(char c)
{
    if (c >= '0' && c <= '9')
        return 1;
    else
        return 0;
}
char IsLetter(char c)
{
    if ((c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z'))
        return 1;
    else
    {
        return 0;
    }
}
char IsBinary(char c)
{
    if (c == '0' || c == '1')
        return 1;
    else
    {
        return 0;
    }
}
char IsOctal(char c)
{
    if (c >= '0' && c <= '7')
        return 1;
    else
        return 0;
}