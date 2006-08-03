/* File: x86.h
 * ------------
 * The x86 class defines an object capable of emitting x86
 * instructions and managing the allocation and use of registers.
 * It is used by the Tac instruction classes to convert each
 * instruction to the appropriate x86 equivalent.
 *
 * You can scan this code to see it works and get a sense of what
 * it does.  You will not need to modify this class unless
 * you're attempting some machine-specific optimizations. 
 *
 * It comments the emitted assembly but the commenting for the code
 * in the class itself is pretty sparse. The SPIM manual (see link
 * from other materials on our web site) has more detailed documentation
 * on the x86 architecture, instruction set, calling conventions, etc.
 */

#ifndef _H_x86
#define _H_x86

#include "tac.h"
#include "list.h"
class Location;


class X86 {
  private:
    // the register "zero" is a virtual register which is just 
    // used to help implement GetRegister()
    typedef enum {zero, eax, ebx, ecx, edx, esi, edi,
                             ebp, esp, NumRegs } Register;

    struct RegContents {
	bool isDirty;
	Location *var;
	const char *name;
	bool isGeneralPurpose;
    } regs[NumRegs];

    Register lastUsed;

    typedef enum { ForRead, ForWrite } Reason;

    // general purpose
    Register GetRegister(Location *var, Reason reason, Register avoid1, Register avoid2);
    // for read
    Register GetRegister(Location *var, Register avoid1 = zero, Register avoid1 = zero);
    // for write
    Register GetRegisterForWrite(Location *var, Register avoid1 = zero, Register avoid2 = zero);
    // bind a variable with a given register
    void BindVarWithRegister(Location *var,  Register reg, Reason reason = ForRead);

    bool FindRegisterWithContents(Location *var, Register& reg);
    bool FindNullRegister(Register& reg, Register avoid1 = zero, Register avoid2 = zero);
    Register SelectRegisterToSpill(Register avoid1, Register avoid2);
    void SpillRegister(Register reg);
    void SpillAllDirtyRegisters();
    void SpillForEndFunction();

    void EmitCallInstr(Location *dst, const char *fn, bool isL);
    
    static const char *x86Name[BinaryOp::NumOps];
    static const char *NameForTac(BinaryOp::OpCode code);

 public:
    
    X86();

    static void Emit(const char *fmt, ...);
    
    void EmitLoadConstant(Location *dst, int val);
    void EmitLoadStringConstant(Location *dst, const char *str);
    void EmitLoadLabel(Location *dst, const char *label);

    void EmitLoad(Location *dst, Location *reference, int offset);
    void EmitStore(Location *reference, Location *value, int offset);
    void EmitCopy(Location *dst, Location *src);
    
    void EmitBinaryOp(BinaryOp::OpCode code, Location *dst, 
			    Location *op1, Location *op2);

    void EmitLabel(const char *label);
    void EmitGoto(const char *label);
    void EmitIfZ(Location *test, const char*label);
    void EmitReturn(Location *returnVal);
    
    void EmitBeginMainFunction(int frameSize);
    void EmitBeginFunction(int frameSize);
    void EmitEndFunction();

    void EmitParam(Location *arg);
    void EmitLCall(Location *result, const char* label);
    void EmitACall(Location *result, Location *fnAddr);
    void EmitPopParams(int bytes);
    
    void EmitVTable(const char *label, List<const char*> *methodLabels);

    void EmitPreamble();
};

#endif
 
