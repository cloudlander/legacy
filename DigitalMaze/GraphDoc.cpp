// GraphDoc.cpp : implementation of the CGraphDoc class
//

#include "stdafx.h"
#include "Graph.h"

#include "GraphDoc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CGraphDoc

IMPLEMENT_DYNCREATE(CGraphDoc, CDocument)

BEGIN_MESSAGE_MAP(CGraphDoc, CDocument)
	//{{AFX_MSG_MAP(CGraphDoc)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CGraphDoc construction/destruction

CGraphDoc::CGraphDoc()
{
	// TODO: add one-time construction code here

}

CGraphDoc::~CGraphDoc()
{
}

BOOL CGraphDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: add reinitialization code here
	// (SDI documents will reuse this document)

	return TRUE;
}



/////////////////////////////////////////////////////////////////////////////
// CGraphDoc serialization

void CGraphDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// TODO: add storing code here
	}
	else
	{
		// TODO: add loading code here
	}
}

/////////////////////////////////////////////////////////////////////////////
// CGraphDoc diagnostics

#ifdef _DEBUG
void CGraphDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CGraphDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CGraphDoc commands
