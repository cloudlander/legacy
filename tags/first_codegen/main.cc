/* File: main.cc
 * -------------
 * This file defines the main() routine for the program and not much else.
 * You should not need to modify this file.
 */
 
#include <string.h>
#include <stdio.h>
#include "utility.h"
#include "parser.h"
#include "errors.h"

/* declare external subroutines */
#include "main.h"

// The pointer directed to a dynamic allocated place(a dynamic array) to 
// hold all idtable pointers. All idtables can be founded from this pointer
// stack <*SymTab_PTR>     mantance SymbleTable stack
// queue <*SymTab_PTR>     after sematic analysis all SymbleTable can be found from here

/*
 * Function: main()
 * ----------------
 * Entry point to the entire program.  We parse the command line and turn
 * on any debugging flags requested by the user when invoking the program.
 * InitScanner() is used to set up the scanner.
 * InitParser() is used to set up the parser. The call to yyparse() will
 * attempt to parse a complete program from the input. 
 */
int main(int argc, char *argv[])
{
    ParseCommandLine(argc, argv);

    InitScanner();
    InitParser();
    yyparse();
	BuildSymTable();
	GenCode();
    return (ReportError::NumErrors() == 0? 0 : -1);
}

