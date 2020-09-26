#ifndef SCANNER_H
#define SCANNER_H
#include <stdio.h>

#define TOKEN_VALUE_MAX_LEN 100
#define TOKEN_LIST_INIT_SIZE 16
typedef enum TOKEN_TYPE
{
    ID,
    DECIMAL,
    FLOAT,
    HEX,
    OCTAL,
    BINARY,
    SPECIAL_TOKEN,
    KEYWORD,
    WHITE_SPACE,
    COMMENT,
    REGISTER, 
    PSEUDO_REGISTER,
    UNKNOWN
} TOKEN_TYPE;

typedef struct _TOKEN
{
    TOKEN_TYPE Type;
    char *Value;
} * TOKEN;

typedef struct _TOKEN_LIST
{
    TOKEN* Head;
    unsigned int Pointer;
    unsigned int Size;
} *TOKEN_LIST;

TOKEN NewToken(void);
TOKEN_LIST NewTokenList(void);
void PrintTokenList(TOKEN_LIST TokenList);
void PrintToken(TOKEN Token);
TOKEN_LIST Push(TOKEN_LIST TokenList, TOKEN Token);
void Append(TOKEN Token, char c);
TOKEN GetToken(char* c, FILE *f);
TOKEN_LIST Scan(FILE* f);
char IsHex(char c);
char IsDecimal(char c);
char IsLetter(char c);
char IsBinary(char c);
char IsOctal(char c);


#endif