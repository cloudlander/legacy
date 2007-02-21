#ifndef QUAD_H
#define QUAD_H

#include "symtable.h"

class Operand
{
public:
		bool lvalue;
		ValueType type;
		
		Symbol* sym;	// point to a symbol in symtable		
		List<Operand*>* indexes;

		char* name;	// Quad's value as an operand

		union{
			int Integer;
			double Double;
			bool Boolean;
			char* String;
		}value;		// only useful for lvalue==false

		ValueType GetType(){return type;}

		Operand(ValueType vt, int t=-1);
			
		Operand(Symbol* s,List<Operand*>* d=NULL)
		{
			lvalue=true;
			sym=s;
			indexes=d;
			type=sym->GetValueType();
			name=NULL;
		}

		Operand()// identifies an error
		{
			sym=NULL;
			name=NULL;
			lvalue=false;
		}

		/* for constant operand */
		Operand(int i){type=Integer; lvalue=false; value.Integer=i;sym=NULL; name=NULL;}
		Operand(double d){ type=Double; lvalue=false; value.Double=d;sym=NULL; name=NULL;}
		Operand(bool b){ type=Boolean; lvalue=false; value.Boolean=b;sym=NULL; name=NULL;}
		Operand(char* s){ type=String; lvalue=false; value.String=s;sym=NULL; name=NULL;}

		void Print();

		void SetValue(int);
		void SetValue(double);
		void SetValue(bool);
		
		ValueType GetValue(Value* v);
			
};


class Quad
{
public:
    typedef enum {Halt, Read, Write, Assign, GoFalse, Goto, Add, Sub, Mul, Div, Mod, Eq, Less, And, Or, LessThan, NotEq, Not,NumOps} OpCode;
    static char * opName[NumOps];

	OpCode op;

	Operand* src1;	
	Operand* src2;

	Operand* dest;

	int target;	// for jump target
	char* tlabel;

	char* label;

	OpCode GetOpCode(){return op;}
	int Execute();

	Quad(Operand* d,OpCode oc,Operand* s1=NULL,Operand* s2=NULL);
	void SetTarget(int t);
	
	void Print();

	void SetLabel(int l)
	{
		char buf[20];
		sprintf(buf,"L%d",l);
		label=new char[strlen(buf)+1];
		strncpy(label,buf,strlen(buf));
	}
};

extern List<Quad*> insts;

#endif
