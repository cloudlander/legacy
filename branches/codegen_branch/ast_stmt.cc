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
extern int localoffset;		// for stmtblock offset determination

extern Location* ehValueLoc;
extern Location* ehTypeLoc;
extern Location* ehDimLoc;

extern const char* labelIntTypeObject;
extern const char* labelDoubleTypeObject;
extern const char* labelBoolTypeObject;
extern const char* labelStringTypeObject;

List<char*> exitLabelStack;	// for break stmt 

Location* testExpr=NULL;			// for switch stmt

List<char*> tryExitLabelStack;	// for try stmt

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

Location* StmtBlock::GenTac(CodeGenerator* cg,SymTable* symtbl)
{
	Assert(cg && symtbl);
	for(int i=0;i<stmts->NumElements();i++)
		stmts->Nth(i)->GenTac(cg,symtable);
	return NULL;
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

Location* IfStmt::GenTac(CodeGenerator* cg,SymTable* symtbl)
{
	Assert(cg && symtbl);
	Location* testresult=test->GenTac(cg,symtbl);

	if(testresult->IsPointer())
		testresult=cg->GenLoad(testresult,0,symtbl);

	char* thenlabel=NULL;
	char* ifend=cg->NewLabel();
	if(elseBody)
	{
		thenlabel=cg->NewLabel();
		cg->GenIfZ(testresult,thenlabel);
	}
	else
		cg->GenIfZ(testresult,ifend);
	body->GenTac(cg,symtbl);	
	if(elseBody)
	{
		cg->GenGoto(ifend);		
		cg->GenLabel(thenlabel);
		elseBody->GenTac(cg,symtbl);
	}
	cg->GenLabel(ifend);
	return NULL;
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

Location* WhileStmt::GenTac(CodeGenerator* cg,SymTable* symtbl)
{
	Assert(cg && symtbl);
	char* whileend=cg->NewLabel();
	char* whilebegin=cg->NewLabel();

	exitLabelStack.Append(whileend);

	cg->GenLabel(whilebegin);
	Location* testresult=test->GenTac(cg,symtbl);
	
	if(testresult->IsPointer())
		testresult=cg->GenLoad(testresult,0,symtbl);

	cg->GenIfZ(testresult,whileend);
	body->GenTac(cg,symtbl);
	cg->GenGoto(whilebegin);
	cg->GenLabel(whileend);

	exitLabelStack.RemoveAt(exitLabelStack.NumElements()-1);
	return NULL;
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

// for stmt's scope not very sure
Location* ForStmt::GenTac(CodeGenerator* cg,SymTable* symtbl)
{
	Assert(cg && symtbl);
	init->GenTac(cg,symtbl);
	char* forend=cg->NewLabel();
	char* forbegin=cg->NewLabel();
	
	exitLabelStack.Append(forend);
	
	cg->GenLabel(forbegin);
	Location* testresult=test->GenTac(cg,symtbl);
	
	if(testresult->IsPointer())
		testresult=cg->GenLoad(testresult,0,symtbl);

	cg->GenIfZ(testresult,forend);
	body->GenTac(cg,symtbl);
	step->GenTac(cg,symtbl);
	cg->GenGoto(forbegin);
	cg->GenLabel(forend);

	exitLabelStack.RemoveAt(exitLabelStack.NumElements()-1);
	
	return NULL;
}
	
Location* BreakStmt::GenTac(CodeGenerator* cg,SymTable* symtbl)
{
	Assert(cg && symtbl);
	if(0==exitLabelStack.NumElements())
	{
		Failure("break not valid");
		return NULL;
	}
	cg->GenGoto(exitLabelStack.Nth(exitLabelStack.NumElements()-1));
	return NULL;
}

Location* ReturnStmt::GenTac(CodeGenerator* cg,SymTable* symtbl)
{
	Assert(cg && symtbl);
	Location* tmp=expr->GenTac(cg,symtbl);
	if(tmp && tmp->IsPointer())
		tmp=cg->GenLoad(tmp,0,symtbl);
	cg->GenReturn(tmp);
	return NULL;
}

Location* PrintStmt::GenTac(CodeGenerator* cg,SymTable* symtbl)
{
	Assert(cg && symtbl);
	if(NULL==args)
		return NULL;
	for(int i=0;i<args->NumElements();i++)
	{
		Type* argType=args->Nth(i)->GetType(symtbl);
		Assert(!argType->IsEquivalentTo(Type::errorType));
		Location* tmp=args->Nth(i)->GenTac(cg,symtbl);
		if(tmp->IsPointer())
			tmp=cg->GenLoad(tmp,0,symtbl);
		if(argType->IsEquivalentTo(Type::intType))
			cg->GenBuiltInCall(PrintInt,tmp,NULL,symtbl);
		else if(argType->IsEquivalentTo(Type::doubleType))
			cg->GenBuiltInCall(PrintDouble,tmp,NULL,symtbl);
		else if(argType->IsEquivalentTo(Type::stringType))
			cg->GenBuiltInCall(PrintString,tmp,NULL,symtbl);
		else if(argType->IsEquivalentTo(Type::boolType))
			cg->GenBuiltInCall(PrintBool,tmp,NULL,symtbl);
	/* Add object address print support, just like ansi C's "%x" style */
		else if(typeid(NamedType)==typeid(*argType))
			cg->GenBuiltInCall(PrintInt,tmp,NULL,symtbl);
	/* Pending ..  Add array print support */
		else
			Assert(0);
	}
	return NULL;
}
	

void TryStmt::BuildSymTable(SymTable* parent)
{
	Assert(NULL!=parent);
	Assert(NULL==symtable);
/*
	symtable=new SymTable(parent,"Try Scope");
	DEBUGSCOPEBEGIN(symtable)
*/
	int i;
	for(i=0;i<stmts->NumElements();i++)
		stmts->Nth(i)->BuildSymTable(parent);
	for(i=0;i<catchblocks->NumElements();i++)
		catchblocks->Nth(i)->BuildSymTable(parent);
/*
	DEBUGSCOPE(symtable)
	DEBUGSCOPEEND(symtable)
*/
}

void TryStmt::DetermineLocation()
{
/*
	Assert(NULL!=symtable);
	DEBUGLOCATIONBEGIN(symtable)
	localoffset=symtable->DetermineLocalLocation(localoffset,-1);
*/
	int i;
	for(i=0;i<stmts->NumElements();i++)
		stmts->Nth(i)->DetermineLocation();
	for(i=0;i<catchblocks->NumElements();i++)
		catchblocks->Nth(i)->DetermineLocation();
/*
	DEBUGLOCATION(symtable)
	DEBUGLOCATIONEND(symtable)
*/	
}

Location* TryStmt::GenTac(CodeGenerator* cg,SymTable* symtbl)
{
	char* labelCatchBlock=cg->NewLabel();
	char* labelExitTry=cg->NewLabel();

	cg->GenBeginTry(labelCatchBlock);

	int i;
	for(i=0;i<stmts->NumElements();i++)
		stmts->Nth(i)->GenTac(cg,symtbl);
	
	cg->GenEndTry();

	tryExitLabelStack.Append(labelExitTry);

	cg->GenGoto(labelExitTry);
	
	cg->GenLabel(labelCatchBlock);

	for(i=0;i<catchblocks->NumElements();i++)
		catchblocks->Nth(i)->GenTac(cg,symtbl);
	
	/* rethrow the same exception if no matching found */
	cg->GenThrow();
	
	cg->GenLabel(labelExitTry);

	tryExitLabelStack.RemoveAt(tryExitLabelStack.NumElements()-1);
	
	return NULL;
}

void CatchStmt::BuildSymTable(SymTable* parent)
{
	Assert(NULL!=parent);
	Assert(NULL==symtable);
	symtable=new SymTable(parent,"CatchStmt Scope");
	DEBUGSCOPEBEGIN(symtable)
	exception->BuildSymTable(symtable);
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

Location* CatchStmt::GenTac(CodeGenerator* cg, SymTable* symtbl)
{
	Assert(NULL!=symtable);
	Type* ehType=exception->GetType();
	Location* typematch=NULL;
	Location* typeobject=NULL;
	Location* typedim=NULL;
	
	Type* tmpType=ehType;

	while(typeid(ArrayType)==typeid(*tmpType))
		tmpType=static_cast<ArrayType*>(tmpType)->GetElemType();
	
	if(typeid(NamedType)==typeid(*tmpType))
	{
		char typeLabel[100];
		const char* name=static_cast<NamedType*>(tmpType)->GetName();
		strcpy(typeLabel,name);	
		strcat(typeLabel,"_Type");
		typeobject=cg->GenLoadLabel(typeLabel,symtable);
	}
	else if(tmpType->IsEquivalentTo(Type::intType))
	{
		typeobject=cg->GenLoadLabel(labelIntTypeObject,symtable);
	}
	else if(tmpType->IsEquivalentTo(Type::doubleType))
	{
		typeobject=cg->GenLoadLabel(labelDoubleTypeObject,symtable);
	}
	else if(tmpType->IsEquivalentTo(Type::boolType))
	{
		typeobject=cg->GenLoadLabel(labelBoolTypeObject,symtable);
	}
	else if(tmpType->IsEquivalentTo(Type::stringType))
	{
		typeobject=cg->GenLoadLabel(labelStringTypeObject,symtable);
	}
	else
		Assert(0);

	if(typeid(ArrayType)==typeid(*ehType))
		typedim=cg->GenLoadConstant(static_cast<ArrayType*>(ehType)->GetDim(),symtable);
	else
		typedim=cg->GenLoadConstant(0,symtable);

	typematch=cg->GenThunkCall(IsKindOf,typeobject,typedim,symtbl);

	char* catchEnd=cg->NewLabel();
	
	cg->GenIfZ(typematch,catchEnd);
	
	cg->GenAssign(symtable->Find(exception->GetName())->GetLocation(),ehValueLoc);

	int i;
	for(i=0;i<actions->NumElements();i++)
		actions->Nth(i)->GenTac(cg,symtable);

	cg->GenGoto(tryExitLabelStack.Nth(tryExitLabelStack.NumElements()-1));
	
	cg->GenLabel(catchEnd);
	return NULL;
}

Location* ThrowStmt::GenTac(CodeGenerator* cg,SymTable* symtbl)
{
	Location* tmp=exception->GenTac(cg,symtbl);
	if(tmp->IsPointer())
		tmp=cg->GenLoad(tmp,0,symtbl);
	cg->GenAssign(ehValueLoc,tmp);
	
	Location* typeobject=NULL;
	Location* typedim=NULL;
	
	Type* ehType=exception->GetType(symtbl);

	Assert(! ehType->IsEquivalentTo(Type::nullType));
	
	Type* tmpType=ehType;

	while(typeid(ArrayType)==typeid(*tmpType))
		tmpType=static_cast<ArrayType*>(tmpType)->GetElemType();
	
	if(typeid(NamedType)==typeid(*ehType) && typeid(NamedType)==typeid(*tmpType))		// only determine type object dynamiclly for object
	{
		Location *vtableaddr=cg->GenLoad(ehValueLoc,0,symtbl);
		typeobject=cg->GenLoad(vtableaddr,-4,symtbl);
//		cg->GenBuiltInCall(PrintInt,ehTypeLoc,NULL,symtbl);
	}
	else if(typeid(ArrayType)==typeid(*ehType) && typeid(NamedType)==typeid(*tmpType))	// staticly for array
	{
		char typeLabel[100];
		const char* name=static_cast<NamedType*>(tmpType)->GetName();
		strcpy(typeLabel,name);	
		strcat(typeLabel,"_Type");
		typeobject=cg->GenLoadLabel(typeLabel,symtbl);
	}
	else if(tmpType->IsEquivalentTo(Type::intType))
	{
		typeobject=cg->GenLoadLabel(labelIntTypeObject,symtbl);
	}
	else if(tmpType->IsEquivalentTo(Type::doubleType))
	{
		typeobject=cg->GenLoadLabel(labelDoubleTypeObject,symtbl);
	}
	else if(tmpType->IsEquivalentTo(Type::boolType))
	{
		typeobject=cg->GenLoadLabel(labelBoolTypeObject,symtbl);
	}
	else if(tmpType->IsEquivalentTo(Type::stringType))
	{
		typeobject=cg->GenLoadLabel(labelStringTypeObject,symtbl);
	}
	else
		Assert(0);

	cg->GenAssign(ehTypeLoc,typeobject);
	if(typeid(ArrayType)==typeid(*ehType))
		typedim=cg->GenLoadConstant(static_cast<ArrayType*>(ehType)->GetDim(),symtbl);
	else
		typedim=cg->GenLoadConstant(0,symtbl);
	cg->GenAssign(ehDimLoc,typedim);
	
	cg->GenThrow();
	return NULL;
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

Location* SwitchStmt::GenTac(CodeGenerator* cg,SymTable* symtbl)
{
	Assert(!test->GetType(symtbl)->IsEquivalentTo(Type::errorType));
	Assert(test->GetType(symtbl)->IsEquivalentTo(Type::intType));
	testExpr=test->GenTac(cg,symtbl);
	if(testExpr->IsPointer())
		testExpr=cg->GenLoad(testExpr,0,symtbl);
	
	char* switchexit=cg->NewLabel();
	exitLabelStack.Append(switchexit);
			
	body->GenTac(cg,symtbl);
	cg->GenLabel(switchexit);
	
	exitLabelStack.RemoveAt(exitLabelStack.NumElements()-1);

	return NULL;
}

void SwitchBody::BuildSymTable(SymTable* parent)
{
	Assert(NULL!=parent);
	Assert(NULL==symtable);
	int i;
	for(i=0;i<CaseStmtList->NumElements();i++)
		CaseStmtList->Nth(i)->BuildSymTable(parent);
	if(defaultStmt)
		defaultStmt->BuildSymTable(parent);
}

void SwitchBody::DetermineLocation()
{
	Assert(NULL==symtable);
	int i;
	for(i=0;i<CaseStmtList->NumElements();i++)
		CaseStmtList->Nth(i)->DetermineLocation();
	if(defaultStmt)
		defaultStmt->DetermineLocation();
}

Location* SwitchBody::GenTac(CodeGenerator* cg,SymTable* symtbl)
{
	int i;
	for(i=0;i<CaseStmtList->NumElements();i++)
		CaseStmtList->Nth(i)->GenTac(cg,symtbl);
	if(defaultStmt)
		defaultStmt->GenTac(cg,symtbl);
	return NULL;
}

void CaseStmt::BuildSymTable(SymTable* parent)
{
	Assert(NULL!=parent);
	Assert(NULL==symtable);
	body->BuildSymTable(parent);	
}

void CaseStmt::DetermineLocation()
{
	Assert(NULL==symtable);
	body->DetermineLocation();
}

Location* CaseStmt::GenTac(CodeGenerator* cg,SymTable* symtbl)
{
	Assert(testExpr);
	Location* tmp=test->GenTac(cg,symtbl);
	Location* equal=cg->GenBinaryOp("==",testExpr,tmp,symtbl);
	char* caseexit=cg->NewLabel();
	cg->GenIfZ(equal,caseexit);
	body->GenTac(cg,symtbl);
	cg->GenGoto(caseexit);
	cg->GenLabel(caseexit);
	return NULL;
}
	
void DefaultStmt::BuildSymTable(SymTable* parent)
{
	Assert(NULL!=parent);
	Assert(NULL==symtable);
	body->BuildSymTable(parent);	
}

void DefaultStmt::DetermineLocation()
{
	Assert(NULL==symtable);
	body->DetermineLocation();
}

Location* DefaultStmt::GenTac(CodeGenerator* cg,SymTable* symtbl)
{
	body->GenTac(cg,symtbl);
	cg->GenGoto(exitLabelStack.Nth(exitLabelStack.NumElements()-1));
	return NULL;
}


//////////////////////////////////////////////////

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
