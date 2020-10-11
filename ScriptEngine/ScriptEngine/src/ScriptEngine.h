#pragma once
#include <stdio.h>
#include "scanner.h"

// #define _SCRIPT_ENGINE_DBG_EN

#define SYMBOL_ID_TYPE  0
#define SYMBOL_NUM_TYPE 1
#define SYMBOL_REGISTER_TYPE 2
#define SYMBOL_PSEUDO_REG_TYPE 3
#define SYMBOL_SEMANTIC_RULE_TYPE 4


#define R0_MNEMONIC 0
#define RCX_MNEMONIC 16

#define INVALID -1


#define FUNC_OR 0
#define FUNC_XOR 1
#define FUNC_AND 2
#define FUNC_ASR 3
#define FUNC_ASL 4
#define FUNC_ADD 5
#define FUNC_SUB 6
#define FUNC_MUL 7
#define FUNC_DIV 8
#define FUNC_MOD 9
#define FUNC_POI 10
#define FUNC_DB 11
#define FUNC_DD 12
#define FUNC_DQ 13
#define FUNC_STR 14
#define FUNC_WSTR 15
#define FUNC_SIZEOF 16
#define FUNC_NOT 17
#define FUNC_NEG 18
#define FUNC_HI 19
#define FUNC_LOW 20


#define SYMBOL_BUFFER_INIT_SIZE 16

__declspec(dllexport) typedef struct SYMBOL
{
	long long unsigned Type;
	long long unsigned Value;
}SYMBOL, * PSYMBOL;

__declspec(dllexport) typedef struct SYMBOL_BUFFER
{
	PSYMBOL Head;
	unsigned int Pointer;
	unsigned int Size;

}SYMBOL_BUFFER, * PSYMBOL_BUFFER;

char IsNoneTerminal(TOKEN Token);
char IsSemanticRule(TOKEN Token); 
char IsEqual(const TOKEN Token1, const TOKEN Token2);

int GetNonTerminalId(TOKEN Token);
int GetTerminalId(TOKEN Token);


PSYMBOL NewSymbol(void);
void RemoveSymbol(PSYMBOL Symbol);
void PrintSymbol(PSYMBOL Symbol);

PSYMBOL_BUFFER NewSymbolBuffer(void);
void RemoveSymbolBuffer(PSYMBOL_BUFFER SymbolBuffer);
PSYMBOL_BUFFER PushSymbol(PSYMBOL_BUFFER SymbolBuffer, const PSYMBOL Symbol);
PSYMBOL PopSymbol(PSYMBOL_BUFFER SymbolBuffer);
void PrintSymbolBuffer(const PSYMBOL_BUFFER SymbolBuffer);

PSYMBOL ToSymbol(TOKEN Token);

PSYMBOL_BUFFER ExtractSymbolBuffer(TOKEN_LIST TokenList);






// Util Functions:
void SetType(unsigned long long* Val, unsigned char Type);
unsigned long long int DecimalToInt(char* str);
unsigned long long int HexToInt(char* str);
unsigned long long int OctalToInt(char* str);
unsigned long long int BinaryToInt(char* str);

unsigned long long int RegisterToInt(char* str);
unsigned long long int PseudoRegToInt(char* str);
unsigned long long int SemanticRuleToInt(char* str);




__declspec(dllexport) void ScriptEngineParse(char* str);

