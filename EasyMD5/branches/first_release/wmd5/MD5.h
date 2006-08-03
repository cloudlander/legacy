// MD5.h: interface for the MD5 class.
// modified by Ronny(zhangxiaomin@gmail.com)
// see copyright info below
//////////////////////////////////////////////////////////////////////
/* 
	Copyright (C) 1991-2, RSA Data Security, Inc. Created 1991. All 
	rights reserved.

	License to copy and use this software is granted provided that it
	is identified as the "RSA Data Security, Inc. MD5 Message-Digest
	Algorithm" in all material mentioning or referencing this software
	or this function.
  
	License is also granted to make and use derivative works provided
	that such works are identified as "derived from the RSA Data
	Security, Inc. MD5 Message-Digest Algorithm" in all material
	mentioning or referencing the derived work.
			
	RSA Data Security, Inc. makes no representations concerning either
	the merchantability of this software or the suitability of this
	software for any particular purpose. It is provided "as is"
	without express or implied warranty of any kind.
			  
	These notices must be retained in any copies of any part of this
	documentation and/or software.
*/

#if !defined(AFX_MD5_H__EE05B742_1916_48C6_8523_214774198B43__INCLUDED_)
#define AFX_MD5_H__EE05B742_1916_48C6_8523_214774198B43__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/* PROTOTYPES should be set to one if and only if the compiler supports
  function argument prototyping.
The following makes PROTOTYPES default to 0 if it has not already
  been defined with C compiler flags.
 */
#ifndef PROTOTYPES
#define PROTOTYPES 0
#endif
/* POINTER defines a generic pointer type */
typedef unsigned char *POINTER;

/* UINT2 defines a two byte word */
typedef unsigned short int UINT2;

/* UINT4 defines a four byte word */
typedef unsigned long int UINT4;

/* PROTO_LIST is defined depending on how PROTOTYPES is defined above.
If using PROTOTYPES, then PROTO_LIST returns the list, otherwise it
  returns an empty list.
 */
#if PROTOTYPES
#define PROTO_LIST(list) list
#else
#define PROTO_LIST(list) ()
#endif



class MD5  
{
	typedef struct {
		UINT4 state[4];                                   /* state (ABCD) */
		UINT4 count[2];        /* number of bits, modulo 2^64 (lsb first) */
		unsigned char buffer[64];                         /* input buffer */
	} MD5_CTX;

public:
	MD5();
	virtual ~MD5();

	void SessionInit();
	void SessionUpdate(unsigned char*,unsigned int);
	void SessionFinal(unsigned char [16]);
/*
	void MD5Init (MD5_CTX *);
	void MD5Update (MD5_CTX *, unsigned char *, unsigned int);
	void MD5Final (unsigned char [16], MD5_CTX *);
*/	
private:
	static unsigned char PADDING[64];
	MD5_CTX ctx;		

	void MD5Transform (UINT4 [4], unsigned char [64]);
	void Encode 
		(unsigned char *, UINT4 *, unsigned int);
	void Decode 
		(UINT4 *, unsigned char *, unsigned int);
	void MD5_memcpy (POINTER, POINTER, unsigned int);
	void MD5_memset (POINTER, int, unsigned int);
};

#endif // !defined(AFX_MD5_H__EE05B742_1916_48C6_8523_214774198B43__INCLUDED_)
