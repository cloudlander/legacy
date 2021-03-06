/* File: tac.cc
 * ------------
 * Implementation of Location class and Instruction class/subclasses.
 */
  
#include "tac.h"
#include "x86.h"

Location::Location(Segment s, int o, const char *name) :
  variableName(strdup(name)), segment(s), offset(o)
{
	isPointer=false;
}

 
void Instruction::Print() {
  printf("\t%s ;\n", printed);
}


void Instruction::Emit(X86 *x86) {
  if (*printed)
    x86->Emit("# %s", printed);   // emit TAC as comment into assembly
  EmitSpecific(x86);
} 

DeclareGlobal::DeclareGlobal(const char* s){
	Assert(s);
	sprintf(printed,"declare global variable:%s",s);
	str=new char[strlen(s)+1];
	strcpy(str,s);
}

void DeclareGlobal::EmitSpecific(X86 *x86){
	x86->EmitDeclareGlobal(str);
}
	

LoadConstant::LoadConstant(Location *d, int v)
  : dst(d), val(v) {
  Assert(dst != NULL);
  sprintf(printed, "%s = %d", dst->GetName(), val);
}
void LoadConstant::EmitSpecific(X86 *x86) {
  x86->EmitLoadConstant(dst, val);
}


LoadStringConstant::LoadStringConstant(Location *d, const char *s)
  : dst(d) {
  Assert(dst != NULL && s != NULL);
  const char *quote = (*s == '"') ? "" : "\"";
  str = new char[strlen(s) + 2*strlen(quote) + 1];
  sprintf(str, "%s%s%s", quote, s, quote);
  quote = (strlen(str) > 50) ? "...\"" : "";
  sprintf(printed, "%s = %.50s%s", dst->GetName(), str, quote);
}
void LoadStringConstant::EmitSpecific(X86 *x86) {
  x86->EmitLoadStringConstant(dst, str);
}
     

LoadLabel::LoadLabel(Location *d, const char *l)
  : dst(d), label(strdup(l)) {
  Assert(dst != NULL && label != NULL);
  sprintf(printed, "%s = %s", dst->GetName(), label);
}
void LoadLabel::EmitSpecific(X86 *x86) {
  x86->EmitLoadLabel(dst, label);
}


Assign::Assign(Location *d, Location *s)
  : dst(d), src(s) {
  Assert(dst != NULL && src != NULL);
  sprintf(printed, "%s = %s", dst->GetName(), src->GetName());
}
void Assign::EmitSpecific(X86 *x86) {
  x86->EmitCopy(dst, src);
}


Load::Load(Location *d, Location *s, int off)
  : dst(d), src(s), offset(off) {
  Assert(dst != NULL && src != NULL);
  if (offset) 
    sprintf(printed, "%s = *(%s + %d)", dst->GetName(), src->GetName(), offset);
  else
    sprintf(printed, "%s = *(%s)", dst->GetName(), src->GetName());
}
void Load::EmitSpecific(X86 *x86) {
  x86->EmitLoad(dst, src, offset);
}


Store::Store(Location *d, Location *s, int off)
  : dst(d), src(s), offset(off) {
  Assert(dst != NULL && src != NULL);
  if (offset)
    sprintf(printed, "*(%s + %d) = %s", dst->GetName(), offset, src->GetName());
  else
    sprintf(printed, "*(%s) = %s", dst->GetName(), src->GetName());
}
void Store::EmitSpecific(X86 *x86) {
  x86->EmitStore(dst, src, offset);
}


char * BinaryOp::opName[BinaryOp::NumOps] = {"+", "-", "*", "/", "%", "==", "<", "&&", "||", "<=", "!=", "!"};
 
BinaryOp::OpCode BinaryOp::OpCodeForName(const char *name) {
  for (int i = 0; i < NumOps; i++) 
    if (opName[i] && !strcmp(opName[i], name))
	return (OpCode)i;
  Failure("Unrecognized Tac operator: '%s'\n", name);
  return Add; // can't get here, but compiler doesn't know that
}
BinaryOp::BinaryOp(OpCode c, Location *d, Location *o1, Location *o2)
  : code(c), dst(d), op1(o1), op2(o2) {
  Assert(dst != NULL && op1 != NULL && op2 != NULL);
  Assert(code >= 0 && code < NumOps);
  sprintf(printed, "%s = %s %s %s", dst->GetName(), op1->GetName(), opName[code], op2->GetName());
}
void BinaryOp::EmitSpecific(X86 *x86) {	  
  x86->EmitBinaryOp(code, dst, op1, op2);
}


Label::Label(const char *l) : label(strdup(l)) {
  Assert(label != NULL);
  *printed = '\0';
}
void Label::Print() {
  printf("%s:\n", label);
}
void Label::EmitSpecific(X86 *x86) {
  x86->EmitLabel(label);
}

 
Goto::Goto(const char *l) : label(strdup(l)) {
  Assert(label != NULL);
  sprintf(printed, "Goto %s", label);
}
void Goto::EmitSpecific(X86 *x86) {	  
  x86->EmitGoto(label);
}


IfZ::IfZ(Location *te, const char *l)
   : test(te), label(strdup(l)) {
  Assert(test != NULL && label != NULL);
  sprintf(printed, "IfZ %s Goto %s", test->GetName(), label);
}
void IfZ::EmitSpecific(X86 *x86) {	  
  x86->EmitIfZ(test, label);
}


BeginMainFunc::BeginMainFunc() {
  sprintf(printed,"BeginMainFunc (unassigned)");
  frameSize = -555; // used as sentinel to recognized unassigned value
}
void BeginMainFunc::SetFrameSize(int numBytesForAllLocalsAndTemps) {
  frameSize = numBytesForAllLocalsAndTemps; 
  sprintf(printed,"BeginMainFunc %d", frameSize);
}
void BeginMainFunc::EmitSpecific(X86 *x86) {
  x86->EmitBeginMainFunction(frameSize);
}


BeginFunc::BeginFunc() {
  sprintf(printed,"BeginFunc (unassigned)");
  frameSize = -555; // used as sentinel to recognized unassigned value
}
void BeginFunc::SetFrameSize(int numBytesForAllLocalsAndTemps) {
  frameSize = numBytesForAllLocalsAndTemps; 
  sprintf(printed,"BeginFunc %d", frameSize);
}
void BeginFunc::EmitSpecific(X86 *x86) {
  x86->EmitBeginFunction(frameSize);
}


EndFunc::EndFunc() : Instruction() {
  sprintf(printed, "EndFunc");
}
void EndFunc::EmitSpecific(X86 *x86) {
  x86->EmitEndFunction();
}

 
Return::Return(Location *v) : val(v) {
  sprintf(printed, "Return %s", val? val->GetName() : "");
}
void Return::EmitSpecific(X86 *x86) {	  
  x86->EmitReturn(val);
}


PushParam::PushParam(Location *p)
  :  param(p) {
  Assert(param != NULL);
  sprintf(printed, "PushParam %s", param->GetName());
}
void PushParam::EmitSpecific(X86 *x86) {
  x86->EmitParam(param);
} 


PopParams::PopParams(int nb)
  :  numBytes(nb) {
  sprintf(printed, "PopParams %d", numBytes);
}
void PopParams::EmitSpecific(X86 *x86) {
  x86->EmitPopParams(numBytes);
} 


LCall::LCall(const char *l, Location *d)
  :  label(strdup(l)), dst(d) {
  sprintf(printed, "%s%sLCall %s", dst? dst->GetName(): "", dst?" = ":"", label);
}
void LCall::EmitSpecific(X86 *x86) {
  x86->EmitLCall(dst, label);
}


ACall::ACall(Location *ma, Location *d)
  : dst(d), methodAddr(ma) {
  Assert(methodAddr != NULL);
  sprintf(printed, "%s%sACall %s", dst? dst->GetName(): "", dst?" = ":"",
	    methodAddr->GetName());
}
void ACall::EmitSpecific(X86 *x86) {
  x86->EmitACall(dst, methodAddr);
} 


VTable::VTable(const char *l, List<const char *> *m)
  : methodLabels(m), label(strdup(l)) {
  Assert(methodLabels != NULL && label != NULL);
  sprintf(printed, "VTable for class %s", l);
}
void VTable::Print() {
  printf("VTable %s =\n", label);
  for (int i = 0; i < methodLabels->NumElements(); i++) 
    printf("\t%s,\n", methodLabels->Nth(i));
  printf("; \n"); 
}
void VTable::EmitSpecific(X86 *x86) {
  x86->EmitVTable(label, methodLabels);
}


VTableWithType::VTableWithType(const char *l, List<const char *> *m , const char* tn)
  : VTable(l,m) , typeName(strdup(tn)) {
  Assert(methodLabels != NULL && label != NULL && tn != NULL);
  sprintf(printed, "VTableWithType for class %s", l);
}
void VTableWithType::Print() {
  printf("VTableWithType %s =\n", label);
  printf("\t%s\n", typeName);
  for (int i = 0; i < methodLabels->NumElements(); i++) 
    printf("\t%s,\n", methodLabels->Nth(i));
  printf("; \n"); 
}
void VTableWithType::EmitSpecific(X86 *x86) {
  x86->EmitVTableWithType(label, methodLabels,typeName);
}

TypeObject::TypeObject(const char* l,const char* tn,const char* pn)
:label(strdup(l)),typeName(strdup(tn)),parentName(strdup(pn)) {
	Assert(label != NULL && typeName != NULL && parentName != NULL);
	sprintf(printed, "TypeObject of class %s",typeName);
}
void TypeObject::Print() {
	printf("TypeObject %s =\n", label);
	printf("\ttype=\"%s\"\n",typeName);
	printf("\tparent=%s\n",parentName);
	printf("; \n");
}
void TypeObject::EmitSpecific(X86 *x86) {
	x86->EmitTypeObject(label,typeName,parentName);
}
	
BeginTry::BeginTry(const char* l)
:labelCatchBlock(strdup(l)) {
	Assert(labelCatchBlock != NULL);
	sprintf(printed, "BeginTry(CatchBlockLabel: %s)\n",labelCatchBlock);
}
void BeginTry::Print() {
	printf("Begin Try(CatchBlockLabel: %s)\n",labelCatchBlock);
}
void BeginTry::EmitSpecific(X86 *x86) {
	x86->EmitBeginTry(labelCatchBlock);
}

void EndTry::Print() {
	printf("End Try\n");
}
void EndTry::EmitSpecific(X86 *x86) {
	x86->EmitEndTry();
}

void Throw::Print() {
	printf("Throw Exception\n");
}
void Throw::EmitSpecific(X86 *x86){
	x86->EmitThrow();
}
