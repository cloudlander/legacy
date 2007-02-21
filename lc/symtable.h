#ifndef _SYMTABLE_H
#define _SYMTABLE_H

#include "util.h"

typedef enum {NOTDETERMINED=10,GlobalVar,LocalVar,Function} SymType;

typedef enum {ERROR,Integer,Double,Boolean,String} ValueType;

typedef union{
	int Integer;
	double Double;
	bool Boolean;
	char* String;
}Value;

class Symbol
{
	public:
		/* symbol name */
		const char* name;		
		SymType type;

		struct{
			List<int>* dims;	// NULL if not an array
			bool assigned;
			bool Const;
			ValueType vtype;
			union
			{
				int Integer;
				double Double;
				bool Boolean;
				char* String;
				int* arrInt;
				double* arrDbl;
				bool* arrBool;
			}v;
		}value;
			

	public:
		Symbol(const char* n,SymType st,List<int>* dims=NULL)
		{
			Assert(n!=NULL);
			name=n;
			type=st;
			value.assigned=false;
			value.vtype=ERROR;
			value.Const=false;
			value.dims=dims;
		}

		void SetConst(int v)
		{
			value.assigned=true;
			value.Const=true;
			value.vtype=Integer;
			value.v.Integer=v;
		}

		void SetConst(double v)
		{
			value.assigned=true;
			value.Const=true;
			value.vtype=Double;
			value.v.Double=v;
		}

		void SetConst(bool v)
		{
			value.assigned=true;
			value.Const=true;
			value.vtype=Boolean;
			value.v.Boolean=v;
		}
		
		void SetVType(ValueType vt)
		{
			value.vtype=vt;
			if(value.dims)
			{
				int temp=1;
				for(int i=0;i<value.dims->NumElements();i++)
				{
					temp*=value.dims->Nth(i);
				}
				switch(vt)
				{
				case Integer:
					value.v.arrInt=new int[temp];
					break;
				case Double:
					value.v.arrDbl=new double[temp];
					break;
				case Boolean:
					value.v.arrBool=new bool[temp];
					break;
				default:
					fprintf(stderr,"FFDT");
				}
			}
		}

		ValueType GetValueType()
		{
			return value.vtype;
		}
		
		void SetValue(int,int* idx=NULL);
		void SetValue(double,int* idx=NULL);
		void SetValue(bool,int* idx=NULL);

		ValueType GetValue(Value* v,int* idx=NULL);
		
		const char* GetName(){return  name;}
		
		// checking symbol's type,implemented by  delayed determination
		bool IsGlobalVar();
		bool IsLocalVar();
		bool IsFunction();

		List<int>* ArrayDims()
		{
			return value.dims;
		}

		void dump();
};

class SymTable
{
	public:
		/* the actual symbol table implemention */
		Hashtable<Symbol*>	tbl;
		/* point to the parent symbol table */
		SymTable* parent;

		const char* info;
	public:
		// p refer to parent symtable, i just for debug
		SymTable(SymTable* p,char* i=NULL)
		{
			parent=p;
			info=i;
		}

		Symbol* Append(const char* n,SymType t,List<int>* d=NULL);
		Symbol* Append(Symbol* s);
		
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

};

extern SymTable* curSymTbl;

#endif
