#ifndef _H_scanner
#define _H_scanner

#include <stdio.h>

#define MaxIdentLen 31    // Maximum length for identifiers

extern char *yytext;      // Text of lexeme just scanned


int yylex();              // Defined in the generated lex.yy.c file
void yyrestart(FILE *fp); // ditto


void InitScanner();                 // Defined in scanner.l user subroutines
const char *GetLineNumbered(int n); // ditto
 
#endif
