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
		Failure("type not match");
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
		Failure("type not match");
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
		Symbol* symfield=symclass->GetDecl()->GetSymTable()->Find(field->GetName());
		if(NULL==symfield || symfield->IsClassVar())
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
			Symbol* symfield=symclass->GetDecl()->GetSymTable()->Find(field->GetName());
			if(NULL==symfield || symfield->IsClassVar())
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
		Symbol* symfield=symclass->GetDecl()->GetSymTable()->Find(field->GetName());
		if(NULL==symfield || symfield->IsMethod())
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
