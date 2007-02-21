/*
 * Lex inupt file to generate the scanner for L compiler.
 */

%{

#include <string.h>
#include "errors.h"
#include "parser.h" // for token codes, yylval
#include "util.h"

#define TAB_SIZE 4

/* Global variables
 * ----------------
 * (For shame!) But we need a few to keep track of things that are
 * preserved between calls to yylex or used outside the scanner.
 */
static int inhibit_out=0;

static int curLineNum, curColNum;
List<char*> savedLines;

static void DoBeforeEachAction();
#define YY_USER_ACTION DoBeforeEachAction();
#define YY_DECL int lexscan(void)
%}

%s N
%x COPY COMM

%option stack

/* 
 * Definitions
 */

DIGIT             ([0-9])
HEX_DIGIT         ([0-9a-fA-F])
HEX_INTEGER       (0[Xx]{HEX_DIGIT}+)
INTEGER           ({DIGIT}+)
EXPONENT          ([Ee][-+]?{INTEGER})
DOUBLE            ({INTEGER}"."{DIGIT}*{EXPONENT}?)
BEG_STRING        (\"[^"\n]*)
STRING            ({BEG_STRING}\")
IDENTIFIER        ([a-zA-Z][_a-zA-Z0-9]*)
OPERATOR          ([-+/*,:;<>()[\]])
BEG_COMMENT       ("{")
END_COMMENT       ("}")
SINGLE_COMMENT    ("//"[^\n]*)

%%             /* BEGIN RULES SECTION */
<COPY>.*               { 
                         savedLines.Append(strdup(yytext));
                         curColNum = 1; yy_pop_state(); yyless(0); }
<COPY><<EOF>>          { yy_pop_state(); }

<*>\n|\r\n                  { curLineNum++; curColNum = 1;
			 if (YYSTATE == COPY) savedLines.Append("");
                         else yy_push_state(COPY);
		       }
[ ]+                   { /* ignore all spaces */  }
<*>[\t]                { curColNum-=yyleng;curColNum += TAB_SIZE - curColNum%TAB_SIZE + 1; }

 /* -------------------- Comments ----------------------------- */
{BEG_COMMENT}          { BEGIN(COMM); }
<COMM>{END_COMMENT}    { BEGIN(0); }
<COMM><<EOF>>          { ReportError::UntermComment();
                         return 0; }
<COMM>.                   { /* ignore everything else that doesn't match */ }
{SINGLE_COMMENT}       { /* skip to end of line for // comment */ }


 /* --------------------- Keywords ------------------------------- */
"CONST"|"const"				{ return T_Const;}
"ARRAY"|"array"				{ return T_Array;}
"PROGRAM"|"program"			{ return T_Program; }
"BEGIN"|"begin"				{ return T_Begin; }
"END"|"end"					{ return T_End; }
"INTEGER"|"integer"           { return T_Int;         }
"REAL"|"real"              { return T_Double;      }
"BOOLEAN"|"boolean"           { return T_Bool;        }
"WHILE"|"while"             { return T_While;       }
"DO"|"do"                { return T_Do;         }
"IF"|"if"                { return T_If;          }
"ELSE"|"else"              { return T_Else;        }
"THEN"|"then"              { return T_Then;      }
"REPEAT"|"repeat"            { return T_Repeat;       }
"UNTIL"|"until"             { return T_Until;         }
"FOR"|"for"               { return T_For;    }
"TO"|"to"                { return T_To;       }
"READ"|"read"              { return T_Read; }
"WRITE"|"write"             { return T_Write;    }

 /* -------------------- Operators ----------------------------- */
":="				{ return T_Assign; }
"<="                { return T_LessEqual;   }
">="                { return T_GreaterEqual;}
"="                { return T_Equal;       }
"<>"                { return T_NotEqual;    }
"AND"|"and"                { return T_And;         }
"OR"|"or"                { return T_Or;          }
"NOT"|"not"                { return T_Not;          }
{OPERATOR}          { return yytext[0];     }
    
 /* -------------------- Constants ------------------------------ */
"TRUE"|"FALSE"|"true"|"false"      { yylval.boolConstant = (yytext[0] == 'T' || yytext[0] == 't');
                         return T_BoolConstant; }
{INTEGER}           { yylval.integerConstant = strtol(yytext, NULL, 10);
                         return T_IntConstant; }
{HEX_INTEGER}       { yylval.integerConstant = strtol(yytext, NULL, 16);
                         return T_IntConstant; }
{DOUBLE}            { yylval.doubleConstant = atof(yytext);
                         return T_DoubleConstant; }
{STRING}            { yylval.stringConstant = strdup(yytext); 
                         return T_StringConstant; }
{BEG_STRING}        { ReportError::UntermString(&yylloc, yytext); }

 /* -------------------- Identifiers --------------------------- */
{IDENTIFIER}        { if (strlen(yytext) > MaxIdentLen)
                         ReportError::LongIdentifier(&yylloc, yytext);
                       strncpy(yylval.identifier, yytext, MaxIdentLen);
                       yylval.identifier[MaxIdentLen] = '\0';
                       return T_Identifier; }


 /* -------------------- Default rule (error) -------------------- */
.                   { ReportError::UnrecogChar(&yylloc, yytext[0]); }


%%


/*
 * Function: InitScanner
 * ---------------------
 * This function will be called before any calls to yylex().  It is designed
 * to give you an opportunity to do anything that must be done to initialize
 * the scanner (set global variables, configure starting state, etc.). One
 * thing it already does for you is assign the value of the global variable
 * yy_flex_debug that controls whether flex prints debugging information
 * about each token and what rule was matched. If set to false, no information
 * is printed. Setting it to true will give you a running trail that might
 * be helpful when debugging your scanner. Please be sure the variable is
 * set to false when submitting your final version.
 */
void InitScanner()
{
    PrintDebug("lex", "Initializing scanner");
    yy_flex_debug = false;
    BEGIN(N);
    yy_push_state(COPY);
    curLineNum = 1;
    curColNum = 1;
}


/*
 * Function: DoBeforeEachAction()
 * ------------------------------
 * This function is installed as the YY_USER_ACTION. This is a place
 * to group code common to all actions.
 * On each match, we fill in the fields to record its location and
 * update our column counter.
 */
static void DoBeforeEachAction()
{
#if 0
	if(yytext[yyleng-1]!='\n' && yytext[yyleng-1]!='\r')
	printf("%d,%s\n",yyleng,yytext);
#endif
    yylloc.first_line = curLineNum;
    yylloc.first_column = curColNum;
    yylloc.last_column = curColNum + yyleng - 1;
    curColNum += yyleng;
	
}

/* Function: GetLineNumbered()
 * ---------------------------
 * Returns string with contents of line numbered n or NULL if the
 * contents of that line are not available.  Our scanner copies
 * each line scanned and appends each to a list so we can later
 * retrieve them to report the context for errors.
 */
const char *GetLineNumbered(int num) {
   if (num <= 0 || num > savedLines.NumElements()) return NULL;
   return savedLines.Nth(num-1);
}



