/* File: ast.h
 * ----------- 
 * This file defines the abstract base class Node and the concrete 
 * Identifier and Error node subclasses that are used through the tree as 
 * leaf nodes. A parse tree is a hierarchical collection of ast nodes (or, 
 * more correctly, of instances of concrete subclassses such as VarDecl,
 * ForStmt, and AssignExpr).
 * 
 * Location: Each node maintains its lexical location (line and columns in 
 * file), that location can be NULL for those nodes that don't care/use 
 * locations. The location is typcially set by the node constructor.  The 
 * location is used to provide the context when reporting semantic errors.
 *
 * Parent: Each node has a pointer to its parent. For a Program node, the 
 * parent is NULL, for all other nodes it is the pointer to the node one level
 * up in the parse tree.  The parent is not set in the constructor (during a 
 * bottom-up parse we don't know the parent at the time of construction) but 
 * instead we wait until assigning the children into the parent node and then 
 * set up links in both directions. The parent link is typically not used 
 * during parsing, but is more important in later phases.
 *
 * Printing: Tthe only interesting behavior of the node classes for pp2 is the 
 * ability to print the tree using an in-order walk.  Each node class is 
 * responsible for printing itself/children by overriding the virtual 
 * GetPrintNameForNode() and PrintChildren() methods. All the classes we 
 * provide already implement these methods, so your job is to construct the
 * nodes and wire them up during parsing. Once that's done, printing is a snap!
 
 * Code generation: For pp4 you are adding "Emit" behavior to the ast
 * node classes. Your code generator should do an postorder walk on the
 * parse tree, and when visiting each node, emitting the necessary
 * instructions for that construct.
 */

#ifndef _H_ast
#define _H_ast

#include <stdlib.h>   // for NULL
#include <iostream>
#include "location.h"
#include "utility.h"
#include "codegen.h"

using namespace std;

class SymTable;

class Type;

class Node 
{
  protected:
    yyltype *location;
    Node *parent;

	SymTable* symtable;

	Type* type;	
  public:
    Node(yyltype loc);
    Node();
    
    yyltype *GetLocation()   { return location; }
    void SetParent(Node *p)  { parent = p; }
    Node *GetParent()        { return parent; }

    virtual const char *GetPrintNameForNode() = 0;
    
    // Print() is deliberately _not_ virtual
    // subclasses should override PrintChildren() instead
    void Print(int indentLevel, const char *label = NULL); 
    virtual void PrintChildren(int indentLevel)  {}

	// for symbol table operation
	SymTable* GetSymTable(){return symtable;}
	virtual void BuildSymTable(SymTable* parent)
	{
		if(IsDebugOn("scope"))
			fprintf(stderr," ----- BuildSymTable in %s  ------\n",GetPrintNameForNode());
	}
	virtual void DetermineLocation()
	{
		if(IsDebugOn("location"))
			fprintf(stderr," ----- DetermineLocation in %s  ------\n",GetPrintNameForNode());
	}
	
	virtual Location* GenTac(CodeGenerator* cg,SymTable* symtable)
	{
	    if (GetLocation()) 
        	printf("%*d", 2, GetLocation()->first_line);
    	else 
	        printf("%*s", 2,"");
		Failure(" --- GenTac in %s ---\n",GetPrintNameForNode());
		return NULL;
	}
	
	virtual bool Check(SymTable* symtbl)
	{
	    if (GetLocation()) 
        	printf("%*d", 2, GetLocation()->first_line);
    	else 
	        printf("%*s", 2,"");
		Failure(" --- Check in %s ---\n",GetPrintNameForNode());
		return false;
	}
};
   

class Identifier : public Node 
{
  protected:
    char *name;
    
  public:
    Identifier(yyltype loc, const char *name);
    friend ostream& operator<<(ostream& out, Identifier *id) { return out << id->name; }    
    const char *GetPrintNameForNode()   { return "Identifier"; }
    void PrintChildren(int indentLevel);

	/* access protected name for PP4 */
	char* GetName(){return name;}
};


// This node class is designed to represent a portion of the tree that 
// encountered syntax errors during parsing. The partial completed tree
// is discarded along with the states being popped, and an instance of
// the Error class can stand in as the placeholder in the parse tree
// when your parser can continue after an error.
class Error : public Node
{
  public:
    Error() : Node() {}
    const char *GetPrintNameForNode()   { return "Error"; }
};

#endif
