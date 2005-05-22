/* File: ast_type.h
 * ----------------
 * In our parse tree, Type nodes are used to represent and
 * store type information. The base Type class is used
 * for built-in types, the NamedType for classes ,
 * and the ArrayType for arrays of other types.  
 */
 
#ifndef _H_ast_type
#define _H_ast_type

#include "ast.h"
#include "list.h"
#include <iostream>
class SymTable;

class Type : public Node 
{
  protected:
    char *typeName;

  public :
    static Type  *intType, *doubleType, *boolType, *voidType, *nullType, *stringType, *errorType, *argType/* for main subroutine*/;

    Type(yyltype loc) : Node(loc) {}
    Type(const char *str);
    virtual void PrintToStream(ostream& out) { out << typeName; }
    friend ostream& operator<<(ostream& out, Type *t) { t->PrintToStream(out); return out; }
    virtual bool IsEquivalentTo(Type *other) { return this == other; }
    const char *GetPrintNameForNode() { return "Type"; }
    void PrintChildren(int indentLevel);
	
	virtual bool IsCompatibleTo(Type* other);
	bool Check(SymTable*){return true;}
};

class NamedType : public Type 
{
  protected:
    Identifier *id;
    
  public:
    NamedType(Identifier *i);
    void PrintToStream(ostream& out) { out << id; }    
    const char *GetPrintNameForNode() { return "NamedType"; }
    void PrintChildren(int indentLevel);
//  for symbol table usage
	char* GetName(){return id->GetName();}
	Identifier* GetIdentifier(){return id;}
	bool IsEquivalentTo(Type* other)
	{
		if(Type::IsEquivalentTo(other))
			return true;
		if(typeid(*this)==typeid(*other))
			return 0==strcmp(id->GetName(),static_cast<NamedType*>(other)->GetName());
		else
			return false;
	}
	// check if this NamedType is sub class of "other"
	bool IsCompatibleTo(Type* other);
	bool Check(SymTable*);
};

class ArrayType : public Type 
{
  protected:
    Type *elemType;

  public:
    ArrayType(yyltype loc, Type *elemType);

	ArrayType(Type* elemType);
	
    void PrintToStream(ostream& out) { out << elemType << "[]"; }    
    const char *GetPrintNameForNode() { return "ArrayType"; }
    void PrintChildren(int indentLevel);

	Type* GetElemType(){return elemType;}
	bool IsEquivalentTo(Type* other)
	{
		if(Type::IsEquivalentTo(other))
			return true;
		if(typeid(*this)==typeid(*other))
			return elemType->IsEquivalentTo(static_cast<ArrayType*>(other)->GetElemType());
		else
			return false;
	}

	bool IsCompatibleTo(Type* other){return false;}
	bool Check(SymTable*);
	/* return the dimension of this array */
	int GetDim();
};

 
#endif
