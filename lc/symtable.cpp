#include "symtable.h"
#include "errors.h"

SymTable* curSymTbl;
extern yyltype yylloc;


bool Symbol::IsGlobalVar()
{
	return type==GlobalVar;
}
	
bool Symbol::IsLocalVar()
{
	return type==LocalVar;
}
	
bool Symbol::IsFunction()
{
	return type==Function;
}
	
void Symbol::SetValue(int v,int* idx)
{
	if(value.Const)
	{
		ReportError::RuntimeError("%s: attemp to assign const",name);
	}
	if(idx)
	{
		if(NULL==value.dims)
		{
			fprintf(stderr,"Fatal error");
			exit(-1);
		}
		int index=idx[0];
		for(int i=1;i<10 && i<value.dims->NumElements();i++)
		{
			if(idx[i]<0 || idx[i]>=value.dims->Nth(i))
				ReportError::RuntimeError("array index out of bound");
			index=index*value.dims->Nth(i)+idx[i];
		}
		value.v.arrInt[index]=v;
		value.assigned=true;
		return;
	}
	value.assigned=true;
	value.v.Integer=v;
}

void Symbol::SetValue(double v,int* idx)
{
	if(value.Const)
	{
		ReportError::RuntimeError("%s: attemp to assign const",name);
	}
	if(idx)
	{
		if(NULL==value.dims)
		{
			fprintf(stderr,"Fatal error");
			exit(-1);
		}
		int index=idx[0];
		for(int i=1;i<10 && i<value.dims->NumElements();i++)
		{
			if(idx[i]<0 || idx[i]>=value.dims->Nth(i))
				ReportError::RuntimeError("array index out of bound");
			index=index*value.dims->Nth(i)+idx[i];
		}
		value.v.arrDbl[index]=v;
		value.assigned=true;
		return;
	}
	value.assigned=true;
	value.v.Double=v;
}

void Symbol::SetValue(bool v,int* idx)
{
	if(value.Const)
	{
		ReportError::RuntimeError("%s: attemp to assign const",name);
	}
	if(idx)
	{
		if(NULL==value.dims)
		{
			fprintf(stderr,"Fatal error");
			exit(-1);
		}
		int index=idx[0];
		for(int i=1;i<10 && i<value.dims->NumElements();i++)
		{
			if(idx[i]<0 || idx[i]>=value.dims->Nth(i))
				ReportError::RuntimeError("array index out of bound");
			index=index*value.dims->Nth(i)+idx[i];
		}
		value.v.arrBool[index]=v;
		value.assigned=true;
		return;
	}
	value.assigned=true;
	value.v.Boolean=v;
}

ValueType Symbol::GetValue(Value* v,int* idx)
{
	if(idx)
	{
		if(NULL==value.dims)
		{
			fprintf(stderr,"Fatal error");
			exit(-1);
		}
		int index=idx[0];
		for(int i=1;i<10 && i<value.dims->NumElements();i++)
		{
			if(idx[i]<0 || idx[i]>=value.dims->Nth(i))
				ReportError::RuntimeError("%s: array index out of bound",name);
			index=index*value.dims->Nth(i)+idx[i];
		}
		switch(value.vtype)
		{
			case Integer:
				v->Integer=value.v.arrInt[index];
				return Integer;
			case Double:
				v->Double=value.v.arrDbl[index];
				return Double;
			case Boolean:
				v->Boolean=value.v.arrBool[index];
				return Boolean;
			default:
				fprintf(stderr,"FFDT");
				exit(-1);
		}
	}
	if(!value.assigned)
	{
		ReportError::RuntimeError("%s: uninitialized variable",name);
	}
	switch(value.vtype)
	{
		case Integer:
			v->Integer=value.v.Integer;
			return Integer;
		case Double:
			v->Double=value.v.Integer;
			return Double;
		case Boolean:
			v->Boolean=value.v.Integer;
			return Boolean;
		default:
			fprintf(stderr,"FFDT");
			exit(-1);
	}
	return value.vtype;
}

void Symbol::dump()
{
	fprintf(stderr,"\tSymbol(name=%s)",name);
	if(1)
	{
		fprintf(stderr,",(type=");
		switch(type)
		{
		case LocalVar:
			fprintf(stderr,"LocalVar");
			break;
		case GlobalVar:
			fprintf(stderr,"GlobalVar");
			break;
		case Function:
			fprintf(stderr,"Function");
			break;
		default:
			Failure("NOTDETERMINED,INTERNAL ERROR");
		}
		fprintf(stderr,") Value=(%d,%d,",value.assigned,value.Const);
		switch(value.vtype)
		{
		case Integer:
			fprintf(stderr,"Int:%d)",value.v.Integer);
			break;
		case Double:
			fprintf(stderr,"Real:%e)",value.v.Double);
			break;
		case Boolean:
			fprintf(stderr,"Bool:%d)",value.v.Boolean);
			break;
		default:
			Failure("INTERNAL ERROR");
		}
	}
	fprintf(stderr,"\n");
}

Symbol* SymTable::Append(Symbol* s)
{
			if(Find(s->GetName()))
				ReportError::Formatted(&yylloc,"declaration conflict");
			tbl.Enter(s->GetName(),s);
			return s;
}

Symbol* SymTable::Append(const char* n,SymType t,List<int>* dims)
		{
			if(Find(n))
				ReportError::Formatted(&yylloc,"declaration conflict");
			Symbol* s=new Symbol(n,t,dims);
			tbl.Enter(n,s);
			return s;
		}

void SymTable::dump_begin()
{
	fprintf(stderr,"<<<<Start of dumping symbol table\n\n");
}

void SymTable::dump()
{
	Iterator<Symbol*> iter=tbl.GetIterator();
	Symbol* sym;
	while((sym=iter.GetNextValue())!=NULL)
	{
		sym->dump();
	}
}

void SymTable::dump_end()
{
	fprintf(stderr,">>>>End of dumping symbol table\n\n");
}

Symbol* SymTable::Find(const char* n,bool checkinparent)
{
	if(!checkinparent)
		return tbl.Lookup(n);
	else
	{
		Symbol* f=tbl.Lookup(n);
		if(NULL==f && parent)
			return parent->Find(n,parent->parent);
		else
			return f;
	}
}
