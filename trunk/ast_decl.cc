/* File: ast_decl.cc
 * -----------------
 * Implementation of Decl node classes.
 */
#include "ast_decl.h"
#include "ast_type.h"
#include "ast_stmt.h"
#include "utility.h"
#include "symtable.h"
#include "errors.h"

 
/* implemention of PP4 start */
int localoffset;
/* vardecls don't pull a new scope, it'll just insert itself to the parent symbol table */
void VarDecl::BuildSymTable(SymTable* parent)
{
	Symbol* sym=parent->Find(id->GetName());
	if(NULL!=sym)
	{
		ReportError::DeclConflict(this,sym->GetDecl());
		return;
	}
	else
		parent->Append(id->GetName(),this);
}

bool VarDecl::Check(SymTable* symtbl)
{
	Symbol* sym=symtbl->Find(id->GetName());
	Assert(NULL!=sym);
	if(!type->Check(symtbl))		// error has been reported
	{
		type=Type::errorType;		// set as error
		return false;
	}
	return true;
}
/* 
 * as classdecl enters a new scope,class field will be inserted to its scope 
 * also, it checkes its name into parent symbol table
 * iteratively build all the methods' symbol table 
 * 
 */ 
void ClassDecl::BuildSymTable(SymTable* parent)
{
	Assert(NULL==symtable);
	Assert(NULL!=parent);
	Symbol* sym=parent->Find(id->GetName());
	if(NULL!=sym)
	{
		ReportError::DeclConflict(this,sym->GetDecl());
		return;
	}
	else
		parent->Append(id->GetName(),this);
	symtable=new SymTable(parent,id->GetName());
	DEBUGSCOPEBEGIN(symtable)
	for(int i=0;i<members->NumElements();i++)
		members->Nth(i)->BuildSymTable(symtable);
	DEBUGSCOPE(symtable)
	DEBUGSCOPEEND(symtable)
}

void ClassDecl::DetermineLocation()
{
	DEBUGLOCATIONBEGIN(symtable)
	if(NULL==extends)
		symtable->DetermineClassLocation(NULL,this);		// base class
	else
	{
		Symbol* parent=GetGlobalSymTable()->Find(extends->GetName());
		if(NULL==parent)
			symtable->DetermineClassLocation(NULL,this);	// error, but consider it as a base class
		symtable->DetermineClassLocation(static_cast<ClassDecl*>(parent->GetDecl()),this);		// deriving class
	}
	DEBUGLOCATION(symtable)
	if(IsDebugOn("location"))
	{
		fprintf(stderr,"Start of dumping VTABLE:\n");
		for(int i=0;i<vtable->NumElements();i++)
			fprintf(stderr,"--- %s ---\n",vtable->Nth(i));
		fprintf(stderr,"End of dumping VTABLE\n");
	}
	DEBUGLOCATIONEND(symtable)
}

const char* ClassDecl::GetExtendClassName()
{
		if(extends)
			return extends->GetName();
		else
			return NULL;
}

const char* ClassDecl::GetClassName()
{
	return id->GetName();
}

bool ClassDecl::Check(SymTable* symtbl)
{
	Assert(symtable);
	bool ret=true;
	for(int i=0;i<members->NumElements();i++)
		if(false==members->Nth(i)->Check(symtable))
			ret=false;
	return ret;
}

void FnDecl::BuildSymTable(SymTable* parent)
{
	Assert(NULL==symtable);
	Assert(NULL!=parent);
	Symbol* sym=parent->Find(id->GetName());
	if(NULL!=sym)
	{
		ReportError::DeclConflict(this,sym->GetDecl());
	// set type as error (bug fixed) !
		type=Type::errorType;
		return;
	}
	else
		parent->Append(id->GetName(),this);
	symtable=new SymTable(parent,id->GetName());
	DEBUGSCOPEBEGIN(symtable)
	Assert(NULL==formalTable);
	formalTable=new SymTable(symtable,"Formal's scope");
	DEBUGSCOPEBEGIN(formalTable)
	int i;
	for(i=0;i<formals->NumElements();i++)
		formals->Nth(i)->BuildSymTable(formalTable);
	/* compiler generates "this" parameter */
	if(parent->Find(id->GetName())->IsMethod())
	{
		// set the actual Decl* of "this"
		formalTable->Append("this",new VarDecl(new Identifier(*location,"this"),
					new NamedType(new Identifier(*(GetParent()->GetLocation()),		// set the actual location
					static_cast<ClassDecl*>(GetParent())->GetClassName()))));
	}
	DEBUGSCOPE(formalTable)
	DEBUGSCOPEEND(formalTable)
	body->BuildSymTable(formalTable);
	DEBUGSCOPE(symtable)
	DEBUGSCOPEEND(symtable)
}
     
void FnDecl::DetermineLocation()
{
	if(type && type->IsEquivalentTo(Type::errorType))
		return;

	DEBUGLOCATIONBEGIN(symtable)
	DEBUGLOCATIONBEGIN(formalTable);
	localoffset=-4;
	(formalTable->DetermineParamLocation(8,1)-8);
	DEBUGLOCATION(formalTable)
	DEBUGLOCATIONEND(formalTable)
	body->DetermineLocation();
	frameSize=-(4+localoffset);
	DEBUGLOCATION(symtable)
	if(IsDebugOn("location"))
		fprintf(stderr,"frameSize=%d\n",frameSize);
	DEBUGLOCATIONEND(symtable)
	localOffset=localoffset;
}

extern bool mainFunctionFound;

bool FnDecl::Check(SymTable* symtbl)
{
	if(type && type->IsEquivalentTo(Type::errorType))
		return false;
	
	bool ret=true;
	int i;
	for(i=0;i<formals->NumElements();i++)
		if(false==formals->Nth(i)->Check(formalTable))
			ret=false;
	
	if(0==strcmp("main",id->GetName()))
		mainFunctionFound=true;
	// return type check will be delayed until return stmt found
	return body->Check(symtbl) && ret;
}

void FnDecl::GenCode(CodeGenerator* cg)
{
	BeginMainFunc* pm=NULL;
	BeginFunc* pf=NULL;
	if(0==strcmp("main",id->GetName()))
		pm=cg->GenBeginMainFunc();
	else
		pf=cg->GenBeginFunc();

	localoffset=localOffset;
	
	if(pm)
		GenTopExceptionHandler(cg);
	else
		body->GenTac(cg,formalTable);
	
	frameSize=-(4+localoffset);

	if(0==strcmp("main",id->GetName()))
		pm->SetFrameSize(frameSize);
	else
		pf->SetFrameSize(frameSize);
	cg->GenEndFunc();
}

extern int bie_size;
extern BuiltInException bie[];

void FnDecl::GenTopExceptionHandler(CodeGenerator* cg)
{
	char* labelCatchBlock=cg->NewLabel();
	char* labelExitTry=cg->NewLabel();

	cg->GenBeginTry(labelCatchBlock);

	body->GenTac(cg,formalTable);
	
	cg->GenEndTry();

	cg->GenGoto(labelExitTry);
	
	cg->GenLabel(labelCatchBlock);

	Location* typematch=NULL;
	Location* typeobject=NULL;
	Location* typedim=NULL;
	int i;
	for(i=0;i<bie_size-1;i++)
	{
		char typeLabel[100];
		strcpy(typeLabel,bie[i].typeName);	
		strcat(typeLabel,"_Type");
		typeobject=cg->GenLoadLabel(typeLabel,formalTable);
		
		typedim=cg->GenLoadConstant(0,formalTable);

		typematch=cg->GenThunkCall(IsKindOf,typeobject,typedim,formalTable);

		char* catchEnd=cg->NewLabel();
	
		cg->GenIfZ(typematch,catchEnd);

		cg->GenBuiltInCall(PrintString,cg->GenLoadConstant(bie[i].errMsg,formalTable),NULL,formalTable);

		cg->GenGoto(labelExitTry);
	
		cg->GenLabel(catchEnd);
	}

	/* catch the unhandled exception if no matching found */
	cg->GenBuiltInCall(PrintString,cg->GenLoadConstant(bie[i].errMsg,formalTable),NULL,formalTable);
	
	cg->GenLabel(labelExitTry);
}

Decl::Decl(Identifier *n) : Node(*n->GetLocation()) {
    Assert(n != NULL);
    (id=n)->SetParent(this); 
}


VarDecl::VarDecl(Identifier *n, Type *t) : Decl(n) {
    Assert(n != NULL && t != NULL);
    (type=t)->SetParent(this);
}
  
void VarDecl::PrintChildren(int indentLevel) { 
   type->Print(indentLevel+1);
   id->Print(indentLevel+1);
}

ClassDecl::ClassDecl(Identifier *n, NamedType *ex, List<Decl*> *m) : Decl(n) {
    // extends can be NULL, impl & mem may be empty lists but cannot be NULL
    Assert(n != NULL && m != NULL);     
    extends = ex;
    if (extends) extends->SetParent(this);
    //(implements=imp)->SetParentAll(this);
    (members=m)->SetParentAll(this);
}

/*
ClassDecl::ClassDecl(Identifier *n, NamedType *ex, List<NamedType*> *imp, List<Decl*> *m) : Decl(n) {
    // extends can be NULL, impl & mem may be empty lists but cannot be NULL
    Assert(n != NULL && imp != NULL && m != NULL);
    extends = ex;
    if (extends) extends->SetParent(this);
    (implements=imp)->SetParentAll(this);
    (members=m)->SetParentAll(this);
}
*/





void ClassDecl::PrintChildren(int indentLevel) {
    id->Print(indentLevel+1);
    if (extends) extends->Print(indentLevel+1, "(extends) ");
    //implements->PrintAll(indentLevel+1, "(implements) ");
    members->PrintAll(indentLevel+1);
}


/*
InterfaceDecl::InterfaceDecl(Identifier *n, List<Decl*> *m) : Decl(n) {
    Assert(n != NULL && m != NULL);
    (members=m)->SetParentAll(this);
}

void InterfaceDecl::PrintChildren(int indentLevel) {
    id->Print(indentLevel+1);
    members->PrintAll(indentLevel+1);
}
*/	
FnDecl::FnDecl(Identifier *n, Type *r, List<VarDecl*> *d) : Decl(n) {
    Assert(n != NULL && r!= NULL && d != NULL);
    (returnType=r)->SetParent(this);
    (formals=d)->SetParentAll(this);
    body = NULL;

	formalTable=NULL;
	frameSize=0;
}

void FnDecl::SetFunctionBody(Stmt *b) { 
    (body=b)->SetParent(this);
}

void FnDecl::PrintChildren(int indentLevel) {
    returnType->Print(indentLevel+1, "(return type) ");
    id->Print(indentLevel+1);
    formals->PrintAll(indentLevel+1, "(formals) ");
    if (body) body->Print(indentLevel+1, "(body) ");
}
