/* File: codegen.cc
 * ----------------
 * Implementation for the CodeGenerator class. The methods don't do anything
 * too fancy, mostly just create objects of the various Tac instruction
 * classes and append them to the list.
 */

#include "codegen.h"
#include "symtable.h"
#include <string.h>
#include "tac.h"
#include "x86.h"

extern int localoffset;  

CodeGenerator::CodeGenerator()
{
  code = new List<Instruction*>();
}

char *CodeGenerator::NewLabel()
{
  static int nextLabelNum = 0;
  char temp[10];
  sprintf(temp, "_L%d", nextLabelNum++);
  return strdup(temp);
}


Location *CodeGenerator::GenTempVar()
{
  static int nextTempNum;
  char temp[10];
  Location *result = NULL;
  sprintf(temp, "_tmp%d", nextTempNum++);
  /* pp4: need to create variable in proper location
     in stack frame for use as temporary. Until you
     do that, the assert below will always fail to remind
     you this needs to be implemented  */
  Assert(result != NULL);
  return result;
}

// just for debug
Location *CodeGenerator::GenTempVar(SymTable* symtbl)
{
  static int nextTempNum;
  char* temp=new char[10];
  Location *result = NULL;
  sprintf(temp, "_tmp%d", nextTempNum++);
//  result = new Location(fpRelative, 0, temp);
  symtbl->Append(temp,NULL);
  Symbol* sym=symtbl->Find(temp);
  Assert(sym);
  sym->SetLocation(fpRelative,localoffset);
  localoffset-=VarSize;
  return sym->GetLocation();
}

Location *CodeGenerator::GenLoadConstant(int value)
{
  Location *result = GenTempVar();
  code->Append(new LoadConstant(result, value));
  return result;
}

// just for debug
Location *CodeGenerator::GenLoadConstant(int value,  SymTable* symtbl)
{
  Location *result = GenTempVar(symtbl);
  code->Append(new LoadConstant(result, value));
  return result;
}

Location *CodeGenerator::GenLoadConstant(const char *s)
{
  Location *result = GenTempVar();
  code->Append(new LoadStringConstant(result, s));
  return result;
} 

// just for debug
Location *CodeGenerator::GenLoadConstant(const char *s, SymTable* symtbl)
{
  Location *result = GenTempVar(symtbl);
  code->Append(new LoadStringConstant(result, s));
  return result;
} 

Location *CodeGenerator::GenLoadLabel(const char *label, SymTable* symtbl)
{
  Location *result = GenTempVar(symtbl);
  code->Append(new LoadLabel(result, label));
  return result;
} 

// modify to return dst
/*
void CodeGenerator::GenAssign(Location *dst, Location *src)
{
  code->Append(new Assign(dst, src));
}
*/
Location* CodeGenerator::GenAssign(Location *dst, Location *src)
{
  code->Append(new Assign(dst, src));
  return dst;
}

Location *CodeGenerator::GenLoad(Location *ref, int offset,SymTable* symtbl)
{
  Location *result = GenTempVar(symtbl);
  code->Append(new Load(result, ref, offset));
  return result;
}

void CodeGenerator::GenStore(Location *dst,Location *src, int offset)
{
  code->Append(new Store(dst, src, offset));
}


Location *CodeGenerator::GenBinaryOp(const char *opName, Location *op1,
									   Location *op2, SymTable* symtbl)
{
  Location *result;
  
  if (symtbl == NULL)
    result = GenTempVar();
  else
    result = GenTempVar(symtbl);
  
  code->Append(new BinaryOp(BinaryOp::OpCodeForName(opName), result, op1, op2));
  return result;
}


void CodeGenerator::GenLabel(const char *label)
{
  code->Append(new Label(label));
}

void CodeGenerator::GenIfZ(Location *test, const char *label)
{
  code->Append(new IfZ(test, label));
}

void CodeGenerator::GenGoto(const char *label)
{
  code->Append(new Goto(label));
}

void CodeGenerator::GenReturn(Location *val)
{
  code->Append(new Return(val));
}

BeginMainFunc *CodeGenerator::GenBeginMainFunc()
{
  BeginMainFunc *result = new BeginMainFunc;
  code->Append(result);
  return result;
}

BeginFunc *CodeGenerator::GenBeginFunc()
{
  BeginFunc *result = new BeginFunc;
  code->Append(result);
  return result;
}

void CodeGenerator::GenEndFunc()
{
  code->Append(new EndFunc());
}

void CodeGenerator::GenPushParam(Location *param)
{
  code->Append(new PushParam(param));
}

void CodeGenerator::GenPopParams(int numBytesOfParams)
{
  Assert(numBytesOfParams >= 0 && numBytesOfParams % VarSize == 0); // sanity check
  if (numBytesOfParams > 0)
    code->Append(new PopParams(numBytesOfParams));
}

Location *CodeGenerator::GenLCall(const char *label, bool fnHasReturnValue, SymTable* symtbl)
{
  Location *result = NULL;

  if (symtbl == NULL) {
	  if (fnHasReturnValue) result = GenTempVar();
  }
  else {
	  if (fnHasReturnValue) result = GenTempVar(symtbl);
  }
  
  code->Append(new LCall(label, result));
  return result;
}

Location *CodeGenerator::GenACall(Location *fnAddr, bool fnHasReturnValue,SymTable* symtbl)
{
  Location *result = fnHasReturnValue ? GenTempVar(symtbl) : NULL;
  code->Append(new ACall(fnAddr, result));
  return result;
}
 
 
static struct _builtin {
  const char *label;
  int numArgs;
  bool hasReturn;
} builtins[] =
 {{"_ReadInt", 0, true},
   {"_ReadDouble", 0, true},
   {"_ReadLine", 0, true},
   {"_ReadBool", 0, true},
   {"_PrintInt", 1, false},
   {"_PrintDouble", 1, false},
   {"_PrintString", 1, false},
   {"_PrintBool", 1, false},
   {"_Alloc", 1, true},
   {"_StringEqual", 2, true},
   {"_Halt", 0, false}};

Location *CodeGenerator::GenBuiltInCall(BuiltIn bn,Location *arg1, Location *arg2, SymTable* symtbl)
{
  Assert(bn >= 0 && bn < NumBuiltIns);
  struct _builtin *b = &builtins[bn];
  Location *result = NULL;

  if (symtbl == NULL) {
	  if (b->hasReturn) result = GenTempVar();
  }
  else {
	  if (b->hasReturn) result = GenTempVar(symtbl);
  }

                // verify appropriate number of non-NULL arguments given
  Assert((b->numArgs == 0 && !arg1 && !arg2)
	|| (b->numArgs == 1 && arg1 && !arg2)
	|| (b->numArgs == 2 && arg1 && arg2));
  if (arg2) code->Append(new PushParam(arg2));
  if (arg1) code->Append(new PushParam(arg1));
  code->Append(new LCall(b->label, result));
  GenPopParams(VarSize*b->numArgs);
  return result;
}

static struct _thunk {
  const char *label;
  int numArgs;
  bool hasReturn;
} thunks[] =
 {{"_NewClass", 2, true},
   {"_NewArray", 2, true},
   {"_ArrayLength", 1, true},
   {"_EqualString", 2, true},
   {"_IsKindOf", 2, true},
   {"_CheckIndex", 2, false},
   };


Location * CodeGenerator::GenThunkCall(Thunk tn, Location *arg1, Location* arg2, SymTable* symtbl)
{
	Assert(tn >=0 && tn < NumThunks);
	struct _thunk* t = &thunks[tn];
	Location *result=NULL;

  	if (symtbl == NULL) {
	  if (t->hasReturn) result = GenTempVar();
	}
	else {
	  if (t->hasReturn) result = GenTempVar(symtbl);
	}

                // verify appropriate number of non-NULL arguments given
  Assert((t->numArgs == 0 && !arg1 && !arg2)
	|| (t->numArgs == 1 && arg1 && !arg2)
	|| (t->numArgs == 2 && arg1 && arg2));
  
  if (arg2) code->Append(new PushParam(arg2));
  if (arg1) code->Append(new PushParam(arg1));
  code->Append(new LCall(t->label, result));
  GenPopParams(VarSize*t->numArgs);
  return result;
}


void CodeGenerator::GenVTable(const char *className, List<const char *> *methodLabels)
{
  code->Append(new VTable(className, methodLabels));
}

void CodeGenerator::GenVTableWithType(const char* className, List<const char*> *methodLabels,const char* typeName)
{
  code->Append(new VTableWithType(className, methodLabels,typeName));
}

void CodeGenerator::GenTypeObject(const char* label,const char* TypeName,const char* parentName)
{
  code->Append(new TypeObject(label,TypeName,parentName));
}

void CodeGenerator::GenBeginTry(const char* l)
{
  code->Append(new BeginTry(l));
}

void CodeGenerator::GenEndTry()
{
  code->Append(new EndTry);
}

void CodeGenerator::GenThrow()
{
  code->Append(new Throw);
}

void CodeGenerator::GenGlobalVar(const char* var)
{
  code->Append(new DeclareGlobal(var));
}


void CodeGenerator::DoFinalCodeGen()
{
  if (IsDebugOn("tac")) { // if debug don't translate to x86, just print Tac
    for (int i = 0; i < code->NumElements(); i++)
	code->Nth(i)->Print();
   }  else {
     X86 x86;
     x86.EmitPreamble();
     for (int i = 0; i < code->NumElements(); i++)
	 code->Nth(i)->Emit(&x86);
  }
}


