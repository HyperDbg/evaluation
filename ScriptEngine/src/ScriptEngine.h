#pragma once
#include <stdio.h>
#include "scanner.h"

//#define INPUT_FILE	0
//#define INPUT_STR	1 


//MATHLIBRARY_API void Parse(FILE* f);

char IsNoneTerminal(TOKEN Token);
char IsSemanticRule(TOKEN Token); 
char IsEqual(const TOKEN Token1, const TOKEN Token2);

int GetNonTerminalId(TOKEN Token);
int GetTerminalId(TOKEN Token);


__declspec(dllexport) void Parse(char* str);
