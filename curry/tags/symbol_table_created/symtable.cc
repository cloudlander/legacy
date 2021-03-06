#include "ast_stmt.h"

#include "errors.h"

#include "symtable.h"



extern Program* program;

/*

Type* Symbol::GetType()

{

	Assert(decl!=NULL);

	if(typeid(decl) == typeid(VarDecl))

	{

		return decl->

}

*/

bool Symbol::IsGlobalVar()

{

	if(NOTDETERMINED!=type)

		return type==GlobalVar;

	if(typeid(*decl)!=typeid(VarDecl))

		return false;

	Node* p=decl->GetParent();

	while(p!=NULL)

	{

		if(typeid(*p)==typeid(FnDecl) || 

		   typeid(*p)==typeid(ClassDecl))

			return false;

		p=p->GetParent();

	}

	type=GlobalVar;

	return true;

}

	

bool Symbol::IsLocalVar()

{

	if(NOTDETERMINED!=type)

		return type==LocalVar;

	if(typeid(*decl)!=typeid(VarDecl))

		return false;

	Node* p=decl->GetParent();

	while(p!=NULL)

	{

		if(typeid(*p)==typeid(FnDecl))

		{

			type=LocalVar;

			return true;

		}

		p=p->GetParent();

	}

	return false;

}

	

	

bool Symbol::IsClassVar()

{

	if(NOTDETERMINED!=type)

		return type==ClassVar;

	if(typeid(*decl)!=typeid(VarDecl))

		return false;

	if(IsLocalVar()) return false;

	Node* p=decl->GetParent();

	while(p!=NULL)

	{

		if(typeid(*p)==typeid(ClassDecl))

		{

			type=ClassVar;

			return true;

		}

		p=p->GetParent();

	}

	return false;

}

	

	

bool Symbol::IsClass()

{

	if(NOTDETERMINED!=type)

		return type==Class;

	if(typeid(*decl)!=typeid(ClassDecl))

		return false;

	type=Class;

	return true;

}

	

	

bool Symbol::IsMethod()

{

	if(NOTDETERMINED!=type)

		return type==Method;

	if(typeid(*decl)!=typeid(FnDecl))

		return false;

	Node* p=decl->GetParent();

	while(p!=NULL)

	{

		if(typeid(*p)==typeid(ClassDecl))

		{

			type=Method;

			return true;

		}

		p=p->GetParent();

	}

	return false;

}

	

	

bool Symbol::IsFunction()

{

	if(NOTDETERMINED!=type)

		return type==Function;

	if(typeid(*decl)!=typeid(FnDecl))

		return false;

	Node* p=decl->GetParent();

	while(p!=NULL)

	{

		if(typeid(*p)==typeid(ClassDecl))

			return false;

		p=p->GetParent();

	}

	type=Function;

	return true;

}

	



void Symbol::dump()

{

	fprintf(stderr,"\tSymbol(name=%s)",name);

	if(loc && IsDebugOn("location"))

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

		case ClassVar:

			fprintf(stderr,"ClassVar");

			break;

		case Class:

			fprintf(stderr,"Class");

			break;

		case Method:

			fprintf(stderr,"Method");

			break;

		case Function:

			fprintf(stderr,"Function");

			break;

		default:

			Failure("NOTDETERMINED,INTERNAL ERROR");

		}

		fprintf(stderr,"),(location=");

		switch(loc->GetSegment())

		{

			case gpRelative:

				fprintf(stderr,"gpRelative,");

				break;

			case fpRelative:

				fprintf(stderr,"fpRelative,");

				break;

			case clsRelative:

				fprintf(stderr,"clsRelative,");

				break;

			case vtbRelative:

				fprintf(stderr,"vtbRelative,");

				break;

		}

		fprintf(stderr,"%d)",loc->GetOffset());

	}

	fprintf(stderr,"\n");

}



void SymTable::dump_begin()

{

	fprintf(stderr,"<<<<Start of dumping symbol table(parent:%s):%s(%p)\n\n", NULL!=parent ? parent->info : "no parent",NULL!=info ? info : "(no info)" , this);

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

	fprintf(stderr,">>>>End of dumping symbol table(parent:%s):%s(%p)\n\n", NULL!=parent ? parent->info : "no parent",NULL!=info ? info : "(no info)" , this);

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



void SymTable::DetermineGlobalLocation()

{

	bool changed;

	do{

	  	changed=false;

		Iterator<Symbol*> iter=tbl.GetIterator();

		Symbol* sym;

	  	while((sym=iter.GetNextValue())!=NULL)

		{

			if(sym->GetLocation())	// ignore symbols that have been processed

				continue;

			if(sym->IsClass())

			{

				const char* ext=static_cast<ClassDecl*>(sym->GetDecl())->GetExtendClass();

				if(NULL==ext)

				{

					/* it's a base class */

					sym->GetDecl()->DetermineLocation();

											// determine each variable/method's offset from its

									  		// declaration order and its base classes (see below)

					sym->SetLocation(gpRelative,0);

					changed=true;

				}

				else

				{

					/* it has a parent class */

					// check the existence of parent class

					Symbol* parent=GetGlobalSymTable()->Find(ext);

					if(NULL==parent)

					{

						ReportError::ParentClassNotDefined(sym->GetDecl(),ext);

						/* just try to handle it as a base class */

						sym->GetDecl()->DetermineLocation();

						sym->SetLocation(gpRelative,0);

						changed=true;

					}

					else if(parent->GetLocation())	// all the parent class has location determined

					{

						sym->GetDecl()->GetSymTable()->SetParent(parent->GetDecl()->GetSymTable());

						sym->GetDecl()->DetermineLocation();

						sym->SetLocation(gpRelative,0);

						changed=true;

					}

				}

			}

			else

			{

				if(sym->IsGlobalVar() || sym->IsFunction()) // type of symbol is var or function)	// just set this symbol global

				{

					sym->SetLocation(gpRelative,0);

					if(sym->IsFunction()) //symbol is function

					{

						sym->GetDecl()->DetermineLocation();		// determine the global function's local variables' locations

						static_cast<FnDecl*>(sym->GetDecl())->SetMangledName(NameMangling(NULL,sym->GetName()));

					}

					changed=true;

				}

			}

		}

	}

	while(changed);

}



void SymTable::DetermineClassLocation(ClassDecl* parent,ClassDecl* me)

{

	int varoffset;   	//=parent.get(the direct parent's last offset) (size of class)

	int methodoffset; 	//=parent.get(the vtable size of the direct parent) 

	if(NULL==parent)

	{

		varoffset=4; 	// for vtable point;

		methodoffset=0;

	}

	else

	{

		varoffset=parent->GetVarOffset();

		methodoffset=parent->GetMethodOffset();

	}





	List<Symbol*> orderedList;

	int size=tbl.NumEntries();

	int i=0;

	Symbol* sym;

	while(i<size)

	{

		Iterator<Symbol*> iter=tbl.GetIterator();

		while((sym=iter.GetNextValue())!=NULL)

		{

			if(sym->GetOrder()==i)

			{

				orderedList.Append(sym);

				i++;

				break;

			}

		}

	}



	List<const char*>* vt=new List<const char*>;

	if(parent)

		CopyVtable(vt,parent->GetVtable());



	Symbol* symParent;

	for(i=0;i<orderedList.NumElements();i++)	//	foreach symbol in this symbol table in declaration order 

	{

		sym=orderedList.Nth(i);

		if(parent!=NULL && NULL!=(symParent=parent->GetSymTable()->Find(sym->GetName(),true)))	//	symbol can be found in parent's symbol table)	// sub class overide parent's symbol(variable or method)

		{

			//	link the parent's location to this symbol table

			sym->LinkLocation(symParent->GetLocation());

			if(sym->IsMethod())	//	symbol is method)

			{

				sym->GetDecl()->DetermineLocation();			// determine the method's local variables' locations

				OverideMethod(vt,sym->GetLocation()->GetOffset(),me,sym->GetName());

			}

		}

		else

		{

			if(sym->IsClassVar())	//	symbol is var

			{

				sym->SetLocation(clsRelative,varoffset);

				// increase total size because of new variable found

				varoffset+=sym->GetSize();

			}

			else 

			{

				if(sym->IsMethod())	//	symbol is method)

				{

					sym->SetLocation(vtbRelative,methodoffset);	// set the vtable offset of this newly found virtual function

					methodoffset+=sym->GetSize();

					sym->GetDecl()->DetermineLocation();			// determine the method's local variables' locations

					vt->Append(NameMangling(me,sym->GetName()));

				}

			}

		}

	}

	me->SetVarOffset(varoffset);	//	set the new sizeofclass

	me->SetMethodOffset(methodoffset);

	me->SetVtable(vt);

	me->SetVtableName(NameMangling(me,NULL));

}





int SymTable::DetermineParamLocation(int start,int increase)

{

	/* sort the symbol table to get a paramenter pushing order of C calling convention(backwards) */

	List<Symbol*> orderedList;

	int size=tbl.NumEntries();

	int i=0;

	Symbol* sym;

	while(i<size)

	{

		Iterator<Symbol*> iter=tbl.GetIterator();

		while((sym=iter.GetNextValue())!=NULL)

		{

			if(sym->GetOrder()==i)

			{

				orderedList.Append(sym);

				i++;

				break;

			}

		}

	}



	int offset=start;

	for(i=orderedList.NumElements()-1;i>=0;i--)	//	foreach symbol in symbol table in backward order

	{

		sym=orderedList.Nth(i);

		Assert(sym->IsLocalVar());

		sym->SetLocation(fpRelative,offset);

		offset+=(increase*sym->GetSize());

	}

	return offset;

}



int SymTable::DetermineLocalLocation(int start,int increase)

{

	int offset=start;

	Iterator<Symbol*> iter=tbl.GetIterator();

	Symbol* sym;

	while(NULL!=(sym=iter.GetNextValue()))	//	foreach symbol in symbol table

	{

		Assert(sym->IsLocalVar());

		sym->SetLocation(fpRelative,offset);

		offset+=(increase*sym->GetSize());

	}

	return offset;

}



SymTable* GetGlobalSymTable()

{

	Assert(program!=NULL && NULL!=program->GetSymTable());

	return program->GetSymTable();

}



void CopyVtable(List<const char*>* dest,List<const char*>* src)

{

	Assert(NULL!=dest && NULL!=src);

	for(int i=0;i<src->NumElements();i++)

		dest->Append(src->Nth(i));

}



const char* NameMangling(ClassDecl* decl,const char* func)

{

	Assert(decl || func);

	if(NULL==decl)

	{

		char* buf=new char[strlen(func)+2];

		strcpy(buf,"_");

		strcat(buf,func);

		return buf;

	}

	if(NULL==func)

	{

		char* buf=new char[strlen(decl->GetClassName())+8];

		strcpy(buf,decl->GetClassName());

		strcat(buf,"_vtable");

		return buf;

	}

	// mangling to Class.Func

	int declSize=strlen(decl->GetClassName())+1;

	int funcSize=strlen(func)+1;

	char* buf=new char[declSize+funcSize+1];

	strcpy(buf,decl->GetClassName());

	strcat(buf,".");

	strcat(buf,func);

	return buf;

}



void OverideMethod(List<const char*>* vtable,int offset,ClassDecl* decl,const char* func)

{

	const char* overide=NameMangling(decl,func);

	vtable->RemoveAt(offset/4);	

	vtable->InsertAt(overide,offset/4);

}



const char* GetMangledMethod(List<const char*>* vtable,int offset)

{

	return vtable->Nth(offset/4);

}



/* 

 * recursively build all scopes's symbol tables.

 * perform redefination checking only

 * 

 */

void BuildSymTable()

{

	program->BuildSymTable(0);		

	program->DetermineLocation();

}



