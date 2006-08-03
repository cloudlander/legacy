// FileFindEx.cpp: implementation of the CFileFindEx class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "wmd5.h"
#include "FileFindEx.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CFileFindEx::CFileFindEx()
{

}

CFileFindEx::~CFileFindEx()
{

}

void CFileFindEx::GetLength64(LONGLONG &length)
{
	ASSERT(m_hContext != NULL);
	ASSERT_VALID(this);

	if (m_pFoundInfo != NULL)
	{
		LONGLONG low=((LPWIN32_FIND_DATA) m_pFoundInfo)->nFileSizeLow ;
		LONGLONG high=((LPWIN32_FIND_DATA) m_pFoundInfo)->nFileSizeHigh;
//		length= ((LPWIN32_FIND_DATA) m_pFoundInfo)->nFileSizeLow +
//				(((LPWIN32_FIND_DATA) m_pFoundInfo)->nFileSizeHigh << 32);
		length=(high << 32) + low;
	}
	else
		length= 0;
}
