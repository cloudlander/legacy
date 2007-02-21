#ifndef _H_parser
#define _H_parser

#include "lex.h"            // for MaxIdentLen
#include "util.h"       	// because we use all these types

#ifndef YYBISON                 
#include "symtable.h"
#include "quad.h"
#include "y.tab.h"              
#endif

int yyparse();              // Defined in the generated y.tab.c file
void InitParser();          // Defined in parser.y

#endif
