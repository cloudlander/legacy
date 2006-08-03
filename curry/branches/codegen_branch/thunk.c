#include <stdio.h>



char *True="TRUE";

char *False="FALSE";



extern unsigned int exception_value;

extern unsigned int exception_type;

extern unsigned int exception_type_dim;



typedef struct tagTypeInfo{

	unsigned char* typename;

	struct tagTypeInfo* parent;

}TypeInfo;



enum BuiltInExceptionType{

	IndexOutOfBound,

	NewObjectFailure,

	ArraySize,

};



TypeInfo BuiltInExceptions[]={

	{"IndexOutOfBoundException",NULL},

	{"NewObjectFailureException",NULL},

	{"ArraySizeException", NULL},

};



void _throw_exception(enum BuiltInExceptionType bie_type)

{

	exception_value=0;		// ignore value

	exception_type=(unsigned int)&BuiltInExceptions[bie_type];

	exception_type_dim=0;	// always 0

	asm volatile ("movl %edi,%esp");

	asm volatile ("movl (%esp),%edx");

	asm volatile ("movl 4(%esp),%ebp");

   	asm volatile ("movl 8(%esp),%edi");

	asm volatile ("addl $12,%esp");

	asm volatile ("jmp *%edx");

	/* never return*/

}



unsigned int ReadLine()

{

	int c=0;

	char* s=(char*)malloc(1);

	int ch=getchar();

	while(EOF!=ch && '\n'!=(char)ch)

	{

		s[c++]=(char)ch;

		ch=getchar();

		s=(char*)realloc(s,c+1);

	}

	// zero-terminate it

	s[c]='\0';

	return (unsigned int)s;

}



unsigned int ReadInt()

{

	int x;

	char* s=(char*)ReadLine();

	x=atoi(s);

	return x;

}



void PrintInt(int x)

{

	printf("%d",x);

	fflush(stdout);

}



void PrintString(char* s)

{

	printf("%s",s);	

	fflush(stdout);

}



void PrintBool(int b)

{

	if(b>0)

		printf("%s",True);

	else

		printf("%s",False);

	fflush(stdout);

}



unsigned int NewClass(int size,int vtbaddr)

{

	unsigned int *addr=(unsigned int*)malloc(size);



	if(NULL==addr)

		_throw_exception(NewObjectFailure);

	

	(*addr)=vtbaddr;



	return (unsigned int)(addr);

}



unsigned int NewArray(int length,int typesize)

{

	if(length<=0)

		_throw_exception(ArraySize);

	

	unsigned int *addr=(unsigned int*)malloc(length*typesize+4);



	if(NULL==addr)

		_throw_exception(NewObjectFailure);

	

	*addr=length;

	return (unsigned int)(addr+1);

}



unsigned int ArrayLength(unsigned int addr)

{

	return (unsigned int)*((unsigned int*)addr-1);

}



void CheckIndex(unsigned int addr, unsigned int index)

{

	int length=(unsigned int)*((unsigned int*)addr-1) - 1;

	if(index<0 || index>length)

		_throw_exception(IndexOutOfBound);

}



unsigned int EqualString(char* s1,char* s2)

{

	if(0==strcmp(s1,s2))

		return 1;

	else

		return 0;

}



unsigned int IsKindOf(unsigned int objtype,unsigned int dim)

{

//	printf("%d,%d,%d,%d\n",&exception_value,exception_type,exception_type_dim,objtype);

	

	if(dim != exception_type_dim)

		return 0;

	TypeInfo* eh_type=(TypeInfo*)exception_type;

//	printf("%s\n",eh_type->typename);



	TypeInfo* ptype=(TypeInfo*)objtype;

//	printf("%s\n",ptype->typename);



	if(0==strcmp("Integer",ptype->typename) ||

	   0==strcmp("Double",ptype->typename) ||

	   0==strcmp("Bool",ptype->typename) ||

	   0==strcmp("String",ptype->typename) )

		return objtype==exception_type;

	else

	{

		while(eh_type!=0)

		{

			if(0==strcmp(eh_type->typename,ptype->typename))

				return 1;

			else

				eh_type=eh_type->parent;

		}

	}

	return 0;

}

