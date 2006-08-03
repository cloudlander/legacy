#ifndef _SYMTABLE_H
#define _SYMTABLE_H

#include "hashtable.h"
#include "ast_type.h"
#include "ast_decl.h"
#include "tac.h"
#include "utility.h"

// curry only contains these 6 types of symbol
typedef enum {NOTDETERMINED,GlobalVar,LocalVar,ClassVar,Class,Method,Function} SymType;

class Symbol
{
	public:
		/* symbol name */
		const char* name;		
		/* symbol's declaration node */
		Decl* decl;
		/* symbol's location infomation (used in PP4) */
		Location* loc;		

		int order;

		SymType type;

	public:
		Symbol(const char* n,int o,Decl* d)
		{
			Assert(n!=NULL);
			name=n;
			decl=d;		
			loc=NULL;
			type=NOTDETERMINED;
			order=o;
			// for "this" support
			if(0==strcmp("this",n))
				type=LocalVar;
		}
		const char* GetName(){return  name;}
		Decl* GetDecl()
		{
			Assert(decl!=NULL);
			return decl;
		}
		void SetLocation(Segment seg,int offset)
		{
			Assert(loc==NULL);
			loc=new Location(seg,offset,name);
		}
		void LinkLocation(Location* l)
		{
			Assert(loc==NULL);
			loc=l;
		}
		Location* GetLocation()
		{
//			Assert(loc!=NULL);
			return loc;
		}
		int GetOrder()
		{
			return order;
		}
		int GetSize()
		{
			return 4;
		}
		
		// checking symbol's type,implemented by  delayed determination
		bool IsGlobalVar();
		bool IsLocalVar();
		bool IsClassVar();
		bool IsClass();
		bool IsMethod();
		bool IsFunction();

		void dump();
//		Type* GetType();
};

class SymTable
{
	public:
		/* the actual symbol table implemention */
		Hashtable<Symbol*>	tbl;
		/* point to the parent symbol table */
		SymTable* parent;

		const char* info;

		int order;	// record the lexical declaration order

	public:
		// p refer to parent symtable, i just for debug
		SymTable(SymTable* p,char* i=NULL)
		{
			parent=p;
			info=i;
			order=0;
		}
		void Append(const char* n,Decl* d)
		{
			tbl.Enter(n,new Symbol(n,order++,d));
		}
		void Remove(const char* n)
		{
			tbl.Remove(n,tbl.Lookup(n));
		}
		void SetParent(SymTable* sym)
		{
			Assert(sym);
			parent=sym;
		}
		SymTable* GetParent()
		{
			return parent;
		}
	public:
		void dump_begin();
		void dump();
		void dump_end();

		/* find symbol from this table, also check parent table if checkinparent is true */
		Symbol* Find(const char* n,bool checkinparent=false);

		/* determine all the symbol's location in this symbol table */
		void DetermineGlobalLocation();				// root method
		void DetermineClassLocation(ClassDecl*,ClassDecl*);			// for class
		int DetermineLocalLocation(int,int);			// for function/method
		int DetermineParamLocation(int,int);			// for function/method's parameters

};

SymTable* GetGlobalSymTable();
void CopyVtable(List<const char*>* dest,List<const char*>* src);
const char* NameMangling(ClassDecl*,const char*);
void OverideMethod(List<const char*>* vtable,int offset,ClassDecl* decl,const char* func);
const char* GetMangledMethod(List<const char*>* vtable,int offset);

#define DEBUGSCOPEBEGIN(X)		{if(IsDebugOn("scope")) (X)->dump_begin();}
#define DEBUGSCOPEEND(X)		{if(IsDebugOn("scope")) (X)->dump_end();}
#define DEBUGSCOPE(X)			{if(IsDebugOn("scope")) (X)->dump();}

#define DEBUGLOCATIONBEGIN(X)	{if(IsDebugOn("location")) (X)->dump_begin();}
#define DEBUGLOCATIONEND(X)		{if(IsDebugOn("location")) (X)->dump_end();}
#define DEBUGLOCATION(X)		{if(IsDebugOn("location")) (X)->dump();}
#endif
