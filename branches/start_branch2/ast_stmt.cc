/* File: ast_stmt.cc
 * -----------------
 * Implementation of statement node classes.
 */
#include "ast_stmt.h"
#include "ast_type.h"
#include "ast_decl.h"
#include "ast_expr.h"


Program::Program(List<Decl*> *d) {
    Assert(d != NULL);
    (decls=d)->SetParentAll(this);
}

void Program::PrintChildren(int indentLevel) {
    decls->PrintAll(indentLevel+1);
    printf("\n");
}

StmtBlock::StmtBlock(List<VarDecl*> *d, List<Stmt*> *s) {
    Assert(d != NULL && s != NULL);
    (decls=d)->SetParentAll(this);
    (stmts=s)->SetParentAll(this);
}

void StmtBlock::PrintChildren(int indentLevel) {
    decls->PrintAll(indentLevel+1);
    stmts->PrintAll(indentLevel+1);
}

ExprStmt::ExprStmt(yyltype loc, Expr *e) : Stmt(loc) {
        if(e)
          (expr=e)->SetParent(this);
}

void ExprStmt::PrintChildren(int indentLevel)
{   
        if(expr)
          expr->Print(indentLevel+1);
}       

ConditionalStmt::ConditionalStmt(Expr *t, Stmt *b) { 
    Assert(t != NULL && b != NULL);
    (test=t)->SetParent(this); 
    (body=b)->SetParent(this);
}

ForStmt::ForStmt(Expr *i, Expr *t, Expr *s, Stmt *b): LoopStmt(t, b) { 
    Assert(i != NULL && t != NULL && s != NULL && b != NULL);
    (init=i)->SetParent(this);
    (step=s)->SetParent(this);
}

void ForStmt::PrintChildren(int indentLevel) {
    init->Print(indentLevel+1, "(init) ");
    test->Print(indentLevel+1, "(test) ");
    step->Print(indentLevel+1, "(step) ");
    body->Print(indentLevel+1, "(body) ");
}

void WhileStmt::PrintChildren(int indentLevel) {
    test->Print(indentLevel+1, "(test) ");
    body->Print(indentLevel+1, "(body) ");
}

IfStmt::IfStmt(Expr *t, Stmt *tb, Stmt *eb): ConditionalStmt(t, tb) { 
    Assert(t != NULL && tb != NULL); // else can be NULL
    elseBody = eb;
    if (elseBody) elseBody->SetParent(this);
}

void IfStmt::PrintChildren(int indentLevel) {
    test->Print(indentLevel+1, "(test) ");
    body->Print(indentLevel+1, "(then) ");
    if (elseBody) elseBody->Print(indentLevel+1, "(else) ");
}

DefaultStmt::DefaultStmt(Stmt *b)
{
    Assert(b != NULL);
    (body = b)->SetParent(this);
}

void DefaultStmt::PrintChildren(int indentLevel)
{
    body->Print(indentLevel+1,"(body) ");
}

void CaseStmt::PrintChildren(int indentLevel)
{
    test->Print(indentLevel+1, "(test) ");
    body->Print(indentLevel+1, "(body) ");
}

void SwitchStmt::PrintChildren(int indentLevel)
{
    test->Print(indentLevel+1, "(test) ");
    body->Print(indentLevel+1, "(body) ");
}

SwitchBody::SwitchBody(List<CaseStmt*> *CaseList,DefaultStmt *Default)
{
    Assert(CaseList != NULL);
    (CaseStmtList = CaseList)->SetParentAll(this);
    if(Default != NULL)
	(defaultStmt = Default)->SetParent(this);
}

void SwitchBody::PrintChildren(int indentLevel)
{
    CaseStmtList->PrintAll(indentLevel+1);
    if(defaultStmt != NULL)
	defaultStmt->Print(indentLevel+1, "(default) ");
}

ReturnStmt::ReturnStmt(yyltype loc, Expr *e) : Stmt(loc) { 
    Assert(e != NULL);
    (expr=e)->SetParent(this);
}

void ReturnStmt::PrintChildren(int indentLevel) {
    expr->Print(indentLevel+1);
}
  
PrintStmt::PrintStmt(List<Expr*> *a) {    
    Assert(a != NULL);
    (args=a)->SetParentAll(this);
}

void PrintStmt::PrintChildren(int indentLevel) {
    args->PrintAll(indentLevel+1, "(args) ");
}


CatchStmt::CatchStmt(VarDecl *excpt,List<Stmt*> *actbody){
	Assert(excpt != NULL);
	(exception=excpt)->SetParent(this);
	if (actbody != NULL)
		(actions=actbody)->SetParentAll(this);
	else
	    actions=NULL;
}

void CatchStmt::PrintChildren(int indentLevel) {
	exception->Print(indentLevel+1,"(exception)");
	actions->PrintAll(indentLevel+1, "(actions)");
}

TryStmt::TryStmt(List<Stmt*> *mainbody,List<CatchStmt*> *cblocks){
	Assert(mainbody != NULL);
	(stmts=mainbody)->SetParentAll(this);
	if (cblocks != NULL)
		(catchblocks=cblocks)->SetParentAll(this);
	else
	    catchblocks=NULL;
}

void TryStmt::PrintChildren(int indentLevel) {
	stmts->PrintAll(indentLevel+1, "(stmts)");
	catchblocks->PrintAll(indentLevel+1);
}


ThrowStmt::ThrowStmt(yyltype loc, Expr *excpt) : Stmt(loc) { 
	Assert(excpt != NULL);
	(exception=excpt)->SetParent(this);
}

void ThrowStmt::PrintChildren(int indentLevel) {
	exception->Print(indentLevel+1);
}
