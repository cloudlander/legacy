/* File: x86.cc
 * -------------
 * Implementation of x86 class, which is responsible for TAC->x86
 * translation, register allocation, etc.
 *
 * Edited by WangPeng(Fudan SoftWare School) academic year 2003-12
 * Loosely based on the file from stanford decaf project 
 *
 * A simple final code generator to translate Tac to x86.
 * It uses simplistic algorithms, in particular, its register handling
 * and spilling strategy is inefficient to the point of begin mocked
 * by elementary school children.
 */

#include "x86.h"
#include <stdarg.h>

/* Method: BindVarWithRegister
 * ------------------------
 *  Allocate the very register reg to var
 */
 void X86::BindVarWithRegister(Location *var,  Register reg, Reason reason)
{
	Register tmpreg;
	
	if (regs[reg].var != var) {
		SpillRegister(reg);
		if (FindRegisterWithContents(var, tmpreg)) {
			Emit("movl\t%s, %s", regs[tmpreg].name, regs[reg].name);
			regs[reg].var = var;
			regs[reg].isDirty = regs[tmpreg].isDirty;
			regs[tmpreg].var = NULL;
			regs[tmpreg].isDirty = false;
		}		
		else {
			regs[reg].var = var;
			if (reason == ForRead) {                 			// load current value
				if (var->GetSegment() == fpRelative) {
				       Assert(var->GetOffset() % 4 == 0); 	// all (local) variables are 4 bytes
					Emit("movl\t%d(%%ebp), %s\t# Load %s to %s from %s%+d", var->GetOffset(), regs[reg].name,
					        var->GetName(), regs[reg].name, "%ebp", var->GetOffset());
				}
				else {
					Emit("movl\t%s, %s\t# Load %s to %s", var->GetName(), regs[reg].name, 
					        var->GetName(), regs[reg].name);
				}	
				regs[reg].isDirty = false;
			}
		}
	}
	if (reason == ForWrite)
		regs[reg].isDirty = true;
}


/* Method: GetRegister
 * -------------------
 * Given a location for a current var, a reason (ForRead or ForWrite)
 * and up to two registers to avoid, this method will assign
 * to a var to register trying these alternatives in order:
 *  1) if that var is already in a register ("same" is determined
 *      by matching name and same scope), we use that one
 *  2) find an empty register to use for the var
 *  3) find an in-use register that is not dirty.  We don't need
 *      to write value back to memory since it's clean, so we just
 *      replace with the new var
 *  4) spill an in-use, dirty register, by writing its contents to
 *      memory and then replace with the new var
 * For steps 3 & 4, we respect the registers given to avoid (ie the
 * other operands in this operation). The register descriptors are
 * updated to show the new state of the world. If for read, we
 * load the current value from memory into the register. If for
 * write, we mark the register as dirty (since it is getting a
 * new value).
 */
X86::Register X86::GetRegister(Location *var, Reason reason, Register avoid1, Register avoid2)
{
	Register reg;
	
	if (FindRegisterWithContents(var, reg)) {
		if (reg == avoid1 || reg == avoid2)
			SpillRegister(reg);
		else {
			if (reason == ForWrite)
				regs[reg].isDirty = true;
			return reg;
		}			
	}

	if (!FindNullRegister(reg, avoid1, avoid2)) {
       	reg = SelectRegisterToSpill(avoid1, avoid2);
		SpillRegister(reg);
	}
  	regs[reg].var = var;

  	if (reason == ForRead) {                 			// load current value
	     	if (var->GetSegment() == fpRelative) {
	  		Assert(var->GetOffset() % 4 == 0); 	// all variables are 4 bytes
	         	Emit("movl\t%d(%%ebp), %s\t# Load %s to %s from %s%+d", var->GetOffset(), regs[reg].name,
		  	        var->GetName(), regs[reg].name, "%ebp", var->GetOffset());
     		}
     		else {
       		Emit("movl\t%s, %s\t# Load %s to %s", var->GetName(), regs[reg].name, 
				 var->GetName(), regs[reg].name);
     		}
		regs[reg].isDirty = false;
  	}

	if (reason == ForWrite)
		regs[reg].isDirty = true;
	return reg;
}


// Two covers for the above method to make it slightly more
// convenient to call it
X86::Register X86::GetRegister(Location *var, Register avoid1, Register avoid2)
{
  return GetRegister(var, ForRead, avoid1, avoid2);
}

X86::Register X86::GetRegisterForWrite(Location *var, Register avoid1, Register avoid2)
{
  return GetRegister(var, ForWrite, avoid1, avoid2);
} 


// Helper to check if two variable locations are one and the same
// (same name, segment, and offset)
static bool LocationsAreSame(Location *var1, Location *var2)
{
   return (var1 == var2 ||
	       (var1 && var2
	  	  && !strcmp(var1->GetName(), var2->GetName())
		  && var1->GetSegment()  == var2->GetSegment()
		  && var1->GetOffset() == var2->GetOffset()));
}


/* Method: FindRegisterWithContents
 * --------------------------------
 * Searches the descriptors for one with contents var. Assigns
 * register by reference, and returns true/false on whether match found.
 */
bool X86::FindRegisterWithContents(Location *var, Register& reg)
{
  for (reg = zero; reg < NumRegs; reg = (Register)(reg+1))
    if (regs[reg].isGeneralPurpose && LocationsAreSame(var, regs[reg].var))
	return true;
  return false;
}

/* Method: FindNULLRegister
 * --------------------------------
 * Find a register which is not used
 */
bool X86::FindNullRegister(Register& reg, Register avoid1, Register avoid2)
{
  for (reg = zero; reg < NumRegs; reg = (Register)(reg+1))
    if (reg != avoid1 && reg != avoid2 && regs[reg].isGeneralPurpose && LocationsAreSame(NULL, regs[reg].var))
	return true;
  return false;
}


/* Method: SelectRegisterToSpill
 * -----------------------------
 * Chooses an in-use register to replace with a new variable. We
 * prefer to replace a non-dirty once since we would not have to
 * write its contents back to memory, so the first loop searches
 * for a clean one. If none found, we take a dirty one.  In both
 * loops we deliberately won't choose either of the registers we
 * were asked to avoid.  We track the last dirty register spilled
 * and advance on each subsequent spill as a primitive means of
 * trying to not throw out things we just loaded and thus are likely
 * to need. 
 */
X86::Register X86::SelectRegisterToSpill(Register avoid1, Register avoid2)
{
            // first hunt for a non-dirty one, since no work to spill
  for (Register i = zero; i < NumRegs; i = (Register)(i+1)) {
    if (i != avoid1 && i != avoid2 && regs[i].isGeneralPurpose && !regs[i].isDirty)
	return i;
  }
  do {   // otherwise just pick the next usuable register
    lastUsed = (Register)((lastUsed + 1) % NumRegs);
  } while (lastUsed == avoid1 || lastUsed == avoid2 || !regs[lastUsed].isGeneralPurpose);
  return lastUsed;
}


/* Method: SpillRegister
 * ---------------------
 * "Empties" register.  If variable is currently slaved in this register
 * and its contents are out of synch with memory (isDirty), we write back
 * the current contents to memory. We then clear the descriptor so we
 * realize the register is empty.
 */
void X86::SpillRegister(Register reg)
{
  Location *var = regs[reg].var;
  if (var && regs[reg].isDirty) {
    if (var->GetSegment() == fpRelative) {
      Assert(var->GetOffset() % 4 == 0); // all variables are 4 bytes in size		
      Emit("movl\t%s, %d(%s)\t# spill %s from %s to %s%+d", regs[reg].name,
	  	var->GetOffset(), "%ebp", var->GetName(), regs[reg].name,
		"%ebp", var->GetOffset());
    }
    else {
      Emit("movl\t%s, %s\t# spill %s from %s", regs[reg].name,
  	        var->GetName(), var->GetName(), regs[reg].name);
    }	
  }
  regs[reg].isDirty = false;
  regs[reg].var = NULL;
}       


/* Method: SpillAllDirtyRegisters
 * ------------------------------
 * Used before flow of control change (branch, label, etc.) to
 * save contents of all dirty registers. This synchs the contents of
 * the registers with the memory locations for the variables.
 */
void X86::SpillAllDirtyRegisters()
{
  Register i;
  for (i = zero; i < NumRegs; i = (Register)(i+1)) 
    if (regs[i].var && regs[i].isGeneralPurpose && regs[i].isDirty) break;
  if (i != NumRegs) // none are dirty, don't print message to avoid confusion
    Emit("# (save modified registers before flow of control change)");
  for (i = zero; i < NumRegs; i = (Register)(i+1)) 
    if (regs[i].isGeneralPurpose && regs[i].var)
      SpillRegister(i);
}


/* Method: SpillForEndFunction
 * ---------------------------
 * Slight optimization on the above method used when spilling for
 * end of function (return/fall off). In such a case, we do not
 * need to save values for locals, temps, and parameters because the
 * function is about to exit and those variables are going away
 * immediately, so no need to bother with updating contents.
 */
void X86::SpillForEndFunction()
{
  Register i;
  for (i = zero; i < NumRegs; i = (Register)(i+1)) {
    if (regs[i].isGeneralPurpose && regs[i].var) {
	if (regs[i].var->GetSegment() == gpRelative)
	  SpillRegister(i);
	else {  // all stack variables can just be tossed at end func
    	  regs[i].isDirty = false;
	  regs[i].var = NULL;
	}
    }
  }
}


/* Method: Emit
 * ------------
 * General purpose helper used to emit assembly instructions in
 * a reasonable tidy manner.  Takes printf-style formatting strings
 * and variable arguments.
 */
void X86::Emit(const char *fmt, ...)
{
  va_list args;
  char buf[1024];
  
  va_start(args, fmt);
  vsprintf(buf, fmt, args);
  va_end(args);
  if (buf[strlen(buf) - 1] != ':') printf("\t"); // don't tab in labels
  if (buf[0] != '#') printf("  ");   // outdent comments a little
  printf("%s", buf);
  if (buf[strlen(buf)-1] != '\n') printf("\n"); // end with a newline
}

void X86::EmitDeclareGlobal(const char* s)
{
#ifdef CYGWIN
	Emit(".comm %s , %d",s,4);
#else
	Emit(".comm %s , %d , %d",s,4,4);
#endif
}

/* Method: EmitLoadConstant
 * ------------------------
 * Used to assign variable an integer constant value.  Slaves dst into
 * a register (using GetRegister above) and then emits an li (load
 * immediate) instruction with the constant value.
 */
void X86::EmitLoadConstant(Location *dst, int val)
{
  Register reg = GetRegisterForWrite(dst);
  Emit("movl\t$%d, %s\t# load constant value %d into %s", val, regs[reg].name, val, regs[reg].name);
}

/* Method: EmitLoadStringConstant
 * ------------------------------
 * Used to assign a variable a pointer to string constant. Emits
 * assembly directives to create a new null-terminated string in the
 * data segment and assigns it a unique label. Slaves dst into a register
 * and loads that label address into the register.
 */
void X86::EmitLoadStringConstant(Location *dst, const char *str)
{
  static int strNum = 0;
  char label[16];
  sprintf(label, "_string%d", strNum++);
  Emit(".data\t\t\t# create string constant marked with label");
  Emit("%s: .asciz %s", label, str);
  Emit(".text");
  EmitLoadLabel(dst, label);
}


/* Method: EmitLoadLabel
 * ---------------------
 * Used to load a label (ie address in text/data segment) into a variable.
 * Slaves dst into a register and emits an la (load address) instruction
 */
void X86::EmitLoadLabel(Location *dst, const char *label)
{
  Register reg = GetRegisterForWrite(dst);
  Emit("movl\t$%s, %s\t# load label", label ,regs[reg].name);
}
 

/* Method: EmitCopy
 * ----------------
 * Used to copy the value of one variable to another.  Slaves both
 * src and dst into registers and then emits a move instruction to
 * copy the contents from src to dst.
 */
void X86::EmitCopy(Location *dst, Location *src)
{
  Register rSrc = GetRegister(src), rDst = GetRegisterForWrite(dst, rSrc);
  Emit("movl\t%s, %s\t# copy value", regs[rSrc].name, regs[rDst].name);
}


/* Method: EmitLoad
 * ----------------
 * Used to assign dst the contents of memory at the address in reference,
 * potentially with some positive/negative offset (defaults to 0).
 * Slaves both ref and dst to registers, then emits a lw instruction
 * using constant-offset addressing mode y(rx) which accesses the address
 * at an offset of y bytes from the address currently contained in rx.
 */
void X86::EmitLoad(Location *dst, Location *reference, int offset)
{
  Register rSrc = GetRegister(reference), rDst = GetRegisterForWrite(dst, rSrc);
  Emit("movl\t%d(%s), %s\t# load with offset", offset, regs[rSrc].name, regs[rDst].name);
}


/* Method: EmitStore
 * -----------------
 * Used to write value to  memory at the address in reference,
 * potentially with some positive/negative offset (defaults to 0).
 * Slaves both ref and dst to registers, then emits a sw instruction
 * using constant-offset addressing mode y(rx) which writes to the address
 * at an offset of y bytes from the address currently contained in rx.
 */
void X86::EmitStore(Location *reference, Location *value, int offset)
{
  Register rVal = GetRegister(value), rRef = GetRegister(reference, rVal);
  Emit("movl\t%s, %d(%s)\t# store with offset", regs[rVal].name, offset, regs[rRef].name);
}


/* Method: EmitBinaryOp
 * --------------------
 * Used to perform a binary operation on 2 operands and store result
 * in dst. All binary forms for arithmetic, logical, relational, equality
 * use this method. Slaves both operands and dst to registers, then
 * emits the appropriate instruction by looking up the X86 name
 * for the particular op code.
 */
void X86::EmitBinaryOp(BinaryOp::OpCode code, Location *dst, Location *op1, Location *op2)
{
	Register rLeft,  rRight, rDst;

	if (!strcmp(NameForTac(code), "imull")) {
		SpillRegister(edx);
		SpillRegister(eax);		
		rRight = GetRegister(op2, eax, edx);
		Emit("movl\t%s, %%eax", regs[rRight].name);
		SpillRegister(rRight);		
		rLeft = GetRegister(op1, eax, edx);
		Emit("imull\t%s", regs[rLeft].name);  	
		BindVarWithRegister(dst, eax, ForWrite);					
	}	
	else if (!strcmp(NameForTac(code), "idivl")) {
		SpillRegister(edx);
		SpillRegister(eax);		
		rLeft = GetRegister(op1, eax, edx);
		Emit("movl\t%s, %%eax", regs[rLeft].name);
		Emit("cltd");
		SpillRegister(rLeft);		
		rRight = GetRegister(op2, eax, edx);
		Emit("idivl\t%s", regs[rRight].name);  	
		BindVarWithRegister(dst, eax, ForWrite);					
	}		
	else if (!strcmp(NameForTac(code), "reml")) {
		SpillRegister(edx);
		SpillRegister(eax);				
		rLeft = GetRegister(op1, eax, edx);
		Emit("movl\t%s, %%eax", regs[rLeft].name);
		Emit("cltd");
		SpillRegister(rLeft);		
		rRight = GetRegister(op2, eax, edx);
		Emit("idivl\t%s", regs[rRight].name); 
		Emit("movl\t%%edx, %%eax");  	
		BindVarWithRegister(dst, eax, ForWrite);					
	}		
	else if (!strcmp(NameForTac(code), "seql")) {
		rLeft = GetRegister(op1);
		rRight = GetRegister(op2, rLeft);
		rDst = GetRegisterForWrite(dst, rLeft, rRight);
		Emit("cmpl\t%s, %s", regs[rRight].name, regs[rLeft].name);
		SpillRegister(eax);
		Emit("movl\t$0, %%eax");  					
		Emit("sete\t%%al");  			
		Emit("movl\t%%eax, %s", regs[rDst].name);  	
		BindVarWithRegister(dst, eax, ForWrite);					
	}		
	else if (!strcmp(NameForTac(code), "sltl")) {
		rLeft = GetRegister(op1);
		rRight = GetRegister(op2, rLeft);
		rDst = GetRegisterForWrite(dst, rLeft, rRight);
		Emit("cmpl\t%s, %s", regs[rRight].name, regs[rLeft].name);
		SpillRegister(eax);
		Emit("movl\t$0, %%eax");  					
		Emit("setl\t%%al");  			
		Emit("movl\t%%eax, %s", regs[rDst].name);  	
		BindVarWithRegister(dst, eax, ForWrite);			
	}		
	else if (!strcmp(NameForTac(code), "sltle")) {
		rLeft = GetRegister(op1);
		rRight = GetRegister(op2, rLeft);
		rDst = GetRegisterForWrite(dst, rLeft, rRight);
		Emit("cmpl\t%s, %s", regs[rRight].name, regs[rLeft].name);
		SpillRegister(eax);
		Emit("movl\t$0, %%eax");  					
		Emit("setle\t%%al");  			
		Emit("movl\t%%eax, %s", regs[rDst].name);  	
		BindVarWithRegister(dst, eax, ForWrite);			
	}		
	else if (!strcmp(NameForTac(code), "sneql")) {
		rLeft = GetRegister(op1);
		rRight = GetRegister(op2, rLeft);
		rDst = GetRegisterForWrite(dst, rLeft, rRight);
		Emit("cmpl\t%s, %s", regs[rRight].name, regs[rLeft].name);
		SpillRegister(eax);
		Emit("movl\t$0, %%eax");  					
		Emit("setnz\t%%al");  			
		Emit("movl\t%%eax, %s", regs[rDst].name);  	
		BindVarWithRegister(dst, eax, ForWrite);					
	}		
	else if (!strcmp(NameForTac(code), "notl")) {
		/*
		rLeft = GetRegister(op1);
		rRight = GetRegister(op2, rLeft);
		rDst = GetRegisterForWrite(dst, rLeft, rRight);
		Emit("cmpl\t%s, %s", regs[rRight].name, regs[rLeft].name);
		SpillRegister(eax);
		Emit("movl\t$0, %%eax");  					
		Emit("setnz\t%%al");  			
		Emit("movl\t%%eax, %s", regs[rDst].name);  	
		*/
		rRight=GetRegister(op2);
		Emit("notl\t%s",regs[rRight].name);
		Emit("andl\t$0x1, %s",regs[rRight].name);
		BindVarWithRegister(dst, rRight, ForWrite);					
	}		
	else {
		rLeft = GetRegister(op1);
		rRight = GetRegister(op2, rLeft);
		rDst = GetRegisterForWrite(dst, rLeft, rRight);
		Emit("movl\t%s, %s", regs[rLeft].name, regs[rDst].name);
		Emit("%s\t%s, %s", NameForTac(code), regs[rRight].name,regs[rDst].name);  	
	}
}


/* Method: EmitLabel
 * -----------------
 * Used to emit label marker. Before a label, we spill all registers since
 * we can't be sure what the situation upon arriving at this label (ie
 * starts new basic block), and rather than try to be clever, we just
 * wipe the slate clean.
 */
void X86::EmitLabel(const char *label)
{ 
  SpillAllDirtyRegisters(); 
  Emit("%s:", label);
}


/* Method: EmitGoto
 * ----------------
 * Used for an unconditional transfer to a named label. Before a goto,
 * we spill all registers, since we don't know what the situation is
 * we are heading to (ie this ends current basic block) and rather than
 * try to be clever, we just wipe slate clean.
 */
void X86::EmitGoto(const char *label)
{
  SpillAllDirtyRegisters(); 
  Emit("jmp\t%s\t# unconditional branch", label);
}


/* Method: EmitIfZ
 * ---------------
 * Used for a conditional branch based on value of test variable.
 * We slave test var to register and use in the emitted test instruction,
 * either jz. See comments above on Goto for why we spill
 * all registers here.
 */
void X86::EmitIfZ(Location *test, const char *label)
{ 
  Register testReg = GetRegister(test);
  SpillAllDirtyRegisters();
  Emit("cmpl\t$0, %s\t# cmpl %s with 0", regs[testReg].name, test->GetName());
  Emit("jz\t%s\t# jmp if %s is zero ", label, test->GetName());
}


/* Method: EmitParam
 * -----------------
 * Used to push a parameter on the stack in anticipation of upcoming
 * function call. Decrements the stack pointer by 4. Slaves argument into
 * register and then stores contents to location just made at end of
 * stack.
 */
void X86::EmitParam(Location *arg)
{ 
  Register reg = GetRegister(arg);
  Emit("pushl\t%s\t# copy param value to stack", regs[reg].name);
}


/* Method: EmitCallInstr
 * ---------------------
 * Used to effect a function call. All necessary arguments should have
 * already been pushed on the stack, this is the last step that
 * transfers control from caller to callee.  See comments on Goto method
 * above for why we spill all registers before making the jump. We issue call.
 * If there is an expected result passed, we slave the var to a register and
 * copy function return value from %eax into that register.  
 */
void X86::EmitCallInstr(Location *result, const char *fn, bool isLabel)
{
  SpillAllDirtyRegisters();
  if (isLabel)
	Emit("call\t%s\t# jump to function", fn);
  else
  	Emit("call\t*%s\t# jump to function", fn);

  if (result != NULL) {
    Register r1 = GetRegisterForWrite(result);
    Emit("movl\t%s, %s\t# copy function return value from %%eax", regs[eax].name, regs[r1].name);
  }
}

// Two covers for the above method for specific ACall/LCall variants
void X86::EmitACall(Location *dst, Location *fn)
{
  EmitCallInstr(dst, regs[GetRegister(fn)].name, false);
}

void X86::EmitLCall(Location *dst, const char *label)
{ 
	static int jmpEntryNum = 0;
	char temp[16];
/*
	builtins[] =  {
	 {"_ReadInt", 0, true},
        {"_ReadDouble", 0, true}
        {"_ReadLine", 0, true},
        {"_ReadBool", 0, true},
        {"_PrintInt", 1, false},
        {"_PrintDouble", 1, false},
        {"_PrintString", 1, false},
        {"_PrintBool", 1, false},
        {"_Alloc", 1, true},
        {"_StringEqual", 2, true},
        {"_Halt", 0, false}};
*/

	SpillAllDirtyRegisters();
	/*
	if (!strcmp(label, "_ReadInt")) {
		Emit("# the next 2 instructions make %%ebx = %%esp - %%ebp");
		Emit("movl\t%%esp, %%ebx");	
		Emit("subl\t%%ebp, %%ebx");
		Emit("movl\t%%esp, %%esi\t# save %%esp in %%esi");		
		Emit("subl\t$24, %%esp");
		Emit("movl\t$0, -4(%%ebp, %%ebx)");
		Emit("subl\t$12, %%esp");
		Emit("pushl\t$1");
		Emit("call\tmalloc");
		Emit("addl\t$16, %%esp");
		Emit("movl\t%%eax, -12(%%ebp, %%ebx)");
		Emit("call\tgetchar");
		Emit("movb\t%%al, -5(%%ebp, %%ebx)");
		sprintf(temp, "_JL%d", jmpEntryNum++);
  		EmitLabel(temp);
		Emit("cmpb\t$10, -5(%%ebp, %%ebx)");
		sprintf(temp, "jne\t_JL%d", jmpEntryNum);
		Emit(temp);
		sprintf(temp, "jmp\t_JL%d", jmpEntryNum+1);
		Emit(temp);
		sprintf(temp, "_JL%d", jmpEntryNum++);
  		EmitLabel(temp);
		Emit("subl\t$8, %%esp");
		Emit("movl\t-4(%%ebp, %%ebx), %%eax");
		Emit("addl\t$2, %%eax");
		Emit("pushl\t%%eax");
		Emit("pushl\t-12(%%ebp, %%ebx)");
		Emit("call\trealloc");
		Emit("addl\t$16, %%esp");
		Emit("movl\t%%eax, -12(%%ebp, %%ebx)");
		Emit("movl\t-4(%%ebp, %%ebx), %%eax");
		Emit("movl\t-12(%%ebp, %%ebx), %%edx");
		Emit("addl\t%%eax, %%edx");
		Emit("movb\t-5(%%ebp, %%ebx), %%al");
		Emit("movb\t%%al, (%%edx)");
		Emit("leal\t-4(%%ebp, %%ebx), %%eax");
		Emit("incl\t(%%eax)");
		Emit("call\tgetchar");
		Emit("movb\t%%al, -5(%%ebp, %%ebx)");
		sprintf(temp, "jmp\t_JL%d", jmpEntryNum-2);
		Emit(temp);
		sprintf(temp, "_JL%d", jmpEntryNum++);
  		EmitLabel(temp);
		Emit("movl\t-4(%%ebp, %%ebx), %%eax");
		Emit("addl\t-12(%%ebp, %%ebx), %%eax");
		Emit("movb\t$0, (%%eax)");
		Emit("pushl\t-12(%%ebp, %%ebx)");
		Emit("call\tatoi");
		Emit("movl\t%%eax, %%ebx");
		Emit("call\tfree");
		Emit("movl\t%%ebx, %%eax");		
		Emit("movl\t%%esi, %%esp\t# restore %%esp");
	}	
	else if (!strcmp(label, "_ReadDouble")) {
		Assert(0);
	}		
	else if (!strcmp(label, "_ReadLine")) {
		Emit("# the next 2 instructions make %%ebx = %%esp - %%ebp");
		Emit("movl\t%%esp, %%ebx");	
		Emit("subl\t%%ebp, %%ebx");
		Emit("movl\t%%esp, %%esi\t# save %%esp in %%esi");		
		Emit("subl\t$24, %%esp");
		Emit("movl\t$0, -4(%%ebp, %%ebx)");
		Emit("subl\t$12, %%esp");
		Emit("pushl\t$1");
		Emit("call\tmalloc");
		Emit("addl\t$16, %%esp");
		Emit("movl\t%%eax, -12(%%ebp, %%ebx)");
		Emit("call\tgetchar");
		Emit("movb\t%%al, -5(%%ebp, %%ebx)");
		sprintf(temp, "_JL%d", jmpEntryNum++);
  		EmitLabel(temp);
		Emit("cmpb\t$10, -5(%%ebp, %%ebx)");
		sprintf(temp, "jne\t_JL%d", jmpEntryNum);
		Emit(temp);
		sprintf(temp, "jmp\t_JL%d", jmpEntryNum+1);
		Emit(temp);
		sprintf(temp, "_JL%d", jmpEntryNum++);
  		EmitLabel(temp);
		Emit("subl\t$8, %%esp");
		Emit("movl\t-4(%%ebp, %%ebx), %%eax");
		Emit("addl\t$2, %%eax");
		Emit("pushl\t%%eax");
		Emit("pushl\t-12(%%ebp, %%ebx)");
		Emit("call\trealloc");
		Emit("addl\t$16, %%esp");
		Emit("movl\t%%eax, -12(%%ebp, %%ebx)");
		Emit("movl\t-4(%%ebp, %%ebx), %%eax");
		Emit("movl\t-12(%%ebp, %%ebx), %%edx");
		Emit("addl\t%%eax, %%edx");
		Emit("movb\t-5(%%ebp, %%ebx), %%al");
		Emit("movb\t%%al, (%%edx)");
		Emit("leal\t-4(%%ebp, %%ebx), %%eax");
		Emit("incl\t(%%eax)");
		Emit("call\tgetchar");
		Emit("movb\t%%al, -5(%%ebp, %%ebx)");
		sprintf(temp, "jmp\t_JL%d", jmpEntryNum-2);
		Emit(temp);
		sprintf(temp, "_JL%d", jmpEntryNum++);
  		EmitLabel(temp);
		Emit("movl\t-4(%%ebp, %%ebx), %%eax");
		Emit("addl\t-12(%%ebp, %%ebx), %%eax");
		Emit("movb\t$0, (%%eax)");
		Emit("movl\t-12(%%ebp, %%ebx), %%eax");
		Emit("movl\t%%esi, %%esp\t# restore %%esp");
	}
	else if (!strcmp(label, "_ReadBool")) {
		Assert(0);
	}
	else if (!strcmp(label, "_PrintInt")) {
		Emit("movl\t%%esp, %%esi\t# save %%esp in %%esi");
		Emit("pushl\t$_IntStyle");		
		Emit("call\tprintf");
		Emit("movl\t%%esi, %%esp\t# restore %%esp");
	}
	else if (!strcmp(label, "_PrintDouble")) {
		Assert(0);
	}		
	else if (!strcmp(label, "_PrintString")) {
		Emit("call\tprintf");
	}
	else if (!strcmp(label, "_PrintBool")) {
		Emit("movl\t%%esp, %%esi\t# save %%esp in %%esi");
		Emit("subl\t$0, (%%esp)");
	  	sprintf(temp, "jz\t_JL%d", jmpEntryNum);
		Emit(temp);
		Emit("pushl\t$_True");
	  	sprintf(temp, "jmp\t_JL%d", jmpEntryNum+1);
		Emit(temp);
	  	sprintf(temp, "_JL%d", jmpEntryNum++);
  		EmitLabel(temp);
		Emit("pushl\t$_False");
	  	sprintf(temp, "_JL%d", jmpEntryNum++);
  		EmitLabel(temp);
		Emit("call\tprintf");
		Emit("movl\t%%esi, %%esp\t# restore %%esp");
	}
	else if (!strcmp(label, "_StringEqual")) {
		Emit("call\tstrcmp");
	}
	else if (!strcmp(label, "_Alloc")) {
		Emit("call\tmalloc");
	}
	else if (!strcmp(label, "_Halt")) {
		Emit("pushl\t$0");
		Emit("call\texit");
	}
	else {
	  	EmitCallInstr(dst, label, true);
	}
	*/

	EmitCallInstr(dst,label,true);

    	// handle return value  
	if (dst != NULL) {
		Register r1 = GetRegisterForWrite(dst);
		Emit("movl\t%s, %s\t# copy function return value from %%eax", regs[eax].name, regs[r1].name);
	}
}


/*
 * We remove all parameters from the stack after a cmpleted call
 * by adjusting the stack pointer upwards.
 */
void X86::EmitPopParams(int bytes)
{
  if (bytes != 0)
    Emit("addl\t$%d, %%esp\t# pop params off stack", bytes);
}


/* Method: EmitReturn
 * ------------------
 * Used to emit code for returning from a function (either from an
 * explicit return or falling off the end of the function body).
 * If there is an expression to return, we slave that variable into
 * a register and move its contents to %eax(the standard register for
 * function result).  Before exiting, we spill dirty registers (to
 * commit contents of slaved registers to memory, necessary for
 * consistency, see comments at SpillForEndFunction above). We also
 * do the last part of the callee's job in function call protocol,
 * which is to remove our locals/temps from the stack, remove
 * saved registers (%ebp) and restore previous values of %ebp
 * so everything is returned to the state we entered.
 */
 void X86::EmitReturn(Location *returnVal)
{ 
  if (returnVal != NULL) 
    Emit("movl\t%s, %%eax\t# assign return value into %%eax",
	     regs[GetRegister(returnVal)].name);
  SpillForEndFunction();
  Emit("leave");
  Emit("ret");
}


/* Method: EmitBeginMainFunction
 * -------------------------
 */
 void X86::EmitBeginMainFunction(int stackFrameSize)
{
  Assert(stackFrameSize >= 0);
  Emit("pushl\t%%ebp");
  Emit("movl\t%%esp, %%ebp");

  if (stackFrameSize != 0)
    Emit("subl\t$%d, %%esp \t# decrement sp to make space for locals/temps",
	   stackFrameSize);

  Emit("andl\t$-16, %%esp \t# align the stack pointer for performance reasons");
  Emit("# the next two instruction is generated to initialize the accumulator, ");
  Emit("# and also to reset the condition flags");
  Emit("movl\t$0, %%eax");
  Emit("subl\t%%eax, %%esp");
  Emit("# the body of main follows");
}

/* Method: EmitBeginFunction
 * -------------------------
 * Used to handle the callee's part of the function call protocol
 * upon entering a new function. We save the current values of
 * %ebp (since we are going to  change it), then set up the new
 * %ebp, and at last sub %esp to make space for all our locals/temps.
 */
void X86::EmitBeginFunction(int stackFrameSize)
{
  Assert(stackFrameSize >= 0);
  Emit("pushl\t%%ebp");
  Emit("movl\t%%esp, %%ebp");

  if (stackFrameSize != 0)
    Emit("subl\t$%d, %%esp\t# decrement sp to make space for locals/temps",
	   stackFrameSize);
}


/* Method: EmitEndFunction
 * -----------------------
 * Used to end the body of a function. Does an implicit return in fall off
 * case to clean up stack frame, return to caller etc. See comments on
 * EmitReturn above.
 */
void X86::EmitEndFunction()
{ 
  Emit("# (below handles reaching end of fn body with no explicit return)");
  EmitReturn(NULL);
}



/* Method: EmitVTable
 * ------------------
 * Used to layout a vtable. Uses assembly directives to set up new
 * entry in data segment, emits label, and lays out the function
 * labels one after another.
 */
void X86::EmitVTable(const char *label, List<const char*> *methodLabels)
{
  Emit(".data");
  Emit(".align 4");
  Emit("%s:\t# label for class %s vtable", label, label);
  for (int i = 0; i < methodLabels->NumElements(); i++)
    Emit(".long\t%s\n", methodLabels->Nth(i));
  Emit(".text");
}

void X86::EmitVTableWithType(const char* label, List<const char*> *methodLabels, const char* typeName)
{
  Emit(".data");
  Emit(".align 4");
  Emit(".long\t%s\t# type object pointer\n",typeName);
  Emit("%s:\t# label for class %s vtable", label, label);
  for (int i = 0; i < methodLabels->NumElements(); i++)
    Emit(".long\t%s\n", methodLabels->Nth(i));
  Emit(".text");
}

void X86::EmitTypeObject(const char* l,const char* tn,const char* pn)
{
	Emit(".data");
	Emit("asz%s: .asciz\t\"%s\"\t# typeName\n",tn,tn);
	Emit(".align 4");
	Emit("%s:\t#label of %s's type object",l,tn);
	Emit(".long\tasz%s\t# typeName\n",tn);
	Emit(".long\t%s\t# parent's type object\n",pn);
	Emit(".text");
}

void X86::EmitBeginTry(const char* label)
{
	Emit("pushl %%edi\n");
	Emit("pushl %%ebp\n");
	Emit("pushl $%s\n",label);
	Emit("movl %%esp,%%edi\n");
}

void X86::EmitEndTry()
{
	Emit("movl 8(%%esp),%%edi\n");
	Emit("addl $12,%%esp\n");
}

void X86::EmitThrow()
{
	SpillAllDirtyRegisters();
	Emit("movl %%edi,%%esp\n");
	Emit("movl (%%esp),%%edx\n");
	Emit("movl 4(%%esp),%%ebp\n");
	Emit("movl 8(%%esp),%%edi\n");
	Emit("addl $12,%%esp\n");
	Emit("jmp *%%edx\n");
}

/* Method: EmitPreamble
 * --------------------
 * Used to emit the starting sequence needed for a program. Not much
 * here, but need to indicate what follows is in text segment and
 * needs to be aligned on word boundary. main is our only global symbol.
 */
void X86::EmitPreamble()
{
  Emit("# standard Curry preamble");
  /*
  Emit(".section .data");
  Emit("%s: .asciz \"%s\"", "_IntStyle", "%d");
  Emit("%s: .asciz \"%s\"", "_DoubleStyle", "%f");
  Emit("%s: .asciz \"%s\"", "_True",  "TRUE");
  Emit("%s: .asciz \"%s\"", "_False", "FALSE");
  */

#ifdef CYGWIN
  Emit(".text");
  Emit(".globl _main");
#else
  Emit(".text");
  Emit(".globl main");
#endif

//  Emit(".type _main, @function");
}


/* Method: NameForTac
 * ------------------
 * Returns the appropriate X86 instruction (add, seq, etc.) for
 * a given BinaryOp:OpCode (BinaryOp::Add, BinaryOp:Equals, etc.). 
 * Asserts if asked for name of an unset/out of bounds code.
 */
const char *X86::NameForTac(BinaryOp::OpCode code)
{
  Assert(code >=0 && code < BinaryOp::NumOps);
  const char *name = x86Name[code];
  Assert(name != NULL);
  return name;
}



/* Constructor
 * ----------
 * Constructor sets up the X86 names and register descriptors to
 * the initial starting state.
 */
X86::X86() {
  x86Name[BinaryOp::And]	=	"andl";
  x86Name[BinaryOp::Or]	= 	"orl";
  x86Name[BinaryOp::Add]	= 	"addl";
  x86Name[BinaryOp::Sub]	= 	"subl";

  // the next two tac BinaryOps need special treat.
  x86Name[BinaryOp::Mul]	= 	"imull";
  x86Name[BinaryOp::Div]	= 	"idivl";
  
  // the next three tac BinaryOps have no counterpart x86 instructuions,
  // so the right side of the symbol "=" is virtual instruction.
  // they all need special treat.
  x86Name[BinaryOp::Mod]	= 	"reml";
  x86Name[BinaryOp::Eq]	= 	"seql";
  x86Name[BinaryOp::Less]	= 	"sltl";
  
  // add other needed operation 
  x86Name[BinaryOp::LessThan] = "sltle";
  x86Name[BinaryOp::NotEq] = "sneql";
  x86Name[BinaryOp::Not] = "notl";
  
  
  regs[zero] = (RegContents){false, NULL, "%zero", false};
 
  regs[eax] = (RegContents){false, NULL, "%eax", true};
  regs[ebx] = (RegContents){false, NULL, "%ebx", true};
  regs[ecx] = (RegContents){false, NULL, "%ecx", true};
  regs[edx] = (RegContents){false, NULL, "%edx", true};
  regs[esi]  = (RegContents){false, NULL, "%esi",  true};
  
  //regs[edi]  = (RegContents){false, NULL, "%edi",  true};
  regs[edi]  = (RegContents){false, NULL, "%edi",  false};

  regs[ebp] = (RegContents){false, NULL, "%ebp", false};
  regs[esp] = (RegContents){false, NULL, "%esp", false};

  lastUsed = zero;
}

const char *X86::x86Name[BinaryOp::NumOps];

