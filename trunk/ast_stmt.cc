/* File: ast_stmt.cc
 * -----------------
 * Implementation of statement node classes.
 */
#include "ast_stmt.h"
#include "ast_type.h"
#include "ast_decl.h"
#include "ast_expr.h"
#include "utility.h"
#include "symtable.h"

/*  implemention of PP4 start */
extern int localoffset;

void Program::BuildSymTable(SymTable* parent)
{
	int indent=0;
	Assert(NULL==parent);
	Assert(NULL==symtable);
	symtable=new SymTable(parent,"Program Globle Scope");
	DEBUGSCOPEBEGIN(symtable)
	for(int i=0;i<decls->NumElements();i++)
	{
		// append all the global vars,functions,classes to symbol table
		// functions and classes will create a new scope, however, vars will append itself to this scope
		decls->Nth(i)->BuildSymTable(symtable);
	}
	DEBUGSCOPE(symtable)
	DEBUGSCOPEEND(symtable)
}

void Program::DetermineLocation()
{
	DEBUGLOCATIONBEGIN(symtable)
	symtable->DetermineGlobalLocation();
	DEBUGLOCATION(symtable)
	DEBUGLOCATIONEND(symtable)
	
}

void StmtBlock::BuildSymTable(SymTable* parent)
{
	Assert(NULL!=parent);
	Assert(NULL==symtable);
	symtable=new SymTable(parent,"StmtBlock Scope");
	DEBUGSCOPEBEGIN(symtable)
	int i;
	for(i=0;i<decls->NumElements();i++)
		decls->Nth(i)->BuildSymTable(symtable);
	for(i=0;i<stmts->NumElements();i++)
		stmts->Nth(i)->BuildSymTable(symtable);
	DEBUGSCOPE(symtable)
	DEBUGSCOPEEND(symtable)
}

void StmtBlock::DetermineLocation()
{
	Assert(NULL!=symtable);
	DEBUGLOCATIONBEGIN(symtable)
	localoffset=symtable->DetermineLocalLocation(localoffset,-1);
	int i;
	for(i=0;i<stmts->NumElements();i++)
		stmts->Nth(i)->DetermineLocation();
	DEBUGLOCATION(symtable)
	DEBUGLOCATIONEND(symtable)
	
}

void IfStmt::BuildSymTable(SymTable* parent)
{
	Assert(NULL!=parent);
	Assert(NULL==symtable);
	body->BuildSymTable(parent);
	if(elseBody)
		elseBody->BuildSymTable(parent);
}

void IfStmt::DetermineLocation()
{
	body->DetermineLocation();
	if(elseBody)
		elseBody->DetermineLocation();
}

void WhileStmt::BuildSymTable(SymTable* parent)
{
	Assert(NULL!=parent);
	Assert(NULL==symtable);
	body->BuildSymTable(parent);
}

void WhileStmt::DetermineLocation()
{
	body->DetermineLocation();
}


void ForStmt::BuildSymTable(SymTable* parent)
{
	Assert(NULL!=parent);
	Assert(NULL==symtable);
	body->BuildSymTable(parent);
}

void ForStmt::DetermineLocation()
{
	body->DetermineLocation();
}

void TryStmt::BuildSymTable(SymTable* parent)
{
	Assert(NULL!=parent);
	Assert(NULL==symtable);
	symtable=new SymTable(parent,"Try Scope");
	DEBUGSCOPEBEGIN(symtable)
	int i;
	for(i=0;i<stmts->NumElements();i++)
		stmts->Nth(i)->BuildSymTable(symtable);
	for(i=0;i<catchblocks->NumElements();i++)
		catchblocks->Nth(i)->BuildSymTable(symtable);
	DEBUGSCOPE(symtable)
	DEBUGSCOPEEND(symtable)
}

void TryStmt::DetermineLocation()
{
	Assert(NULL!=symtable);
	DEBUGLOCATIONBEGIN(symtable)
	localoffset=symtable->DetermineLocalLocation(localoffset,-1);
	int i;
	for(i=0;i<stmts->NumElements();i++)
		stmts->Nth(i)->DetermineLocation();
	for(i=0;i<catchblocks->NumElements();i++)
		catchblocks->Nth(i)->DetermineLocation();
	DEBUGLOCATION(symtable)
	DEBUGLOCATIONEND(symtable)
	
}

void CatchStmt::BuildSymTable(SymTable* parent)
{
	Assert(NULL!=parent);
	Assert(NULL==symtable);
	symtable=new SymTable(parent,"CatchStmt Scope");
	DEBUGSCOPEBEGIN(symtable)
	int i;
	for(i=0;i<actions->NumElements();i++)
		actions->Nth(i)->BuildSymTable(symtable);
	DEBUGSCOPE(symtable)
	DEBUGSCOPEEND(symtable)
	

}

void CatchStmt::DetermineLocation()
{
	Assert(NULL!=symtable);
	DEBUGLOCATIONBEGIN(symtable)
	localoffset=symtable->DetermineLocalLocation(localoffset,-1);
	int i;
	for(i=0;i<actions->NumElements();i++)
		actions->Nth(i)->DetermineLocation();
	DEBUGLOCATION(symtable)
	DEBUGLOCATIONEND(symtable)
	
}

/* 
 * SwitchStmt doesn't create a new scope( see parser.y for detail)
 */
void SwitchStmt::BuildSymTable(SymTable* parent)
{
	Assert(NULL!=parent);
	Assert(NULL==symtable);
	body->BuildSymTable(parent);
}

void SwitchStmt::DetermineLocation()
{
	Assert(NULL==symtable);
	body->DetermineLocation();
}

void SwitchBody::BuildSymTable(SymTable* parent)
{
	Assert(NULL!=parent);
	Assert(NULL==symtable);
	int i;
	for(i=0;i<CaseStmtList->NumElements();i++)
		CaseStmtList->Nth(i)->BuildSymTable(parent);
	defaultStmt->BuildSymTable(parent);
}

void SwitchBody::DetermineLocation()
{
	Assert(NULL==symtable);
	int i;
	for(i=0;i<CaseStmtList->NumElements();i++)
		CaseStmtList->Nth(i)->DetermineLocation();
	defaultStmt->DetermineLocation();
}

void CaseStmt::BuildSymTable(SymTable* parent)
{
	Assert(NULL!=parent);
	Assert(NULL==symtable);
	body->BuildSymTable(parent);	
}

void CaseStmt::DetermineLocation()
{
	Assert(NULL!=symtable);
	body->DetermineLocation();
}


void DefaultStmt::BuildSymTable(SymTable* parent)
{
	Assert(NULL!=parent);
	Assert(NULL==symtable);
	body->BuildSymTable(parent);	
}

void DefaultStmt::DetermineLocation()
{
	Assert(NULL!=symtable);
	body->DetermineLocation();
}

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
	elsetable=NULL;
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
