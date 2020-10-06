#ifndef PARSER_H
#define PARSER_H
#include <stdio.h>
#include "scanner.h"


void Parse(FILE* f);
char IsNoneTerminal(TOKEN Token);
char IsSemanticRule(TOKEN Token); 
char IsEqual(const TOKEN Token1, const TOKEN Token2);

int GetNonTerminalId(TOKEN Token);
int GetTerminalId(TOKEN Token);

#endif 