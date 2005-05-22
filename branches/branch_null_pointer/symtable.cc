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



bool CheckOverideMethod(Decl* fn,Decl* fnOverided);

	

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

				const char* ext=static_cast<ClassDecl*>(sym->GetDecl())->GetExtendClassName();

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

						ReportError::IdentifierNotDeclared(static_cast<ClassDecl*>(sym->GetDecl())->GetExtendClass(),LookingForClass);

//						ReportError::ParentClassNotDefined(sym->GetDecl(),ext);

						/* just try to handle it as a base class */

						sym->GetDecl()->DetermineLocation();

						sym->SetLocation(gpRelative,0);

						changed=true;

					}

					else if(parent->GetLocation())	// all the parent class has location determined

					{

						if(parent->IsClass())		// must be a class type

						{

							sym->GetDecl()->GetSymTable()->SetParent(parent->GetDecl()->GetSymTable());

							sym->GetDecl()->DetermineLocation();

							sym->SetLocation(gpRelative,0);

						}

						else

						{

							ReportError::IdentifierNotDeclared(static_cast<ClassDecl*>(sym->GetDecl())->GetExtendClass(),LookingForClass);

							/* just try to handle it as a base class */

							sym->GetDecl()->DetermineLocation();

							sym->SetLocation(gpRelative,0);

						}

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

		if(parent!=NULL && NULL!=(symParent=parent->GetSymTable()->Find(sym->GetName(),true)))	//	symbol can be found in parent's symbol table)	// sub class overide parent's method)

		{

			if(sym->IsMethod())	//	symbol is method)

			{

				if(! CheckOverideMethod(sym->GetDecl(),symParent->GetDecl())) // check the signature

				{

					ReportError::OverrideMismatch(sym->GetDecl()); // just ignore this error to detect more erros

					sym->SetLocation(vtbRelative,methodoffset);	   // simulate it as a new found method

					sym->GetDecl()->DetermineLocation();

					methodoffset+=sym->GetSize();

					continue;

				}

			

				//	link the parent's location to this symbol table

				sym->LinkLocation(symParent->GetLocation());

				

				sym->GetDecl()->DetermineLocation();			// determine the method's local variables' locations

				OverideMethod(vt,sym->GetLocation()->GetOffset(),me,sym->GetName());

			}

			else if(sym->IsClassVar())		// disallow overiding variable of parent

				ReportError::DeclConflict(sym->GetDecl(),symParent->GetDecl());

				

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

	for(i=0;i<orderedList.NumElements();i++)	//	foreach symbol in symbol table in order

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



bool mainFunctionFound;



void SymTable::Check()

{

	mainFunctionFound=false;

	Assert(NULL==parent);		// must be the program scope symtable

	Iterator<Symbol*> iter=tbl.GetIterator();

	Symbol* sym;

	while(NULL!=(sym=iter.GetNextValue()))

	{

		Assert(sym);

		sym->GetDecl()->Check(this);

	}

	if(!mainFunctionFound)

		ReportError::NoMainFound();

}



const char* labelIntTypeObject="Int_Type";

const char* labelDoubleTypeObject="Double_Type";

const char* labelBoolTypeObject="Bool_Type";

const char* labelStringTypeObject="String_Type";



#ifdef CYGWIN

const char* ehValue="_exception_value";

const char* ehType="_exception_type";

const char* ehDim="_exception_type_dim";

#else

const char* ehValue="exception_value";

const char* ehType="exception_type";

const char* ehDim="exception_type_dim";

#endif



Location* ehValueLoc=new Location(gpRelative,0,ehValue);

Location* ehTypeLoc=new Location(gpRelative,0,ehType);

Location* ehDimLoc=new Location(gpRelative,0,ehDim);



BuiltInException bie[]={

	{"IndexOutOfBoundException","runtime error: index out of bounds\\n"},

	{"NewObjectFailureException", "runtime error: memory allocation\\n"},

	{"ArraySizeException", "runtime error: invalid array length\\n"},

	{"NullPointerException", "runtime error: null reference\\n"},

/* add more runtime exception here */

	{"UnKnownException", "runtime error: uncaught exception\\n"}

};



int bie_size=sizeof(bie)/sizeof(BuiltInException);



void SymTable::GenCode(CodeGenerator* cg)

{

	/* Generate all the internal basic type object */

	cg->GenTypeObject(labelIntTypeObject,"Integer","0");

	cg->GenTypeObject(labelDoubleTypeObject,"Double","0");

	cg->GenTypeObject(labelBoolTypeObject,"Boolean","0");

	cg->GenTypeObject(labelStringTypeObject,"String","0");



	/* Generate all the builtin exception type object */

	for(int i=0;i<bie_size-1;i++)

	{

		char typeLabel[100];

		strcpy(typeLabel,bie[i].typeName);	

		strcat(typeLabel,"_Type");

		cg->GenTypeObject(typeLabel,bie[i].typeName,"0");

	}



	/* Compiler generates these three global variable to support exception handling */

	cg->GenGlobalVar(ehValue);

	cg->GenGlobalVar(ehType);

	cg->GenGlobalVar(ehDim);



	Iterator<Symbol*> iter=tbl.GetIterator();

	Symbol* sym;

	while(NULL!=(sym=iter.GetNextValue()))

	{

		Assert(sym->GetLocation());

		if(sym->IsGlobalVar())

			cg->GenGlobalVar(sym->GetName());

		else if(sym->IsClass())

		{

			char typeName[100];

			char typeLabel[100];

			char parenttypeLabel[100];

			const char* parentType=static_cast<ClassDecl*>(sym->GetDecl())->GetExtendClassName();

			const char* nameType=static_cast<ClassDecl*>(sym->GetDecl())->GetClassName();



			strcpy(typeLabel,nameType);

			strcat(typeLabel,"_Type");

			strcpy(typeName,"Class_");

			strcat(typeName,nameType);

			if(parentType)

			{

				strcpy(parenttypeLabel,parentType);

				strcat(parenttypeLabel,"_Type");

			}

			else

			{

				strcpy(parenttypeLabel,"0");

			}



			cg->GenTypeObject(typeLabel,typeName,parenttypeLabel);

			

			cg->GenVTableWithType(static_cast<ClassDecl*>(sym->GetDecl())->GetVtableName(),static_cast<ClassDecl*>(sym->GetDecl())->GetVtable(),typeLabel);

			

//			cg->GenVTable(static_cast<ClassDecl*>(sym->GetDecl())->GetVtableName(),static_cast<ClassDecl*>(sym->GetDecl())->GetVtable());



			sym->GetDecl()->GetSymTable()->GenClassCode(static_cast<ClassDecl*>(sym->GetDecl()),cg);

		}

		else if(sym->IsFunction())

		{

			cg->GenLabel(static_cast<FnDecl*>(sym->GetDecl())->GetMangledName());

			static_cast<FnDecl*>(sym->GetDecl())->GenCode(cg);

		}

		else

			Assert(0);

	}

}



void SymTable::GenClassCode(ClassDecl* decl,CodeGenerator* cg)

{

	Iterator<Symbol*> iter=tbl.GetIterator();

	Symbol* sym;

	while(NULL!=(sym=iter.GetNextValue()))

	{

		if(sym->IsMethod())

		{

			cg->GenLabel(GetMangledMethod(decl->GetVtable(),sym->GetLocation()->GetOffset()));

			static_cast<FnDecl*>(sym->GetDecl())->GenCode(cg);

		}

	}

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

	if(NULL==decl)	/* function name mangling */

	{

		char* buf=new char[strlen(func)+2];

#ifdef CYGWIN

		strcpy(buf,"_");

#else

		buf[0]='\0';

#endif

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



/* check the signature of overided funtion

 * return true if two functions' signature are the same

 * (parameter declaration and return type)

 */

bool CheckOverideMethod(Decl* fn,Decl* fnOverided)

{

	FnDecl* childFn=static_cast<FnDecl*>(fn);

	FnDecl* parentFn=static_cast<FnDecl*>(fnOverided);



	if(!childFn->GetType()->IsEquivalentTo(parentFn->GetType()))

		return false;

	

	List<VarDecl*>* childFormals=childFn->GetFormals();

	List<VarDecl*>* parentFormals=parentFn->GetFormals();

	if(childFormals->NumElements() != parentFormals->NumElements())

		return false;

	for(int i=0;i<childFormals->NumElements();i++)

	{

		if(! childFormals->Nth(i)->GetType()->IsEquivalentTo(

				parentFormals->Nth(i)->GetType()))

			return false;

	}

	return true;

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

 * 

 */

void BuildSymTable()

{

	program->BuildSymTable(0);		

	program->DetermineLocation();

}



void TypeCheck()

{

	GetGlobalSymTable()->Check();

}



/* 

 * Generate TAC code

 * 

 */

void GenCode()

{

	CodeGenerator* cg=new CodeGenerator;

	GetGlobalSymTable()->GenCode(cg);

	cg->DoFinalCodeGen();

}

