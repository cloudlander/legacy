/* File: parser.y
 * --------------
 * Yacc input file to generate the parser for L compiler.
 *
 */

%{

#include "lex.h" // for yylex
#include "parser.h"
#include "errors.h"
#include "symtable.h"
#include "quad.h"

void yyerror(char *msg); // standard error-handling routine
extern List<int> forwardTargets;
FILE* lexout;

%}
%token	 T_Program T_Begin T_End T_Const T_Array
%token   T_Bool T_Int T_Double
%token   T_LessEqual T_GreaterEqual T_Equal T_NotEqual T_Assign
%token   T_And T_Or T_Not
%token   T_While T_Do T_Until T_Repeat T_For T_To T_If T_Then T_Else
%token   T_Read T_Write

%union {
    int integerConstant;
    bool boolConstant;
    char *stringConstant;
    double doubleConstant;
    char identifier[MaxIdentLen+1]; // +1 for terminating null

	ValueType vtype;
	List<char*>* identList;
	List<Symbol*>* symList;
	List<Operand*>* exprList;
	List<int>* dims;
	Operand* opr;
	Quad* quad;
}

%token   <identifier> T_Identifier 
%token   <integerConstant> T_IntConstant
%token   <doubleConstant> T_DoubleConstant
%token   <boolConstant> T_BoolConstant
%token   <stringConstant> T_StringConstant 

%token <integerConstant> T_To T_While T_Repeat
%token <quad> T_For T_Do T_If T_Else

%type <vtype> Type;
%type <identList> IdentList;
%type <symList> ArrayDeclList;
%type <dims> Dims;
%type <opr> LValue Expr;
%type <exprList> ExprList;
%type <quad> IFTHEN

%nonassoc T_Then
%left ','
%left T_Assign
%left T_Or 
%left T_And
%left T_Equal T_NotEqual
%left '<' '>' T_LessEqual T_GreaterEqual
%left '-' '+'
%left '*' '/'
%right T_Not
%left '.' '[' 
%nonassoc UMINUS
%nonassoc Lower_Else
%nonassoc T_Else

%%
/*
 * All productions and actions should be placed between the start and stop
 * %% markers which delimit the Rules section.
 */
	 
Program : T_Program  T_Identifier ';' 
		  {
			 @1;
			 curSymTbl=new SymTable(NULL,"GLOBAL");
		  }
		  DeclList T_Begin StmtBlock T_End
		  { 
				insts.Append(new Quad(NULL,Quad::Halt));	     	
          }
        ;
          
DeclList  :    Decl ';' DeclList       
          |    Decl                 
          ;

Decl      :VariableDecl
		  |ConstDecl 
          ;

Const		:  T_Identifier T_Equal T_IntConstant	
	   				{
						curSymTbl->Append(strdup($1),GlobalVar)->SetConst($3);
						
					}
		 |	   T_Identifier T_Equal T_DoubleConstant
	   				{
						curSymTbl->Append(strdup($1),GlobalVar)->SetConst($3);
						
					}
		 |	   T_Identifier T_Equal T_BoolConstant
	   				{
						curSymTbl->Append(strdup($1),GlobalVar)->SetConst($3);
						
					}
		
ConstDeclList:	Const
		 |		Const ',' ConstDeclList

ConstDecl:		T_Const ConstDeclList	

VariableDecl	:Type ':' IdentList	
			 	{ 
					for(int i=0;i<$3->NumElements();i++)
					{
						curSymTbl->Append($3->Nth(i),GlobalVar)->SetVType($1);
					}
				}
		 |	 Type ':' T_Array ArrayDeclList
			 	{ 
					for(int i=0;i<$4->NumElements();i++)
					{
						$4->Nth(i)->SetVType($1);
						curSymTbl->Append($4->Nth(i));
					}
				}
		;
		  
Dims	:	T_IntConstant	{ $$=new List<int>; $$->Append($1);}
		 |	Dims ',' T_IntConstant {$$=$1;$$->Append($3);}


ArrayDeclList: T_Identifier '[' Dims ']' ',' ArrayDeclList
			 {
				$$=$6;
				$$->Append(new Symbol(strdup($1),GlobalVar,$3));
			 }
	    |      T_Identifier '[' Dims ']'
			{
				$$=new List<Symbol*>;
				$$->Append(new Symbol(strdup($1),GlobalVar,$3));
			}

Type		:T_Bool		{ $$=Boolean;  }
		|T_Int		{ $$=Integer; }
		|T_Double	{ $$=Double; }
		;

StmtBlock	:Stmt   
		|	Stmt ';' StmtBlock 
		;

Stmt	: Expr               
		| T_Begin StmtBlock	T_End	
		|IfStmt			
		|WhileStmt		
		|RepeatStmt
		|ForStmt		
		|T_Read IdentList
			{
				for(int i=0;i<$2->NumElements();i++)
				{
					Symbol* s=curSymTbl->Find($2->Nth(i));
					if(!s)
					{
						ReportError::Formatted(& @1, "variable not declared");
						continue;
					}
					insts.Append(new Quad(NULL,Quad::Read,new Operand(s)));
				}
			}
		|T_Write ExprList	
			{
				for(int i=0;i<$2->NumElements();i++)
				{
					Operand* s=$2->Nth(i);
					if(!s)
					{
						ReportError::Formatted(& @1,"Unknow Error");
						continue;
					}
					insts.Append(new Quad(NULL,Quad::Write,s));
				}
			}
		;
IFTHEN	:	
	   		T_If Expr T_Then
	   		{
				if($2->GetType()!=Boolean)
				{
					ReportError::Formatted(& @2,"type conflict");
					$$=NULL;
				}
				else		
				{
					Quad* ifz=new Quad(NULL,Quad::GoFalse,$2);
					insts.Append(ifz);
					$$=ifz;
					$$->target=-1;		// back patching (tricky)
				}
			} //%prec UMINUS 
			;
IfStmt		:
//			T_If Expr T_Then 
			IFTHEN
			Stmt %prec Lower_Else	
			{
				if($1)
				{
					int t=insts.NumElements();
					$1->SetTarget(t);
				}
			}
			|
//			|T_If  Expr 
//			T_Then
			IFTHEN
			Stmt 
			T_Else
			{
				if($1)
				{
					Quad* go=new Quad(NULL,Quad::Goto);
					insts.Append(go);
					$1->target=insts.NumElements();
					go->target=(int)$1;
					$1=go;
				}
			}
			Stmt
			{
				if($1)
				{
					int t=insts.NumElements();
					$1->SetTarget(t);
					((Quad*)($1->target))->SetTarget(((Quad*)($1->target))->target);
				}
			}
		;

RepeatStmt	:T_Repeat 
		  	{	
				$1=insts.NumElements(); 
				// tricy, save for next labeling
				forwardTargets.Append($1);
			} 
			StmtBlock
			T_Until Expr  
			{
				if($5->GetType()!=Boolean)
				{
					ReportError::Formatted(& @5,"type conflict");
				}
				else		
				{
					Quad* ifz=new Quad(NULL,Quad::GoFalse,$5);
					ifz->SetTarget($1);
					insts.Append(ifz);
				}
			}
		;
WhileStmt	:T_While 
		  	{	
				$1=insts.NumElements(); 
				// tricy, save for next labeling
				forwardTargets.Append($1);
			} 
			Expr T_Do 
			{
				if($3->GetType()!=Boolean)
				{
					ReportError::Formatted(& @2,"type conflict");
					$4=NULL;
				}
				else		
				{
					Quad* ifz=new Quad(NULL,Quad::GoFalse,$3);
					insts.Append(ifz);
					$4=ifz;
				}
			}
			Stmt 
			{
				if($4)
				{
					Quad* go=new Quad(NULL,Quad::Goto);
					go->SetTarget($1);
					insts.Append(go);
					$4->SetTarget(insts.NumElements());
				}
			}
		;

ForStmt		:T_For T_Identifier T_Assign Expr T_To  Expr T_Do 
			{
				Symbol* s=curSymTbl->Find($2);
				if(s)
				{
					if(s->GetValueType() != $4->GetType() ||
					   s->GetValueType() != $6->GetType() )
					{
						ReportError::Formatted(& @2,"type conflict");
					}
					insts.Append(new Quad(new Operand(s),Quad::Assign,$4));
					Operand* t=new Operand(Boolean,insts.NumElements());
					Quad* comp=new Quad(t,Quad::LessThan,new Operand(s),$6);
					comp->SetLabel(insts.NumElements());
					$5=insts.NumElements();
					insts.Append(comp);
					Quad* ifz=new Quad(NULL,Quad::GoFalse,t);
					insts.Append(ifz);
					$1=ifz;
				}
				else
				{
					ReportError::Formatted(& @2,"variable not declared");
				}
			}		 
			Stmt 
			{
				Symbol* s=curSymTbl->Find($2);
				if(s)
				{
					if(s->GetValueType() == $4->GetType() &&
					   s->GetValueType() == $6->GetType() )
					{
						Operand* id=new Operand(s);
						insts.Append(new Quad(id,Quad::Add,id,new Operand(1)));
						Quad* go=new Quad(NULL,Quad::Goto);
						go->SetTarget($5);
						insts.Append(go);
						$1->SetTarget(insts.NumElements());
					}
				}
			}
		;

Expr		:T_IntConstant		{ $$=new Operand($1);	}
	  	|T_DoubleConstant		{ $$=new Operand($1);	}
		|T_BoolConstant			{ $$=new Operand($1);	}
		|T_StringConstant		{ $$=new Operand(strdup($1));	}
		|Expr '+' Expr			
			{ 	
				ValueType tl=$1->GetType();
				ValueType tr=$3->GetType();
				if(tl==ERROR || tr==ERROR)
				{
					$$=new Operand(ERROR);
				}
				else
				{
					if(tl!=tr || tl==Boolean)
					{
						ReportError::Formatted(& @2,"type conflict");
						$$=new Operand(ERROR);
					}
					$$=new Operand(tr,insts.NumElements());
					insts.Append(new Quad($$,Quad::Add,$1,$3));
				}
			}
		|Expr '*' Expr		
			{ 	
				ValueType tl=$1->GetType();
				ValueType tr=$3->GetType();
				if(tl==ERROR || tr==ERROR)
				{
					$$=new Operand(ERROR);
				}
				else
				{
					if(tl!=tr || tl==Boolean)
					{
						ReportError::Formatted(& @2,"type conflict");
						$$=new Operand(ERROR);
					}
					$$=new Operand(tr,insts.NumElements());
					insts.Append(new Quad($$,Quad::Mul,$1,$3));
				}
			}
		|Expr '/' Expr		
			{ 	
				ValueType tl=$1->GetType();
				ValueType tr=$3->GetType();
				if(tl==ERROR || tr==ERROR)
				{
					$$=new Operand(ERROR);
				}
				else
				{
					if(tl!=tr || tl==Boolean)
					{
						ReportError::Formatted(& @2,"type conflict");
						$$=new Operand(ERROR);
					}
					$$=new Operand(tr,insts.NumElements());
					insts.Append(new Quad($$,Quad::Div,$1,$3));
				}
			}
		|Expr '>' Expr		
			{ 	
				ValueType tl=$1->GetType();
				ValueType tr=$3->GetType();
				if(tl==ERROR || tr==ERROR)
				{
					$$=new Operand(ERROR);
				}
				else
				{
					if(tl!=tr || tl==Boolean)
					{
						ReportError::Formatted(& @2,"type conflict");
						$$=new Operand(ERROR);
					}
					$$=new Operand(Boolean,insts.NumElements());
					insts.Append(new Quad($$,Quad::Less,$3,$1));
				}
			}
		|Expr '<' Expr		
			{ 	
				ValueType tl=$1->GetType();
				ValueType tr=$3->GetType();
				if(tl==ERROR || tr==ERROR)
				{
					$$=new Operand(ERROR);
				}
				else
				{
					if(tl!=tr || tl==Boolean)
					{
						ReportError::Formatted(& @2,"type conflict");
						$$=new Operand(ERROR);
					}
					$$=new Operand(Boolean,insts.NumElements());
					insts.Append(new Quad($$,Quad::Less,$1,$3));
				}
			}
		|'-'Expr %prec UMINUS
			{ 	
				ValueType tl=$2->GetType();
				if(tl==ERROR)
				{
					$$=new Operand(ERROR);
				}
				else
				{
					if(tl==Boolean)
					{
						ReportError::Formatted(& @2,"type conflict");
						$$=new Operand(ERROR);
					}
					$$=new Operand(tl,insts.NumElements());
					insts.Append(new Quad($$,Quad::Sub,new Operand(0),$2));
				}
			}
		|Expr '-' Expr          
			{ 	
				ValueType tl=$1->GetType();
				ValueType tr=$3->GetType();
				if(tl==ERROR || tr==ERROR)
				{
					$$=new Operand(ERROR);
				}
				else
				{
					if(tl!=tr || tl==Boolean)
					{
						ReportError::Formatted(& @2,"type conflict");
						$$=new Operand(ERROR);
					}
					$$=new Operand(tr,insts.NumElements());
					insts.Append(new Quad($$,Quad::Sub,$1,$3));
				}
			}
		|Expr T_LessEqual Expr	
			{ 	
				ValueType tl=$1->GetType();
				ValueType tr=$3->GetType();
				if(tl==ERROR || tr==ERROR)
				{
					$$=new Operand(ERROR);
				}
				else
				{
					if(tl!=tr || tl==Boolean)
					{
						ReportError::Formatted(& @2,"type conflict");
						$$=new Operand(ERROR);
					}
					$$=new Operand(Boolean,insts.NumElements());
					insts.Append(new Quad($$,Quad::LessThan,$1,$3));
				}
			}
		|Expr T_GreaterEqual Expr	
			{ 	
				ValueType tl=$1->GetType();
				ValueType tr=$3->GetType();
				if(tl==ERROR || tr==ERROR)
				{
					$$=new Operand(ERROR);
				}
				else
				{
					if(tl!=tr || tl==Boolean)
					{
						ReportError::Formatted(& @2,"type conflict");
						$$=new Operand(ERROR);
					}
					$$=new Operand(Boolean,insts.NumElements());
					insts.Append(new Quad($$,Quad::LessThan,$3,$1));
				}
			}
		|Expr T_Equal Expr	
			{ 	
				ValueType tl=$1->GetType();
				ValueType tr=$3->GetType();
				if(tl==ERROR || tr==ERROR)
				{
					$$=new Operand(ERROR);
				}
				else
				{
					if(tl!=tr)
					{
						ReportError::Formatted(& @2,"type conflict");
						$$=new Operand(ERROR);
					}
					$$=new Operand(Boolean,insts.NumElements());
					insts.Append(new Quad($$,Quad::Eq,$1,$3));
				}
			}
		|Expr T_NotEqual Expr	
			{ 	
				ValueType tl=$1->GetType();
				ValueType tr=$3->GetType();
				if(tl==ERROR || tr==ERROR)
				{
					$$=new Operand(ERROR);
				}
				else
				{
					if(tl!=tr)
					{
						ReportError::Formatted(& @2,"type conflict");
						$$=new Operand(ERROR);
					}
					$$=new Operand(Boolean,insts.NumElements());
					insts.Append(new Quad($$,Quad::NotEq,$1,$3));
				}
			}
		|Expr T_Or Expr		
			{ 	
				ValueType tl=$1->GetType();
				ValueType tr=$3->GetType();
				if(tl==ERROR || tr==ERROR)
				{
					$$=new Operand(ERROR);
				}
				else
				{
					if(tl!=tr || tl!=Boolean)
					{
						ReportError::Formatted(& @2,"type conflict");
						$$=new Operand(ERROR);
					}
					$$=new Operand(tr,insts.NumElements());
					insts.Append(new Quad($$,Quad::Or,$1,$3));
				}
			}
		|Expr T_And Expr	
			{ 	
				ValueType tl=$1->GetType();
				ValueType tr=$3->GetType();
				if(tl==ERROR || tr==ERROR)
				{
					$$=new Operand(ERROR);
				}
				else
				{
					if(tl!=tr || tl!=Boolean)
					{
						ReportError::Formatted(& @2,"type conflict");
						$$=new Operand(ERROR);
					}
					$$=new Operand(tr,insts.NumElements());
					insts.Append(new Quad($$,Quad::And,$1,$3));
				}
			}
		|T_Not Expr		
			{ 	
				ValueType tl=$2->GetType();
				if(tl==ERROR )
				{
					$$=new Operand(ERROR);
				}
				else
				{
					if(tl!=Boolean)
					{
						ReportError::Formatted(& @2,"type conflict");
						$$=new Operand(ERROR);
					}
					$$=new Operand(tl,insts.NumElements());
					insts.Append(new Quad($$,Quad::Not,$2));
				}
			}
		|'(' Expr ')'	{ $$=$2; }
		|LValue	{ $$=$1; }		
		|LValue T_Assign Expr	
			{ 	
				ValueType tl=$1->GetType();
				ValueType tr=$3->GetType();
				if(tl==ERROR || tr==ERROR)
				{
					$$=new Operand(ERROR);
				}
				else
				{
					if(tl!=tr)
					{
						ReportError::Formatted(& @2,"type conflict");
						$$=new Operand(ERROR);
					}
					$$=$3;
					insts.Append(new Quad($1,Quad::Assign,$3));
				}
			}
		;

ExprList	:Expr	{ $$=new List<Operand*>; $$->Append($1); }
		|	 ExprList ',' Expr
			{
				$$=$1;
				$$->Append($3);
			}

IdentList:	T_Identifier	
		 	{
				$$=new List<char*>;
				$$->Append(strdup($1));
			}				
		|	T_Identifier ',' IdentList
			{
				$$=$3;
				$$->Append(strdup($1));
			}

		 	
LValue		:T_Identifier		
			{
				Symbol* s=curSymTbl->Find($1);
				if(s)
				{
					$$=new Operand(s);
				}
				else
				{
					$$=new Operand(ERROR); // try to find more errors
					ReportError::Formatted(& @1,"variable not declared");
				}
			}

		|T_Identifier '[' ExprList ']'	
			{
				Symbol* s=curSymTbl->Find($1);
				if(s && s->ArrayDims() && s->ArrayDims()->NumElements() == $3->NumElements())
				{
					$$=new Operand(s,$3);
				}
				else
				{
					$$=new Operand(ERROR); // try to find more errors
					ReportError::Formatted(& @1,"invalid array indexing");
				}
			}
		;
%%

/* The closing %% above marks the end of the Rules section and the beginning
 * of the User Subroutines section. All text from here to the end of the
 * file is copied verbatim to the end of the generated y.tab.c file.
 * This section is where you put definitions of helper functions.
 */

/*
 * Function: InitParser
 * --------------------
 * This function will be called before any calls to yyparse().  It is designed
 * to give you an opportunity to do anything that must be done to initialize
 * the parser (set global variables, configure starting state, etc.). One
 * thing it already does for you is assign the value of the global variable
 * yydebug that controls whether yacc prints debugging information about
 * parser actions (shift/reduce) and contents of state stack during parser.
 * If set to false, no information is printed. Setting it to true will give
 * you a running trail that might be helpful when debugging your parser.
 * Please be sure the variable is set to false when submitting your final
 * version.
 */
void InitParser()
{
  PrintDebug("parser", "Initializing parser");
  yydebug = false;
}

extern int lexscan();
extern char* yytext;

int yylex(void)
{
	int token=lexscan();
	fprintf(lexout,"%s\n",yytext);
	return token;
}


