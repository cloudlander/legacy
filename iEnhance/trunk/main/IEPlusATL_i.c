/* this file contains the actual definitions of */
/* the IIDs and CLSIDs */

/* link this file in with the server and any clients */


/* File created by MIDL compiler version 5.01.0164 */
/* at Tue Sep 03 13:48:00 2002
 */
/* Compiler settings for E:\My Documents\My_Projects\IEPlusATL\IEPlusATL.idl:
    Os (OptLev=s), W1, Zp8, env=Win32, ms_ext, c_ext
    error checks: allocation ref bounds_check enum stub_data 
*/
//@@MIDL_FILE_HEADING(  )
#ifdef __cplusplus
extern "C"{
#endif 


#ifndef __IID_DEFINED__
#define __IID_DEFINED__

typedef struct _IID
{
    unsigned long x;
    unsigned short s1;
    unsigned short s2;
    unsigned char  c[8];
} IID;

#endif // __IID_DEFINED__

#ifndef CLSID_DEFINED
#define CLSID_DEFINED
typedef IID CLSID;
#endif // CLSID_DEFINED

//const IID IID_IIEPlus = {0x4CAFB651,0x71B4,0x43F8,{0xB6,0x3F,0x5D,0x19,0x64,0xA7,0x5B,0x75}};


//const IID LIBID_IEPLUSATLLib = {0x124FBBBB,0x90EF,0x4EAB,{0xB5,0xA8,0xB7,0x8E,0x7A,0x64,0x86,0x3C}};


const CLSID CLSID_IEPlus = {0x83C64D70,0xF259,0x4384,{0x84,0x43,0x21,0x73,0x51,0x43,0x6B,0xE6}};


#ifdef __cplusplus
}
#endif

