/* File: ast_expr.cc
 * -----------------
 * Implementation of expression node classes.
 */
#include "ast_expr.h"
#include "ast_type.h"
#include "ast_decl.h"
#include "symtable.h"
#include "errors.h"

// code implementation for type system/checking and code gen
Type* ArithmeticExpr::GetType(SymTable* symtbl)
{
	if(type) return type;

	Type *lefttype,*righttype;
	if(left)
		lefttype=left->GetType(symtbl);
	Assert(right);
	righttype=right->GetType(symtbl);

	if(left && lefttype->IsEquivalentTo(Type::intType) &&
	   righttype->IsEquivalentTo(Type::intType) ||
	   lefttype->IsEquivalentTo(Type::doubleType) &&
	   righttype->IsEquivalentTo(Type::doubleType)) 
		return type=lefttype;
	else if(NULL==left && righttype->IsEquivalentTo(Type::intType) ||
			righttype->IsEquivalentTo(Type::doubleType))
		return type=righttype;
	else
	{
		Failure("type not match");
		return type=Type::errorType;
	}
}

Type* RelationalExpr::GetType(SymTable* symtbl)
{
	if(type) return type;

	Type *lefttype,*righttype;
	Assert(left);
	lefttype=left->GetType(symtbl);
	Assert(right);
	righttype=right->GetType(symtbl);

	if(lefttype->IsEquivalentTo(Type::intType) &&
	   righttype->IsEquivalentTo(Type::intType) ||
	   lefttype->IsEquivalentTo(Type::doubleType) &&
	   righttype->IsEquivalentTo(Type::doubleType)) 
		return type=Type::boolType;
	else
	{
		Failure("type not match");
		return type=Type::errorType;
	}
}

Type* EqualityExpr::GetType(SymTable* symtbl)
{
	if(type) return type;

	Type *lefttype,*righttype;
	Assert(left);
	lefttype=left->GetType(symtbl);
	Assert(right);
	righttype=right->GetType(symtbl);

	if(lefttype->IsEquivalentTo(righttype))
		return type=Type::boolType;
	if(lefttype->IsCompatibleTo(righttype))
		return type=Type::boolType;
	if(righttype->IsCompatibleTo(lefttype))
		return type=Type::boolType;
	Failure("type not match");
	return type=Type::errorType;
}
	
Type* LogicalExpr::GetType(SymTable* symtbl)
{
	if(type) return type;

	Type *lefttype,*righttype;
	if(left)
		lefttype=left->GetType(symtbl);
	Assert(right);
	righttype=right->GetType(symtbl);
	
	if(left && lefttype->IsEquivalentTo(Type::boolType)
			&& righttype->IsEquivalentTo(Type::boolType))
		return type=Type::boolType;
	else
		if(righttype->IsEquivalentTo(Type::boolType))
			return type=Type::boolType;

	Failure("type not match");
	return type=Type::errorType;	
}

Type* AssignExpr::GetType(SymTable* symtbl)
{
	if(type) return type;

	Type *lefttype,*righttype;
	Assert(left);
	lefttype=left->GetType(symtbl);
	Assert(right);
	righttype=right->GetType(symtbl);

	if(righttype->IsEquivalentTo(lefttype) ||
		righttype->IsCompatibleTo(lefttype))
		return type=lefttype;
	Failure("type not match");
	return type=Type::errorType;
}

Type* This::GetType(SymTable* symtbl)
{
	if(type) return type;
	
	Symbol* sym=symtbl->Find("this",true);
	if(sym)
	{
		Assert(sym->GetDecl() && sym->GetDecl()->GetType());
		return type=sym->GetDecl()->GetType();
	}
	else
	{
		ReportError::ThisOutsideClassScope(this);
		return type=Type::errorType;
	}
}

Type* ArrayAccess::GetType(SymTable* symtbl)
{
	if(type) return type;

	Assert(base);
	Type* basetype=base->GetType(symtbl);
	if(typeid(ArrayType) == typeid(*basetype))
		return type=static_cast<ArrayType*>(basetype)->GetElemType();
	else
	{
		Failure("array type not match");
		return type=Type::errorType;
	}
}

Type* FieldAccess::GetType(SymTable* symtbl)
{
	if(type) return type;

	Type* basetype;
	if(base)
	{
		basetype=base->GetType(symtbl);
		if(typeid(NamedType)!=typeid(*basetype))
		{
			Failure("base not object type");
			return type=Type::errorType;
		}
		const char* classname=static_cast<NamedType*>(basetype)->GetName();			
		Symbol* symclass=GetGlobalSymTable()->Find(classname);
		if(NULL==symclass)
		{
			Failure("type of base is an invalid class");
			return type=Type::errorType;
		}
		Symbol* symfield=symclass->GetDecl()->GetSymTable()->Find(field->GetName(),true);
		if(NULL==symfield || ! symfield->IsClassVar())
		{
			Failure("field not found in class");
			return type=Type::errorType;
		}
		return type=symfield->GetDecl()->GetType();
	}
	else
	{
		Symbol* symfield=symtbl->Find(field->GetName(),true);
		if(NULL==symfield)	// try "this" as base , this seems omittable !!!!
		{
			Symbol* symthis=symtbl->Find("this",true);
			if(NULL==symthis)
			{
				Failure("symbol not found");
				return type=Type::errorType;
			}
			Type* basetype=symthis->GetDecl()->GetType();
			const char* classname=static_cast<NamedType*>(basetype)->GetName();
			Symbol* symclass=GetGlobalSymTable()->Find(classname);
			if(NULL==symclass)
			{
				Failure("type of base is an invalid class");
				return type=Type::errorType;
			}
			Symbol* symfield=symclass->GetDecl()->GetSymTable()->Find(field->GetName(),true);
			if(NULL==symfield || ! symfield->IsClassVar())
			{
				Failure("field not found in class");
				return type=Type::errorType;
			}
			return type=symfield->GetDecl()->GetType();
		}
		return type=symfield->GetDecl()->GetType();
	}
}


Type* Call::GetType(SymTable* symtbl)
{
	if(type) return type;

	Type* basetype;
	if(base)
	{
		basetype=base->GetType(symtbl);
		if(typeid(ArrayType)==typeid(*basetype))
		{
			if(0==strcmp("length",field->GetName()))
				return type=Type::intType;
			else
			{
				Failure("length could only occur under array");
				return type=Type::voidType;
			}
		}
		if(typeid(NamedType)!=typeid(*basetype))
		{
			Failure("base not object type");
			return type=Type::errorType;
		}
		const char* classname=static_cast<NamedType*>(basetype)->GetName();			
		Symbol* symclass=GetGlobalSymTable()->Find(classname);
		if(NULL==symclass)
		{
			Failure("type of base is an invalid class");
			return type=Type::errorType;
		}
		Symbol* symfield=symclass->GetDecl()->GetSymTable()->Find(field->GetName(),true);
		if(NULL==symfield || ! symfield->IsMethod())
		{
			Failure("field not found in class");
			return type=Type::errorType;
		}
		return type=symfield->GetDecl()->GetType();
	}
	else
	{
		Symbol* symfield=symtbl->Find(field->GetName(),true);
		if(NULL==symfield)	// try "this" as base
		{
			Failure("field not found in class");
			return type=Type::errorType;
		}
		return type=symfield->GetDecl()->GetType();
	}
}

Type* ConditionalExpr::GetType(SymTable* symtbl)
{
	if(type) return type;
	Type *expr1type,*expr2type,*expr3type;
	expr1type=expr1->GetType(symtbl);
	Assert(expr1type);
	expr2type=expr2->GetType(symtbl);
	Assert(expr2type);
	expr3type=expr3->GetType(symtbl);
	Assert(expr3type);

	if(!expr1type->IsEquivalentTo(Type::boolType))
	{
		Failure("conditional expr1 not bool type!");
		return type=Type::errorType;
	}
	if(expr2type->IsEquivalentTo(expr3type))
		return type=expr2type;
	else
	{
		Failure("conditional expr2's type not equal to expr3's type");
		return type=Type::errorType;
	}
}

Location* IntConstant::GenTac(CodeGenerator* cg,SymTable* symtbl)
{
	Assert(cg && symtbl);
	return cg->GenLoadConstant(value,symtbl);
}

Location* DoubleConstant::GenTac(CodeGenerator* cg,SymTable* symtbl)
{
	Assert(cg && symtbl);
	Failure("DOUBLE NOT SUPPORT!");
	return NULL;
}

Location* BoolConstant::GenTac(CodeGenerator* cg,SymTable* symtbl)
{
	Assert(cg && symtbl);
	return cg->GenLoadConstant(value ? 0xFFFFFFFF : 0,symtbl);
}

Location* StringConstant::GenTac(CodeGenerator* cg,SymTable* symtbl)
{
	Assert(cg && symtbl);
	return cg->GenLoadConstant(value,symtbl);
}

Location* NullConstant::GenTac(CodeGenerator* cg,SymTable* symtbl)
{
	Assert(cg && symtbl);
	return cg->GenLoadConstant(0,symtbl);
}

Location* ArithmeticExpr::GenTac(CodeGenerator* cg,SymTable* symtbl)
{
	Assert(cg && symtbl);
	Assert(!GetType(symtbl)->IsEquivalentTo(Type::errorType));
	Location *lhs,*rhs;
	if(left)
		lhs=left->GenTac(cg,symtbl);
	else
		lhs=cg->GenLoadConstant(0,symtbl);
	rhs=right->GenTac(cg,symtbl);
	return cg->GenBinaryOp(op->GetOperName(),lhs,rhs,symtbl);
}


Location* RelationalExpr::GenTac(CodeGenerator* cg,SymTable* symtbl)
{
	Assert(cg && symtbl);
	Assert(!GetType(symtbl)->IsEquivalentTo(Type::errorType));
	Location *lhs,*rhs;
	lhs=left->GenTac(cg,symtbl);
	rhs=right->GenTac(cg,symtbl);
	const char* opname=op->GetOperName();
	if(0==strncmp("<",opname,1))
		return cg->GenBinaryOp("<",lhs,rhs,symtbl);
	if(0==strncmp("<=",opname,2))
		return cg->GenBinaryOp("<=",lhs,rhs,symtbl);
	if(0==strncmp(">",opname,1))
		return cg->GenBinaryOp("<",rhs,lhs,symtbl);
	if(0==strncmp(">=",opname,2))
		return cg->GenBinaryOp("<=",rhs,lhs,symtbl);
	Failure("SHOULD NOT REACH HERE");
	return NULL;
}

Location* EqualityExpr::GenTac(CodeGenerator* cg,SymTable* symtbl)
{
	Assert(cg && symtbl);
	Assert(!GetType(symtbl)->IsEquivalentTo(Type::errorType));
	Location *lhs,*rhs;
	lhs=left->GenTac(cg,symtbl);
	rhs=right->GenTac(cg,symtbl);
	return cg->GenBinaryOp(op->GetOperName(),lhs,rhs,symtbl);
}


Location* LogicalExpr::GenTac(CodeGenerator* cg,SymTable* symtbl)
{
	Assert(cg && symtbl);
	Assert(!GetType(symtbl)->IsEquivalentTo(Type::errorType));
	Location *lhs,*rhs;
	Assert(right);
	rhs=right->GenTac(cg,symtbl);
	if(NULL==left)
		return cg->GenBinaryOp(op->GetOperName(),new Location(fpRelative,0,"blah"),rhs,symtbl);
	else
		lhs=left->GenTac(cg,symtbl);
	return cg->GenBinaryOp(op->GetOperName(),lhs,rhs,symtbl);
}

Location* AssignExpr::GenTac(CodeGenerator* cg,SymTable* symtbl)
{
	Assert(cg && symtbl);
	Assert(!GetType(symtbl)->IsEquivalentTo(Type::errorType));
	Location *lvalue,*rvalue;
	lvalue=left->GenTac(cg,symtbl);
	rvalue=right->GenTac(cg,symtbl);
	return cg->GenAssign(lvalue,rvalue);	
}
	
Location* ExprStmt::GenTac(CodeGenerator* cg,SymTable* symtbl)
{
	Assert(cg && symtbl);
	Assert(expr);
	return expr->GenTac(cg,symtbl);
}

Location* NewExpr::GenTac(CodeGenerator* cg,SymTable* symtbl)
{
	Assert(cg && symtbl);
	Assert(cType);
	Assert(!GetType(symtbl)->IsEquivalentTo(Type::errorType));
	Assert(typeid(ClassDecl)==typeid(*(symtbl->Find(cType->GetName(),true)->GetDecl())));
	int classsize=static_cast<ClassDecl*>(symtbl->Find(cType->GetName(),true)->GetDecl())->GetVarOffset();
	const char* vtaddr=static_cast<ClassDecl*>(symtbl->Find(cType->GetName(),true)->GetDecl())->GetVtableName();
	Assert(classsize>=4);
	Location* sizeofclass=cg->GenLoadConstant(classsize,symtbl);
	Location* vtableaddr=cg->GenLoadLabel(vtaddr,symtbl);
	return cg->GenThunkCall(NewClass,sizeofclass,vtableaddr,NULL,symtbl);
}

Location* NewArrayExpr::GenTac(CodeGenerator* cg,SymTable* symtbl)
{
	Assert(cg && symtbl);
	Assert(size && elemType);
	Assert(!GetType(symtbl)->IsEquivalentTo(Type::errorType));
	Location* arraysize=size->GenTac(cg,symtbl);
	if(typeid(*elemType)==typeid(NamedType))
	{
		Assert(typeid(ClassDecl)==typeid(*(symtbl->Find(static_cast<NamedType*>(elemType)->GetName(),true)->GetDecl())));
		int classsize=static_cast<ClassDecl*>(symtbl->Find(static_cast<NamedType*>(elemType)->GetName(),true)->GetDecl())->GetVarOffset();
		const char* vtaddr=static_cast<ClassDecl*>(symtbl->Find(static_cast<NamedType*>(elemType)->GetName(),true)->GetDecl())->GetVtableName();
		Assert(classsize>=4);
		Location* sizeofclass=cg->GenLoadConstant(classsize,symtbl);
		Location* vtableaddr=cg->GenLoadLabel(vtaddr,symtbl);
		return cg->GenThunkCall(NewClassArray,arraysize,sizeofclass,vtableaddr,symtbl);
	}
	else
	{
		// in current implementation, all array/basic type are 4 bytes
		Location *typesize=cg->GenLoadConstant(4,symtbl);
		return cg->GenThunkCall(NewArray,arraysize,typesize,NULL,symtbl);
	}
}		
	
Location* ReadLineExpr::GenTac(CodeGenerator* cg,SymTable* symtbl)
{
	Assert(cg && symtbl);
	return cg->GenBuiltInCall(ReadLine,NULL,NULL,symtbl);
}


Location* ReadIntegerExpr::GenTac(CodeGenerator* cg,SymTable* symtbl)
{
	Assert(cg && symtbl);
	return cg->GenBuiltInCall(ReadInt,NULL,NULL,symtbl);
}

Location* This::GenTac(CodeGenerator* cg,SymTable* symtbl)
{
	Assert(cg && symtbl);
	Assert(!GetType(symtbl)->IsEquivalentTo(Type::errorType));
	if(typeid(NamedType)==typeid(*GetType(symtbl)))
		return symtbl->Find("this",true)->GetLocation();
	else
	{
		Failure("not vaild this");
		return NULL;
	}
}

Location* Call::GenTac(CodeGenerator* cg,SymTable* symtbl)
{
	Assert(cg && symtbl);
	Assert(field && actuals);

	Assert(!GetType(symtbl)->IsEquivalentTo(Type::errorType));

	Location* baseaddr=NULL;
	
	int vfuncOffset;
	
	// check if it has return value
	bool hasretval=false;
	Type* basetype;
	
	if(base)
	{
		basetype=base->GetType(symtbl);
		if(typeid(NamedType)==typeid(*basetype))	// virtual call
		{
			const char* classname=static_cast<NamedType*>(basetype)->GetName();
			Symbol* symclass=GetGlobalSymTable()->Find(classname);
			Assert(symclass);
			SymTable* symtblclass=symclass->GetDecl()->GetSymTable();
			Assert(symtblclass->Find(field->GetName(),true));
			
			vfuncOffset=symtblclass->Find(field->GetName(),true)->GetLocation()->GetOffset();
			hasretval= ! static_cast<FnDecl*>(symtblclass->Find(field->GetName(),true)->GetDecl())->
									GetType()->IsEquivalentTo(Type::voidType);
			baseaddr=base->GenTac(cg,symtbl);
		}	
		else if(typeid(ArrayType)==typeid(*basetype))	// array.length
		{
			printf("currently not implemented!");
			return new Location(fpRelative,2000,"AAA");
		}
	}
	else
	{
		Symbol* symfunc=symtbl->Find(field->GetName(),true);
		if(symfunc && symfunc->IsMethod())	// this.call
		{
			vfuncOffset=symfunc->GetLocation()->GetOffset();
			hasretval= ! static_cast<FnDecl*>(symfunc->GetDecl())->
									GetType()->IsEquivalentTo(Type::voidType);
			baseaddr=symtbl->Find("this",true)->GetLocation();
		}
		else if(symfunc && symfunc->IsFunction())	// global function call
		{
			hasretval= ! static_cast<FnDecl*>(symfunc->GetDecl())->
									GetType()->IsEquivalentTo(Type::voidType);
		}
		else if(symfunc)
		{
			Failure("illegal call");
			return NULL;
		}
	}
	Location* result=NULL;	
	int i;
	Location* paramArray[actuals->NumElements()];
	for(i=0;i<actuals->NumElements();i++)
		paramArray[i]=actuals->Nth(i)->GenTac(cg,symtbl);

	if(baseaddr)	// virtual call
	{
		Location *vtbaddr=cg->GenLoad(baseaddr,0,symtbl);
		Location *vfuncaddr=cg->GenLoad(vtbaddr,vfuncOffset,symtbl);
		
		cg->GenPushParam(baseaddr);
		i=actuals->NumElements()-1;
		for(;i>=0;i--)
			cg->GenPushParam(paramArray[i]);
		result=cg->GenACall(vfuncaddr,hasretval,symtbl);
	}
	else
	{
		// check if it's builtin call (not implemented)
		if(NULL==GetGlobalSymTable()->Find(field->GetName(),true))
		{
			printf("################ %s  BUILTIN CALL NOT IMPLEMENTED!! ####################################\n",field->GetName());
			return new Location(fpRelative,1000,"BBB");
		}
		
		Assert(GetGlobalSymTable()->Find(field->GetName(),true));
		i=actuals->NumElements()-1;
		for(;i>=0;i--)
			cg->GenPushParam(paramArray[i]);

		// global function call
		result=cg->GenLCall(static_cast<FnDecl*>(GetGlobalSymTable()->Find(field->GetName(),true)->GetDecl())->GetMangledName(),hasretval,symtbl);
		
	}	
	cg->GenPopParams(4 * actuals->NumElements());
	return result;

}
		
		
Location* FieldAccess::GenTac(CodeGenerator* cg,SymTable* symtbl)
{
	Assert(cg && symtbl);
	Assert(field);
	Assert(!GetType(symtbl)->IsEquivalentTo(Type::errorType));

	int fieldoffset;
	Location* baseaddr=NULL;
	if(base)
	{
		Type* basetype;
		basetype=base->GetType(symtbl);
		if(typeid(NamedType)==typeid(*basetype))
		{
			const char* classname=static_cast<NamedType*>(basetype)->GetName();
			Symbol* symclass=GetGlobalSymTable()->Find(classname);
			Assert(symclass);
			SymTable* symtblclass=symclass->GetDecl()->GetSymTable();
			Assert(symtblclass->Find(field->GetName(),true));
			
			fieldoffset=symtblclass->Find(field->GetName(),true)->GetLocation()->GetOffset();
			baseaddr=base->GenTac(cg,symtbl);
		}
		else
		{
			Failure("field not found!");
			return NULL;
		}
	}
	else
	{
		Symbol* sym;
		if(NULL!=(sym=symtbl->Find(field->GetName(),true)))
		{
			if(sym->IsClassVar())
			{
				fieldoffset=sym->GetLocation()->GetOffset();
				baseaddr=symtbl->Find("this",true)->GetLocation();
			}
			else if(sym->IsLocalVar())
			{
				return sym->GetLocation();
			}
			else if(sym->IsGlobalVar())
			{
			}
		}
		else
		{
			Failure("sym not found");
			return NULL;
		}
	}
	if(baseaddr)
		return cg->GenLoad(baseaddr,fieldoffset,symtbl);
	else
		return cg->GenLoad(symtbl->Find(field->GetName(),true)->GetLocation(),0,symtbl);
}	

Location* ArrayAccess::GenTac(CodeGenerator* cg,SymTable* symtbl)
{
	Assert(cg && symtbl);
	Assert(base && subscript);
	Assert(!GetType(symtbl)->IsEquivalentTo(Type::errorType));

	Location* subscriptaddr=subscript->GenTac(cg,symtbl);
	Location* baseaddr=base->GenTac(cg,symtbl);
	Location* elemsize=cg->GenLoadConstant(4,symtbl);
	Location* stepsize=cg->GenBinaryOp("*",subscriptaddr,elemsize,symtbl);
	Location* elemaddr=cg->GenBinaryOp("+",baseaddr,stepsize,symtbl);
	
	return cg->GenLoad(elemaddr,0,symtbl);
}


Location* ConditionalExpr::GenTac(CodeGenerator* cg,SymTable* symtbl)
{
	Assert(cg && symtbl);
	Assert(expr1 && expr2 && expr3);
	Assert(!GetType(symtbl)->IsEquivalentTo(Type::errorType));

	Location* testresult=expr1->GenTac(cg,symtbl);
	Location* result=cg->GenTempVar(symtbl);

	char* thenlabel=NULL;
	char* ifend=cg->NewLabel();
	if(expr3)
	{
		thenlabel=cg->NewLabel();
		cg->GenIfZ(testresult,thenlabel);
	}
	else
		cg->GenIfZ(testresult,ifend);

	cg->GenAssign(result,expr2->GenTac(cg,symtbl));	

	if(expr3)
	{
		cg->GenGoto(ifend);		
		cg->GenLabel(thenlabel);
		cg->GenAssign(result, expr3->GenTac(cg,symtbl));
	}
	cg->GenLabel(ifend);
	return result;
}
	
/////////////////////////////////////////////

IntConstant::IntConstant(yyltype loc, int val) : Expr(loc) {
    value = val;
}
void IntConstant::PrintChildren(int indentLevel) { 
    printf("%d", value);
}

DoubleConstant::DoubleConstant(yyltype loc, double val) : Expr(loc) {
    value = val;
}
void DoubleConstant::PrintChildren(int indentLevel) { 
    printf("%g", value);
}

BoolConstant::BoolConstant(yyltype loc, bool val) : Expr(loc) {
    value = val;
}
void BoolConstant::PrintChildren(int indentLevel) { 
    printf("%s", value ? "true" : "false");
}

StringConstant::StringConstant(yyltype loc, const char *val) : Expr(loc) {
    Assert(val != NULL);
    value = strdup(val);
}
void StringConstant::PrintChildren(int indentLevel) { 
    printf("%s",value);
}

Operator::Operator(yyltype loc, const char *tok) : Node(loc) {
    Assert(tok != NULL);
    strncpy(tokenString, tok, sizeof(tokenString));
}

void Operator::PrintChildren(int indentLevel) {
    printf("%s",tokenString);
}

CompoundExpr::CompoundExpr(Expr *l, Operator *o, Expr *r) 
  : Expr(Join(l->GetLocation(), r->GetLocation())) {
    Assert(l != NULL && o != NULL && r != NULL);
    (op=o)->SetParent(this);
    (left=l)->SetParent(this); 
    (right=r)->SetParent(this);
}

CompoundExpr::CompoundExpr(Operator *o, Expr *r) 
  : Expr(Join(o->GetLocation(), r->GetLocation())) {
    Assert(o != NULL && r != NULL);
    left = NULL; 
    (op=o)->SetParent(this);
    (right=r)->SetParent(this);
}

void CompoundExpr::PrintChildren(int indentLevel) {
   if (left) left->Print(indentLevel+1);
   op->Print(indentLevel+1);
   right->Print(indentLevel+1);
}
   
  
ArrayAccess::ArrayAccess(yyltype loc, Expr *b, Expr *s) : LValue(loc) {
    (base=b)->SetParent(this); 
    (subscript=s)->SetParent(this);
}

void ArrayAccess::PrintChildren(int indentLevel) {
    base->Print(indentLevel+1);
    subscript->Print(indentLevel+1, "(subscript) ");
  }
     
FieldAccess::FieldAccess(Expr *b, Identifier *f) 
  : LValue(b? Join(b->GetLocation(), f->GetLocation()) : *f->GetLocation()) {
    Assert(f != NULL); // b can be be NULL (just means no explicit base)
    base = b; 
    if (base) base->SetParent(this); 
    (field=f)->SetParent(this);
}


  void FieldAccess::PrintChildren(int indentLevel) {
    if (base) base->Print(indentLevel+1);
    field->Print(indentLevel+1);
  }

Call::Call(yyltype loc, Expr *b, Identifier *f, List<Expr*> *a) : Expr(loc)  {
    Assert(f != NULL && a != NULL); // b can be be NULL (just means no explicit base)
    base = b;
    if (base) base->SetParent(this);
    (field=f)->SetParent(this);
    (actuals=a)->SetParentAll(this);
}

 void Call::PrintChildren(int indentLevel) {
    if (base) base->Print(indentLevel+1);
    field->Print(indentLevel+1);
    actuals->PrintAll(indentLevel+1, "(actuals) ");
  }
 

NewExpr::NewExpr(yyltype loc, NamedType *c) : Expr(loc) { 
  Assert(c != NULL);
  (cType=c)->SetParent(this);
}

void NewExpr::PrintChildren(int indentLevel) {	
    cType->Print(indentLevel+1);
}

NewArrayExpr::NewArrayExpr(yyltype loc, Expr *sz, Type *et) : Expr(loc) {
    Assert(sz != NULL && et != NULL);
    (size=sz)->SetParent(this); 
    (elemType=et)->SetParent(this);
}

void NewArrayExpr::PrintChildren(int indentLevel) {
    size->Print(indentLevel+1);
    elemType->Print(indentLevel+1);
}

ConditionalExpr::ConditionalExpr(Expr *e1, Operator *o1, Expr *e2, Operator *o2, Expr *e3) 
    : Expr(Join(e1->GetLocation(),e2->GetLocation()))
{
    Assert(e1 != NULL && e2 != NULL && e3 != NULL && o1 != NULL && o2 != NULL);
    (expr1=e1)->SetParent(this);
    (expr2=e2)->SetParent(this);
    (expr3=e3)->SetParent(this);
    (op1=o1)->SetParent(this);
    (op2=o2)->SetParent(this);
}       

void ConditionalExpr::PrintChildren(int indentLevel)
{
    expr1->Print(indentLevel+1);
    op1->Print(indentLevel+1);
    expr2->Print(indentLevel+1);
    op2->Print(indentLevel+1);
    expr3->Print(indentLevel+1);
}
