#include <stdio.h>

//extern char True,False;



unsigned int NewClass(int size,int vtbaddr)

{

	unsigned int *addr=(unsigned int*)malloc(size);

	(*addr)=vtbaddr;

//	printf("%s,%s\n",&True,&False);

	return (unsigned int)(addr);

}



unsigned int NewArray(int length,int typesize)

{

	unsigned int *addr=(unsigned int*)malloc(length*typesize+4);

	*addr=length;

	return (unsigned int)(addr+1);

}



unsigned int ArrayLength(unsigned int addr)

{

	return (unsigned int)*((unsigned int*)addr-1);

}



unsigned int EqualString(char* s1,char* s2)

{

	if(0==strcmp(s1,s2))

		return 1;

	else

		return 0;

}

