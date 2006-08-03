/* File: ast_type.cc
 * -----------------
 * Implementation of type node classes.
 */
#include "ast_type.h"
#include "ast_decl.h"
#include "symtable.h"
 
/* code implementation for type checking */
bool Type::IsCompatibleTo(Type* other)
{
	Assert(typeName);
	if(IsEquivalentTo(Type::nullType))	// null should be compatible to any NamedType,ArrayType
	{
		if(typeid(NamedType)==typeid(*other) || typeid(ArrayType)==typeid(*other))
			return true;
	}
	return false;	
}

bool NamedType::IsCompatibleTo(Type* other)
{
	if(IsEquivalentTo(other))
		return true;
	if(typeid(*this)!=typeid(*other))
		return false;
	
	Symbol* rootsym=GetGlobalSymTable()->Find(static_cast<NamedType*>(other)->GetName());
	Assert(rootsym);
	SymTable* rootsymtbl=static_cast<ClassDecl*>(rootsym->GetDecl())->GetSymTable();
	Assert(rootsymtbl);
	
	Symbol* thissym=GetGlobalSymTable()->Find(this->GetName());
	Assert(thissym);
	SymTable* thissymtbl=static_cast<ClassDecl*>(thissym->GetDecl())->GetSymTable();
	Assert(thissymtbl);

	while(thissymtbl)
	{
		if(thissymtbl == rootsymtbl)
			return true;
		thissymtbl=thissymtbl->GetParent();
	}
	return false;
}

int ArrayType::GetDim()
{
	Type* elem;
	int d=1;
	elem=elemType;
	while(typeid(*elem)==typeid(ArrayType))
	{
		d++;
		elem=static_cast<ArrayType*>(elem)->GetElemType();
	}
	return d;
}

/* Class constants
 * ---------------
 * These are public constants for the built-in base types (int, double, etc.)
 * They can be accessed with the syntax Type::intType. This allows you to
 * directly access them and share the built-in types where needed rather that
 * creates lots of copies.
 */

Type *Type::intType    = new Type("int");
Type *Type::doubleType = new Type("double");
Type *Type::voidType   = new Type("void");
Type *Type::boolType   = new Type("bool");
Type *Type::nullType   = new Type("null");
Type *Type::stringType = new Type("string");
Type *Type::errorType  = new Type("error"); 

Type::Type(const char *n) {
    Assert(n);
    typeName = strdup(n);
}

void Type::PrintChildren(int indentLevel) {
    printf("%s", typeName);
}

	
NamedType::NamedType(Identifier *i) : Type(*i->GetLocation()) {
    Assert(i != NULL);
    (id=i)->SetParent(this);
} 

void NamedType::PrintChildren(int indentLevel) {
    id->Print(indentLevel+1);
}

ArrayType::ArrayType(yyltype loc, Type *et) : Type(loc) {
    Assert(et != NULL);
    (elemType=et)->SetParent(this);
}
void ArrayType::PrintChildren(int indentLevel) {
    elemType->Print(indentLevel+1);
}


