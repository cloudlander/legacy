/* File: ast_stmt.h
 * ----------------
 * The Stmt class and its subclasses are used to represent
 * statements in the parse tree.  For each statment in the
 * language (for, if, return, etc.) there is a corresponding
 * node class for that construct. 
 */


#ifndef _H_ast_stmt
#define _H_ast_stmt

#include "list.h"
#include "ast.h"

class Decl;
class VarDecl;
class Expr;
  
class Program : public Node
{
  protected:
     List<Decl*> *decls;
     
  public:
     Program(List<Decl*> *declList);
     const char *GetPrintNameForNode() { return "Program"; }
     void PrintChildren(int indentLevel);
//   for symbol table usage	 
	 void BuildSymTable(SymTable*);
	 void DetermineLocation();
};

class Stmt : public Node
{
  public:
     Stmt() : Node() {}
     Stmt(yyltype loc) : Node(loc) {}
};

class StmtBlock : public Stmt 
{
  protected:
    List<VarDecl*> *decls;
    List<Stmt*> *stmts;
    
  public:
    StmtBlock(List<VarDecl*> *variableDeclarations, List<Stmt*> *statements);
    const char *GetPrintNameForNode() { return "StmtBlock"; }
    void PrintChildren(int indentLevel);
	 
	void BuildSymTable(SymTable*);
	void DetermineLocation();
	Location* GenTac(CodeGenerator*,SymTable*);

	bool Check(SymTable*);
};

class ConditionalStmt : public Stmt
{
  protected:
    Expr *test;
    Stmt *body;
  
  public:
    ConditionalStmt(Expr *testExpr, Stmt *body);
};

class ExprStmt : public Stmt
{   
   protected:
        Expr *expr;
   public:
        ExprStmt(yyltype loc, Expr *e);
        const char *GetPrintNameForNode() { return "ExprStmt";}
        void PrintChildren(int indentLevel);
	Location* GenTac(CodeGenerator*,SymTable*);

	bool Check(SymTable* symtbl);
};

class LoopStmt : public ConditionalStmt 
{
  public:
    LoopStmt(Expr *testExpr, Stmt *body)
            : ConditionalStmt(testExpr, body) {}
};

class ForStmt : public LoopStmt 
{
  protected:
    Expr *init, *step;
  
  public:
    ForStmt(Expr *init, Expr *test, Expr *step, Stmt *body);
    const char *GetPrintNameForNode() { return "ForStmt"; }
    void PrintChildren(int indentLevel);
	 
	void BuildSymTable(SymTable*);
	void DetermineLocation();
	Location* GenTac(CodeGenerator*,SymTable*);
	bool Check(SymTable*);
};

class WhileStmt : public LoopStmt 
{
  public:
    WhileStmt(Expr *test, Stmt *body) : LoopStmt(test, body) {}
    const char *GetPrintNameForNode() { return "WhileStmt"; }
    void PrintChildren(int indentLevel);
	 
	void BuildSymTable(SymTable*);
	void DetermineLocation();
	Location* GenTac(CodeGenerator*,SymTable*);

	bool Check(SymTable*);
};

class IfStmt : public ConditionalStmt 
{
  protected:
    Stmt *elseBody;

	// for elseBody's new scope
	SymTable* elsetable;
  
  public:
    IfStmt(Expr *test, Stmt *thenBody, Stmt *elseBody);
    const char *GetPrintNameForNode() { return "IfStmt"; }
    void PrintChildren(int indentLevel);
	 
	void BuildSymTable(SymTable*);
	void DetermineLocation();
	Location* GenTac(CodeGenerator*,SymTable*);

	bool Check(SymTable*);
};

class SwitchStmt : public ConditionalStmt
{
  public:
    SwitchStmt(Expr *test, Stmt *body) : ConditionalStmt(test, body) {}
    const char *GetPrintNameForNode() { return "SwitchStmt"; }
    void PrintChildren(int indentLevel);
	 
	void BuildSymTable(SymTable*);
	void DetermineLocation();

	Location* GenTac(CodeGenerator*,SymTable*);
	bool Check(SymTable*);
};

class CaseStmt : public ConditionalStmt
{
  public:
    CaseStmt(Expr *test, Stmt *body) : ConditionalStmt(test, body) {}
    const char *GetPrintNameForNode() { return "CaseStmt"; }
    void PrintChildren(int indentLevel);
	 
	void BuildSymTable(SymTable*);
	void DetermineLocation();

	Location* GenTac(CodeGenerator*,SymTable*);
	bool Check(SymTable*);
};

class DefaultStmt : public Stmt
{
  protected:
    Stmt *body;
  public:
    DefaultStmt(Stmt *b);
    const char *GetPrintNameForNode() { return "DefaultStmt"; }
    void PrintChildren(int indentLevel);
	 
	void BuildSymTable(SymTable*);
	void DetermineLocation();

	Location* GenTac(CodeGenerator*,SymTable*);
	bool Check(SymTable*);
};

class SwitchBody : public Stmt
{
  protected:
    List<CaseStmt*> *CaseStmtList;
    DefaultStmt *defaultStmt;
  public:
    SwitchBody(List<CaseStmt*> *CaseList, DefaultStmt *Default);
    const char *GetPrintNameForNode() { return "SwitchBody"; }
    void PrintChildren(int indentLevel);
	 
	void BuildSymTable(SymTable*);
	void DetermineLocation();

	Location* GenTac(CodeGenerator*,SymTable*);
	bool Check(SymTable*);
};

class BreakStmt : public Stmt 
{
  public:
    BreakStmt(yyltype loc) : Stmt(loc) {}
    const char *GetPrintNameForNode() { return "BreakStmt"; }
	Location* GenTac(CodeGenerator*,SymTable*);
	bool Check(SymTable*);
};

class ReturnStmt : public Stmt  
{
  protected:
    Expr *expr;
  
  public:
    ReturnStmt(yyltype loc, Expr *expr);
    const char *GetPrintNameForNode() { return "ReturnStmt"; }
    void PrintChildren(int indentLevel);
	Location* GenTac(CodeGenerator*,SymTable*);

	bool Check(SymTable*);
};

class PrintStmt : public Stmt
{
  protected:
    List<Expr*> *args;
    
  public:
    PrintStmt(List<Expr*> *arguments);
    const char *GetPrintNameForNode() { return "PrintStmt"; }
    void PrintChildren(int indentLevel);
	Location* GenTac(CodeGenerator*,SymTable*);
	bool Check(SymTable*);
};

class CatchStmt : public Stmt
{
	protected:
	  VarDecl *exception;
	  List<Stmt*> *actions;
	public:
	  CatchStmt(VarDecl *excpt,List<Stmt*> *actbody);
	  const char *GetPrintNameForNode() { return "CatchStmt"; }
	  void PrintChildren(int indentLevel);
	 
	void BuildSymTable(SymTable*);
	void DetermineLocation();

	Location* GenTac(CodeGenerator*,SymTable*);
	bool Check(SymTable*);
};

class TryStmt : public Stmt
{
	protected:
      List<Stmt*> *stmts;
	  List<CatchStmt*> *catchblocks;
	public:
	  TryStmt(List<Stmt*> *mainbody,List<CatchStmt*> *cblocks);
	  const char *GetPrintNameForNode() { return "TryStmt"; }
	  void PrintChildren(int indentLevel);
	 
	void BuildSymTable(SymTable*);
	void DetermineLocation();

	Location* GenTac(CodeGenerator*,SymTable*);
	bool Check(SymTable*);
};

class ThrowStmt : public Stmt
{
	protected:
	  Expr *exception;
	public:
	  ThrowStmt(yyltype loc, Expr *excpt);
	  const char *GetPrintNameForNode() { return "ThrowStmt"; }
	  void PrintChildren(int indentLevel);

	  Location* GenTac(CodeGenerator*,SymTable*);
	bool Check(SymTable*);
};


#endif
