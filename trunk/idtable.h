#ifndef _IDT_H
#define _IDT_H

/* The idtable is allocated dynamically(use realloc)
 *
 *
 */


// A dynamic allocated place(a dynamic array) to hold all names
char* Name_Table;

typedef enum{VAR_ID=1,ARRAY_ID} ID_TYPE;
typedef enum{INTTYPE=1,BOOLTYPE} BASIC_TYPE;
typedef union{
	int v_int; //integer variable's value is held here
	bool v_bool;//boolean variable's value is held here
} DATA_VALUE;

typedef struct{
	//BASIC_TYPE type;
	int dim;
	int low[8];
	int high[8];
	int ww[8];
	int base;
	int total;
	int value_base;//the base place in data section to hold value
} ARRAY_INFO_VECTOR;


typedef struct{
	char* name_ptr;
	TYPE  id_type;
	BASIC_TYPE id_attr;
	DATA_VALUE id_value;
	ARRAY_INFO_VECTOR *a_info_v;//only for arrays
} IDT_Entry;

#endif
