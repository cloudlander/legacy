// GpaCore.h: interface for the CGpaCore class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_GPACORE_H__3C5819E0_E20C_11D6_BD0E_00505647C9B5__INCLUDED_)
#define AFX_GPACORE_H__3C5819E0_E20C_11D6_BD0E_00505647C9B5__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include <process.h>
#include "TipWnd.h"
/////////////////////////////////////////////////////////////////////////////
// CGpaDlg dialog
typedef map<tstring,tstring,less<tstring> > MGRADE;

class CGpaCore  
{
public:
	CGpaCore()
	{
		m_MapOfGrade.insert(MGRADE::value_type("A","4.0"));
		m_MapOfGrade.insert(MGRADE::value_type("A-","3.7"));
		m_MapOfGrade.insert(MGRADE::value_type("B+","3.3"));
		m_MapOfGrade.insert(MGRADE::value_type("B","3.0"));
		m_MapOfGrade.insert(MGRADE::value_type("B-","2.7"));
		m_MapOfGrade.insert(MGRADE::value_type("C+","2.3"));
		m_MapOfGrade.insert(MGRADE::value_type("C","2.0"));
		m_MapOfGrade.insert(MGRADE::value_type("C-","1.7"));
		m_MapOfGrade.insert(MGRADE::value_type("D+","1.3"));
		m_MapOfGrade.insert(MGRADE::value_type("D","1.0"));
		m_MapOfGrade.insert(MGRADE::value_type("D-","0.7"));
		m_MapOfGrade.insert(MGRADE::value_type("E","0.5"));
		m_MapOfGrade.insert(MGRADE::value_type("F","0.0"));
	}

	virtual ~CGpaCore();

	BOOL PostData(HINTERNET hSess,TCHAR* cgi,TCHAR* header,TCHAR* param);
	BOOL GetScoreHref();
	void ParseTable();
	void GenTblHead(fstream& of);
	void GenTblTail(fstream& of);
	void GenRow(tstring& szData,fstream& of);
	void WriteDoc(MSHTML::IHTMLDocument3Ptr&);
	void QueryDone(int res);

	HWND m_hHost;
	HWND m_hWebBrowser;
	IWebBrowser2* m_pFrameWB;
	int m_Sess;
	CTipWnd* m_TipWnd;
	tstring m_szBuffer;
	tstring m_StudentNo;
	tstring m_Passwd;
	map<tstring,tstring,less<tstring> > m_MapOfGrade;

};

#endif // !defined(AFX_GPACORE_H__3C5819E0_E20C_11D6_BD0E_00505647C9B5__INCLUDED_)
