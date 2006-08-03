// FileFindEx.h: interface for the CFileFindEx class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_FILEFINDEX_H__CDF63821_248F_456B_8490_C70BC7F09969__INCLUDED_)
#define AFX_FILEFINDEX_H__CDF63821_248F_456B_8490_C70BC7F09969__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CFileFindEx : public CFileFind  
{
public:
	void GetLength64(LONGLONG& length);
	CFileFindEx();
	virtual ~CFileFindEx();

};

#endif // !defined(AFX_FILEFINDEX_H__CDF63821_248F_456B_8490_C70BC7F09969__INCLUDED_)
