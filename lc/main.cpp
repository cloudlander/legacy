#include <stdio.h>
#include "errors.h"
#include "parser.h"
#include "symtable.h"
#include "quad.h"

extern FILE* yyin;
extern FILE* lexout;
extern FILE* quadout;
extern void Run();

int main(int argc, char** argv)
{
	if(argc!=2)
	{
		fprintf(stderr,"no input file");
		exit(-1);
	}
	
	yyin=fopen(argv[1],"r");
	if(yyin==NULL)
	{
		fprintf(stderr,"failed to open input file");
		exit(-1);
	}

	lexout=fopen("lex.txt","w");
	if(lexout==NULL)
	{
		fprintf(stderr,"failed to create lexical output file:lex.txt");
		exit(-1);
	}
	
	quadout=fopen("quad.txt","w");
	if(quadout==NULL)
	{
		fprintf(stderr,"failed to create quad output file:quad.txt");
		exit(-1);
	}
	
	
    InitScanner();
    InitParser();
    yyparse();

	fclose(lexout);
//	curSymTbl->dump();

	if(!ReportError::NumErrors())
	{
		for(int i=0;i<insts.NumElements();i++)
			insts.Nth(i)->Print();

		fclose(quadout);
		Run();
	}
	return 0;
}
