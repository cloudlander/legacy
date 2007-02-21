#include "quad.h"

void Run()
{
	int pc=0;
	Quad* inst=insts.Nth(pc);	// start
	while(pc<insts.NumElements() && Quad::Halt != inst->GetOpCode())
	{
		if(Quad::GoFalse == inst->GetOpCode() ||
		   Quad::Goto == inst->GetOpCode())
		{
			int i=inst->Execute();
			if(i>=0)
			{	
				inst=insts.Nth(i);
				pc=i;
				continue;
			}
		}
		else
			inst->Execute();
		inst=insts.Nth(++pc);
	}
}
