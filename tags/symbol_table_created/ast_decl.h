/* File: ast_decl.h
 * ----------------
 * In our parse tree, Decl nodes are used to represent and
 * manage declarations. There are 4 subclasses of the base class,
 * specialized for declarations of variables, functions, classes,
 * and interfaces.
 *
 * pp4: You will need to extend the Decl classes to implement
 * code generation for declarations.
 */

#ifndef _H_ast_decl
#define _H_ast_decl

#include "ast.h"
#include "list.h"
#include "ast_type.h"

class Type;
class NamedType;
class Identifier;
class Stmt;

class Decl : public Node 
{
  protected:
    Identifier *id;
  
  public:
    Decl(Identifier *name);
    friend ostream& operator<<(ostream& out, Decl *d) { return out << d->id; }
};

class VarDecl : public Decl 
{
  protected:
    Type *type;
    
  public:
    VarDecl(Identifier *name, Type *type);
    const char *GetPrintNameForNode() { return "VarDecl"; }
    void PrintChildren(int indentLevel);

	void BuildSymTable(SymTable*);
};

class ClassDecl : public Decl 
{
  protected:
    List<Decl*> *members;
    NamedType *extends;
    List<NamedType*> *implements;
 

	// for location determination
	int varoffset;
	int methodoffset;

	List<const char*>* vtable;
	const char* vtableName;
  public:
//    ClassDecl(Identifier *name, NamedType *extends, List<NamedType*> *implements, List<Decl*> *members);
    ClassDecl(Identifier *name, NamedType *extends, List<Decl*> *members);
    const char *GetPrintNameForNode() { return "ClassDecl"; }
    void PrintChildren(int indentLevel);
//  for symbol table usage
	int GetVarOffset(){return varoffset;}
	int GetMethodOffset(){return methodoffset;}
	void SetVarOffset(int v){varoffset=v;}
	void SetMethodOffset(int m){methodoffset=m;}
	const char* GetExtendClass();
	const char* GetClassName();
	void BuildSymTable(SymTable*);
	void DetermineLocation();
	List<const char*>* GetVtable(){Assert(vtable); return vtable;}
	void SetVtable(List<const char*>* v){Assert(v);vtable=v;}
	const char* GetVtableName(){return vtableName;}
	void SetVtableName(const char* n){vtableName=n;}
};

/*
class InterfaceDecl : public Decl 
{
  protected:
    List<Decl*> *members;
    
  public:
    InterfaceDecl(Identifier *name, List<Decl*> *members);
    const char *GetPrintNameForNode() { return "InterfaceDecl"; }
    void PrintChildren(int indentLevel);
};
*/

class FnDecl : public Decl 
{
  protected:
    List<VarDecl*> *formals;
    Type *returnType;
    Stmt *body;

    // just for formal's scope
	SymTable* formalTable;
	
	// frame size, byte count(not local var number count)
	int	frameSize;
	// the next offset to allocate local variable ( for generating temp variable )
	int localOffset;
	const char* mangledName;
  public:
    FnDecl(Identifier *name, Type *returnType, List<VarDecl*> *formals);
    void SetFunctionBody(Stmt *b);
    const char *GetPrintNameForNode() { return "FnDecl"; }
    void PrintChildren(int indentLevel);
//  for symbol table usage
	int GetLocalOffset(){return localOffset;}
	int GetFrameSize(){return frameSize;}
	void BuildSymTable(SymTable*);
	void DetermineLocation();
	void SetMangledName(const char* n){Assert(n);mangledName=n;}
	const char* GetMangledName(){Assert(mangledName);return mangledName;}
};

#endif
