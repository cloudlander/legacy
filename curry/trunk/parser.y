/* File: parser.y
 * --------------
 * Yacc input file to generate the parser for the compiler.
 *
 * pp2: your job is to write a parser that will construct the parse tree
 *      and if no parse errors were found, print it.  The parser should 
 *      accept the language as described in specification, and as augmented 
 *      in the pp2 handout.
 */

%{

/* Just like lex, the text within this first region delimited by %{ and %}
 * is assumed to be C/C++ code and will be copied verbatim to the y.tab.c
 * file ahead of the definitions of the yyparse() function. Add other header
 * file inclusions or C++ variable declarations/prototypes that are needed
 * by your code here.
 */
#include "scanner.h" // for yylex
#include "parser.h"
#include "errors.h"

void yyerror(char *msg); // standard error-handling routine
Program* program;
%}

/*
 * The section before the first %% is the Definitions section of the yacc
 * input file. Here is where you declare tokens and types, add precedence
 * and associativity options, and so on.
 */
 
/*
 * yylval 
 * ------
 * Here we define the type of the yylval global variable that is used by
 * the scanner to store attibute information about the token just scanned
 * and thus communicate that information to the parser. You will need to
 * add new fields to this union as you add different attributes to your
 * non-terminal symbols.
 */
%union {
    int integerConstant;
    bool boolConstant;
    char *stringConstant;
    double doubleConstant;
    char identifier[MaxIdentLen+1]; // +1 for terminating null

    Decl *decl;
    List<Decl*> *declList;
    List<VarDecl*> *varDeclList;
    List<Expr*> *exprList;

    VarDecl *varDecl;
    Type *type;
    Identifier	*ident;
    FnDecl *fnDecl;

    Stmt *stmt;
    List<Stmt*> *stmtList;
    StmtBlock *stmtBlock;

    IfStmt *ifStmt;
    WhileStmt *whileStmt;
    ForStmt *forStmt;
    ReturnStmt *returnStmt;
    BreakStmt *breakStmt;
    PrintStmt *printStmt;
    TryStmt *tryStmt;
    CatchStmt *catchStmt;
    ThrowStmt *throwStmt;
    SwitchStmt *switchStmt;
    CaseStmt *caseStmt;
    DefaultStmt *defaultStmt;
    SwitchBody *switchBody;
    List<CatchStmt*> *catchStmtList;
    List<CaseStmt*>  *caseStmtList;

    Expr *expr;
    LValue *LV;
    Call *call;
}


/* Tokens
 * ------
 * Here we tell yacc about all the token types that we are using.
 * Yacc will assign unique numbers to these and export the #define
 * in the generated y.tab.h header file.
 */
%token   T_Void T_Bool T_Int T_Double T_String T_Class 
%token   T_LessEqual T_GreaterEqual T_Equal T_NotEqual T_Dims
%token   T_And T_Or T_Null T_Extends T_This
%token   T_While T_For T_If T_Else T_Return T_Break
%token   T_New T_NewArray T_Print T_ReadInteger T_ReadLine
%token	 T_Try T_Catch T_Throw T_Switch T_Case T_Default

%token   <identifier> T_Identifier 
%token   <stringConstant> T_StringConstant 
%token   <integerConstant> T_IntConstant
%token   <doubleConstant> T_DoubleConstant
%token   <boolConstant> T_BoolConstant

/* No T_ClassIdentifier */
/* Non-terminal types
 * ------------------
 * In order for yacc to assign/access the correct field of $$, $1, we
 * must to declare which field is appropriate for the non-terminal.
 * As an example, this first type declaration establishes that the DeclList
 * non-terminal uses the field named "declList" in the yylval union. This
 * means that when we are setting $$ for a reduction for DeclList ore reading
 * $n which corresponds to a DeclList nonterminal we are accessing the field
 * of the union named "declList" which is of type List<Decl*>.
 * pp2: You'll need to add many of these of your own.
 */
%type <declList>	DeclList Fieldlist
%type <decl>		Decl	Field	ClassDecl
%type <varDecl>		VariableDecl	Variable
%type <fnDecl>		FunctionDecl
%type <varDeclList>	Formals NonEmptyVList //VarDeclList
%type <type>		Type

%type <stmt>		Stmt
%type <stmtList>	NonEmptySList StmtList 
%type <stmtBlock>	StmtBlock

%type <switchBody>	SwitchBody
%type <defaultStmt>	DefaultStmt
%type <caseStmt>	CaseStmt
%type <switchStmt>	SwitchStmt
%type <ifStmt>		IfStmt
%type <whileStmt>	WhileStmt
%type <forStmt>		ForStmt
%type <returnStmt>	ReturnStmt
%type <breakStmt>	BreakStmt
%type <printStmt>	PrintStmt
%type <tryStmt>		TryCatchBlock
%type <catchStmt>	CatchBlock
%type <throwStmt>	ThrowStmt
%type <catchStmtList>	CatchBlockList NonEmptyCBList
%type <caseStmtList>	CaseStmtList

%type <expr>		Expr Constant IntConstant Expr_Em
%type <LV>		LValue
%type <call>		Call
%type <exprList>	Actuals NonEmptyActuals

//%type <type>		BaseType ClassType
%type <ident>		Ident
//%type <ident>		ClassIdent

%left ','
%left '='
%left ':' '?'
%left T_Or 
%left T_And
%left T_Equal T_NotEqual
%left '<' '>' T_LessEqual T_GreaterEqual
%left '-' '+'
%left '*' '/' '%'
%right '!'
%left '.' '[' 
%nonassoc UMINUS
%nonassoc Lower_Else
%nonassoc T_Else
%%
/*
 * All productions and actions should be placed between the start and stop
 * %% markers which delimit the Rules section.
 */
	 
Program :DeclList
	{ 
	@1; 
        /* pp2: The @1 is needed to convince 
         * yacc to set up yylloc. You can remove 
         * it oncde you have other uses of @n*/
        program = new Program($1);
/*
        if (ReportError::NumErrors() == 0)
		program->Print(0);
*/
        }
        ;
          
DeclList  :    DeclList Decl        { ($$=$1)->Append($2); }
          |    Decl                 { ($$ = new List<Decl*>)->Append($1); }
          ;

Decl      :VariableDecl { $$ = $1; }
	  |ClassDecl	{ $$ = $1; }
	  |FunctionDecl	{ $$ = $1; }
          ;
VariableDecl	:Variable ';'	{ $$ = $1; }
		;
Variable	:Type Ident	{ $$ = new VarDecl($2,$1); }
		;
Ident		:T_Identifier	{ $$ = new Identifier(@1,$1);}
		;
Type		:T_Bool		{ $$ = Type::boolType; }
		|T_Int		{ $$ = Type::intType; }
		|T_Double	{ $$ = Type::doubleType; }
		|T_String	{ $$ = Type::stringType; }
		|Ident	{ NamedType *nameType = new NamedType($1);$$ = nameType;}
		|Type T_Dims	{$$ = new ArrayType(yylloc, $1);}
		;
ClassDecl	:T_Class Ident '{' Fieldlist '}'	{ $$ = new ClassDecl($2,NULL,$4);}
		|T_Class Ident T_Extends Ident '{' Fieldlist '}' { 
			$$ = new ClassDecl($2,new NamedType($4),$6); 
			}
		;
Fieldlist	:Fieldlist Field	{ ($$ = $1)->Append($2);}
		|			{ $$ = new List<Decl*>; }
		;
Field		:VariableDecl	{ $$ = $1; }
		|FunctionDecl	{ $$ = $1; }
		;

FunctionDecl	:Type Ident '(' Formals ')' StmtBlock {$$ = new FnDecl($2,$1,$4);$$->SetFunctionBody($6);}
		|T_Void Ident '(' Formals ')' StmtBlock {$$ = new FnDecl($2,Type::voidType,$4);$$->SetFunctionBody($6);}
		;
Formals		:Variable		{ ($$ = new List<VarDecl*>)->Append($1); }
		|Formals ',' Variable	{ ($$ = $1)->Append($3); }
		|			{ $$ = new List<VarDecl*>; }
		;
StmtBlock	:'{' NonEmptyVList NonEmptySList '}' {$$ = new StmtBlock($2,$3);}
		|'{' NonEmptyVList '}'	{
						List<Stmt*> *emptySList = new List<Stmt*>;
						$$ = new StmtBlock($2,emptySList);
					}
		|'{' NonEmptySList '}'	{
						List<VarDecl*> *emptyVList = new List<VarDecl*>;
						$$ = new StmtBlock(emptyVList,$2);
					}
		|'{' '}'		{List<Stmt*> *emptyStmtList = new List<Stmt*>;
					 List<VarDecl*> *emptyVarList = new List<VarDecl*>;
					 $$ = new StmtBlock(emptyVarList,emptyStmtList);}
		;
NonEmptyVList	:VariableDecl			{ ($$ = new List<VarDecl*>)->Append($1); }
		|NonEmptyVList VariableDecl	{ ($$ = $1)->Append($2);}
		;
NonEmptySList	:Stmt			{($$=new List<Stmt*>)->Append($1);}
		|NonEmptySList Stmt	{($$=$1)->Append($2);}
		;
Stmt	:';'                    {$$=new ExprStmt(yylloc,NULL);}
        |Expr ';'               {$$=new ExprStmt(@1,$1);}
		|StmtBlock		{$$=$1;}
		|IfStmt			{$$=$1;}
		|SwitchStmt		{$$=$1;}
		|WhileStmt		{$$=$1;}
		|ForStmt		{$$=$1;}
		|ReturnStmt		{$$=$1;}
		|BreakStmt		{$$=$1;}
		|PrintStmt		{$$=$1;}
		|TryCatchBlock		{$$=$1;}
		|ThrowStmt		{$$=$1;}
		;
ThrowStmt	:T_Throw Expr ';'	{$$ = new ThrowStmt(yylloc,$2);}
		;
TryCatchBlock	:T_Try '{' StmtList '}' CatchBlockList	{$$ = new TryStmt($3,$5);}
		;
StmtList	:NonEmptySList		{$$=$1;}
		|			{$$=new List<Stmt*>;}
		;
NonEmptyCBList	:CatchBlock			{($$=new List<CatchStmt*>)->Append($1);}
		|NonEmptyCBList CatchBlock	{($$=$1)->Append($2);}
		;
CatchBlockList	:NonEmptyCBList		{$$ = $1;}
		|			{$$=new List<CatchStmt*>;}
		;
CatchBlock	:T_Catch '(' Variable ')' '{' StmtList '}'	{$$ = new CatchStmt($3,$6);}
		;
PrintStmt	:T_Print '(' NonEmptyActuals ')' ';'	{$$ = new PrintStmt($3);}
		;
BreakStmt	:T_Break ';'		{$$ = new BreakStmt(@1);}
		;
ReturnStmt	:T_Return Expr ';'	{$$ = new ReturnStmt(@2,$2);}
		|T_Return ';'		{$$ = new ReturnStmt(@1,new EmptyExpr());}
		;
IfStmt		:T_If '(' Expr ')' Stmt %prec Lower_Else	{$$ = new IfStmt($3,$5,NULL);}
		|T_If '(' Expr ')' Stmt T_Else Stmt		{$$ = new IfStmt($3,$5,$7);}
		;
SwitchStmt	:T_Switch '(' Expr ')' SwitchBody	{$$ = new SwitchStmt($3,$5);}
		;
SwitchBody	:'{' CaseStmtList DefaultStmt '}'	{$$ = new SwitchBody($2,$3);}
		|'{' CaseStmtList '}'			{$$ = new SwitchBody($2,NULL);}
		|'{' DefaultStmt '}'	{
			List<CaseStmt*> *emptyCaseList = new List<CaseStmt*>;
			$$ = new SwitchBody(emptyCaseList,$2);
					}
		|'{' '}'		{
			List<CaseStmt*> *emptyCaseList = new List<CaseStmt*>;
                        $$ = new SwitchBody(emptyCaseList,NULL);
					}
		;
CaseStmtList	:CaseStmt			{($$=new List<CaseStmt*>)->Append($1);}
		|CaseStmtList CaseStmt		{($$=$1)->Append($2);}
		;
CaseStmt	:T_Case IntConstant ':' Stmt		{$$ = new CaseStmt($2, $4);}
		;
DefaultStmt	:T_Default ':' Stmt		{$$ = new DefaultStmt($3);}
		;
WhileStmt	:T_While '(' Expr ')' Stmt	{$$ = new WhileStmt($3,$5);}
		;
ForStmt		:T_For '(' Expr_Em ';' Expr ';' Expr_Em ')' Stmt	{$$ = new ForStmt($3,$5,$7,$9);}
		;
Expr_Em		:Expr	{$$ = $1;}
		|	{$$ = new EmptyExpr();}
		;
Expr		:Constant		{$$ = $1;}
		|Expr '+' Expr		{$$ = new ArithmeticExpr($1,new Operator(@2,"+"),$3);}
		|Expr '*' Expr		{$$ = new ArithmeticExpr($1,new Operator(@2,"*"),$3);}
		|Expr '/' Expr		{$$ = new ArithmeticExpr($1,new Operator(@2,"/"),$3);}
		|Expr '%' Expr		{$$ = new ArithmeticExpr($1,new Operator(@2,"%"),$3);}
		|Expr '>' Expr		{$$ = new RelationalExpr($1,new Operator(@2,">"),$3);}
		|Expr '<' Expr		{$$ = new RelationalExpr($1,new Operator(@2,"<"),$3);}
		|'-'Expr %prec '!'	{$$ = new ArithmeticExpr(new Operator(@1,"-"),$2);}
		|Expr '-' Expr          {$$ = new ArithmeticExpr($1,new Operator(@2,"-"),$3);}
		|Expr T_LessEqual Expr	{$$ = new RelationalExpr($1,new Operator(@2,"<="),$3);}
		|Expr T_GreaterEqual Expr	{$$ = new RelationalExpr($1,new Operator(@2,">="),$3);}
		|Expr T_Equal Expr	{$$ = new EqualityExpr($1,new Operator(@2,"=="),$3);}
		|Expr T_NotEqual Expr	{$$ = new EqualityExpr($1,new Operator(@2,"!="),$3);}
		|Expr T_Or Expr		{$$ = new LogicalExpr($1,new Operator(@2,"||"),$3);}
		|Expr T_And Expr	{$$ = new LogicalExpr($1,new Operator(@2,"&&"),$3);}
		|'!' Expr		{$$ = new LogicalExpr(new Operator(@1,"!"),$2);}
		|'(' Expr ')'		{$$ = $2;}
		|LValue			{$$ = $1;}
		|LValue '=' Expr	{$$ = new AssignExpr($1,new Operator(@2,"="),$3);}
		|T_New '(' Ident ')'	{$$ = new NewExpr(yylloc,new NamedType($3));}
		|T_NewArray '(' Expr ',' Type ')'	{$$ = new NewArrayExpr(yylloc,$3,$5);}
		|T_ReadLine '(' ')'	{$$ = new ReadLineExpr(yylloc);}
		|T_ReadInteger '(' ')'	{$$ = new ReadIntegerExpr(yylloc);}
		|T_This			{$$ = new This(yylloc);}
		|Call			{$$ = $1;}
		|Expr '?' Expr ':' Expr	{
			$$ = new ConditionalExpr($1,new Operator(@2,"?"),$3,new Operator(@4,":"),$5);
					}
		;
Call		:Ident '(' Actuals ')'	{$$ = new Call(Join(@1,@4),NULL,$1,$3);}
		|Expr '.' Ident '(' Actuals ')'	{$$ = new Call(@3,$1,$3,$5);}
		;
NonEmptyActuals	:Expr			{($$ = new List<Expr*>)->Append($1);}
		|NonEmptyActuals ',' Expr	{($$ = $1)->Append($3);}
		;
Actuals		:NonEmptyActuals	{$$ = $1;}
		|			{$$ = new List<Expr*>;}
		;
LValue		:Ident			{$$ = new FieldAccess(NULL,$1);}
		|Expr '.' Ident		{$$ = new FieldAccess($1,$3);}
		|Expr '[' Expr ']'	{$$ = new ArrayAccess(yylloc,$1,$3);}
		;
IntConstant	:T_IntConstant          {$$ = new IntConstant(yylloc,$1);}
		;
Constant	:IntConstant		{$$ = $1;}
		|T_DoubleConstant	{$$ = new DoubleConstant(yylloc,$1);}
		|T_BoolConstant		{$$ = new BoolConstant(yylloc,$1);}
		|T_StringConstant	{$$ = new StringConstant(yylloc,$1);}
		|T_Null			{$$ = new NullConstant(yylloc);}
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


