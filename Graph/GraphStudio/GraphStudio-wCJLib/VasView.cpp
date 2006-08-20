// VasView.cpp : implementation file
//

#include "stdafx.h"
#include "GraphStudio.h"
#include "MainFrm.h"
#include "VasView.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CVasView
IMPLEMENT_DYNCREATE(CVasView, CScrollView)
CVasView::CVasView():m_rect(0,0,0,0),m_Org(0,0)
{
	m_bDraging = FALSE;
	m_DragTop = m_DragBottom = m_DragRight = m_DragLeft = FALSE;
	m_SizeX = m_SizeY = 0;
}

CVasView::~CVasView()
{
}


BEGIN_MESSAGE_MAP(CVasView, CScrollView)
	//{{AFX_MSG_MAP(CVasView)
	ON_COMMAND(ID_ANTI_BMP, OnAntiBmp)
	ON_COMMAND(ID_OPEN_BMP, OnOpenBmp)
	ON_COMMAND(ID_SAVE_BMP, OnSaveBmp)
	ON_WM_LBUTTONDOWN()
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONUP()
	ON_WM_CREATE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CVasView drawing

void CVasView::OnInitialUpdate()
{
	CScrollView::OnInitialUpdate();

	CSize sizeTotal;
	// TODO: calculate the total size of this view
	sizeTotal.cx = 1024;
	sizeTotal.cy = 768;
	SetScrollSizes(MM_TEXT, sizeTotal);
	LoadFile();
}

void CVasView::OnDraw(CDC* pDC)
{
	CDocument* pDoc = GetDocument();
	// TODO: add draw code here
	DrawBitmap();
	pDC->BitBlt(0,0,Cx,Cy,&m_MemDC,0,0,SRCCOPY);
}

void CVasView::LoadFile()
{
	CMainFrame*		pFrame;
    CString lpszPathName;
    char szFilter[]="Bmp Files (*.Bmp)|*.Bmp||";
    CFileDialog FileDlg(TRUE,".Bmp",
        "MyStock",OFN_HIDEREADONLY|OFN_CREATEPROMPT|OFN_OVERWRITEPROMPT,
        szFilter);
    if(FileDlg.DoModal()!=IDOK)
        return;

    lpszPathName=FileDlg.GetPathName();
	CFile	file(lpszPathName,CFile::modeRead);
	if(FALSE == m_Dibfile.Read(&file))
		ASSERT(FALSE);
	pFrame=reinterpret_cast<CMainFrame*>(AfxGetMainWnd());
	CString str;
	str.Format("Size: X=%d,Y=%d,BitCount=%d",m_Dibfile.GetDimensions().cx,m_Dibfile.GetDimensions().cy,m_Dibfile.m_lpBMIH->biBitCount);
	pFrame->SetMessageText(str);
	m_rect.right = m_Dibfile.GetDimensions().cx;
	m_rect.bottom = m_Dibfile.GetDimensions().cy;
}

void CVasView::VasBitmap()
{
	long size = m_Dibfile.GetSizeImage();
	while(size>0)
	{
		m_Dibfile.m_lpImage[size-1] = ~ m_Dibfile.m_lpImage[size-1];
		size--;
	}
	Invalidate(FALSE);
}

/////////////////////////////////////////////////////////////////////////////
// CVasView diagnostics

#ifdef _DEBUG
void CVasView::AssertValid() const
{
	CScrollView::AssertValid();
}

void CVasView::Dump(CDumpContext& dc) const
{
	CScrollView::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CVasView message handlers


void CVasView::OnAntiBmp() 
{
	// TODO: Add your command handler code here
	VasBitmap();
}

void CVasView::OnOpenBmp() 
{
	// TODO: Add your command handler code here
	m_rect = CRect(0,0,0,0);
	m_Org = CPoint(0,0);
	m_bDraging = FALSE;
	m_DragTop = m_DragBottom = m_DragRight = m_DragLeft = FALSE;
	m_SizeX = m_SizeY = 0;
	LoadFile();
	Invalidate(FALSE);
}

void CVasView::OnSaveBmp() 
{
    CString lpszPathName;
    char szFilter[]="Bmp Files (*.Bmp)|*.Bmp||";
    CFileDialog FileDlg(FALSE,".Bmp",
        "MyStock",OFN_HIDEREADONLY|OFN_CREATEPROMPT|OFN_OVERWRITEPROMPT,
        szFilter);
    if(FileDlg.DoModal()!=IDOK)
        return;
    lpszPathName=FileDlg.GetPathName();
	CFile outfile;
	try
	{
		outfile.Open(lpszPathName,CFile::modeCreate|CFile::modeWrite);	
	}
	catch(CException* pe) {
		pe->Delete();
		AfxMessageBox("open file error");
		return;
	}
	m_Dibfile.Write(&outfile);

}

void CVasView::OnLButtonDown(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	CRect rect1,rect2,rect3,rect4;
	CDC* pdc = GetDC();
	pdc->DPtoLP(&point,1);
	if(m_rect == CRect(0,0,0,0))
		return;
	if(m_rect.PtInRect(point))
	{
		pdc->MoveTo(m_Org);
		pdc->LineTo(CPoint(m_rect.right,m_Org.y));
		pdc->LineTo(CPoint(m_rect.right,m_rect.bottom));
		pdc->LineTo(CPoint(m_Org.x,m_rect.bottom));
		pdc->LineTo(m_Org);
		pdc->Rectangle((m_rect.right+m_Org.x)/2-4,m_Org.y-4,(m_rect.right+m_Org.x)/2+4,m_Org.y+4);
		pdc->Rectangle(m_rect.right-4,(m_rect.bottom+m_Org.y)/2-4,m_rect.right+4,(m_rect.bottom+m_Org.y)/2+4);
		pdc->Rectangle((m_rect.right+m_Org.x)/2-4,m_rect.bottom-4,(m_rect.right+m_Org.x)/2+4,m_rect.bottom+4);
		pdc->Rectangle(m_Org.x-4,(m_rect.bottom+m_Org.y)/2-4,m_Org.x+4,(m_rect.bottom+m_Org.y)/2+4);
	}
	else
	{
		if(!m_bDraging)
		{
			Invalidate(FALSE);
		}
	}
			rect1= CRect((m_rect.right+m_Org.x)/2-4,m_Org.y-4,(m_rect.right+m_Org.x)/2+4,m_Org.y+4);
			rect2 = CRect(m_rect.right-4,(m_rect.bottom+m_Org.y)/2-4,m_rect.right+4,(m_rect.bottom+m_Org.y)/2+4);
			rect3 = CRect((m_rect.right+m_Org.x)/2-4,m_rect.bottom-4,(m_rect.right+m_Org.x)/2+4,m_rect.bottom+4);
			rect4 = CRect(m_Org.x-4,(m_rect.bottom+m_Org.y)/2-4,m_Org.x+4,(m_rect.bottom+m_Org.y)/2+4);
			if(rect2.PtInRect(point))
			{
				SetCursor(AfxGetApp()->LoadCursor(IDC_DRAG));
				m_DragRight = TRUE;
			}
			else if(rect4.PtInRect(point))
			{
				SetCursor(AfxGetApp()->LoadCursor(IDC_DRAG));
				m_DragLeft = TRUE;
			}
			else if(rect1.PtInRect(point))
			{
				SetCursor(AfxGetApp()->LoadCursor(IDC_DRAG2));
				m_DragTop = TRUE;
			}
			else if(rect3.PtInRect(point))
			{
				SetCursor(AfxGetApp()->LoadCursor(IDC_DRAG2));
				m_DragBottom = TRUE;
			}
			else if(m_rect.PtInRect(point))
			{
				SetCursor(AfxGetApp()->LoadCursor(IDC_DRAGING));
				m_First = point;
				m_bDraging = TRUE;
			}
	
}

void CVasView::OnMouseMove(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default

	CDC* pdc = GetDC();
	pdc->DPtoLP(&point,1);
	if(m_rect == CRect(0,0,0,0))
		return;
	if(m_bDraging)
	{
		SetCursor(AfxGetApp()->LoadCursor(IDC_DRAGING));
		m_rect.OffsetRect(point -m_First);
		m_Org += point -m_First;
		m_First = point;
		m_rect.top = m_Org.y;
		m_rect.left = m_Org.x;
		Invalidate(FALSE);
	m_rect.top = m_Org.y;
	m_rect.left = m_Org.x;
		return;
	}
	if(m_DragTop)
	{
		SetCursor(AfxGetApp()->LoadCursor(IDC_DRAG2));
		if(point.y > m_rect.bottom -10)
			return;
		m_SizeY += m_Org.y - point.y;
		m_Org.y = point.y;
		Invalidate(FALSE);
	m_rect.top = m_Org.y;
	m_rect.left = m_Org.x;
		return;
	}
	if(m_DragBottom)
	{
		SetCursor(AfxGetApp()->LoadCursor(IDC_DRAG2));
		if(point.y < m_Org.y +10)
			return;
		m_SizeY += point.y - m_rect.bottom;
		m_rect.bottom = point.y;
		Invalidate(FALSE);
	m_rect.top = m_Org.y;
	m_rect.left = m_Org.x;
		return;
	}
	if(m_DragLeft)
	{
		SetCursor(AfxGetApp()->LoadCursor(IDC_DRAG));
		if(point.x > m_rect.right - 10)
			return;
		m_SizeX+= m_Org.x - point.x;
		m_Org.x = point.x;
		Invalidate(FALSE);
	m_rect.top = m_Org.y;
	m_rect.left = m_Org.x;
		return;
	}
	if(m_DragRight)
	{
		SetCursor(AfxGetApp()->LoadCursor(IDC_DRAG));
		if(point.x < m_Org.x + 10)
			return;
		m_SizeX += point.x - m_rect.right;
		m_rect.right = point.x;
		Invalidate(FALSE);
	m_rect.top = m_Org.y;
	m_rect.left = m_Org.x;
		return;
	}
			
}

void CVasView::OnLButtonUp(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	if(m_DragTop)
		m_DragTop = FALSE;
	if(m_DragBottom)
		m_DragBottom = FALSE;
	if(m_DragRight)
		m_DragRight = FALSE;
	if(m_DragLeft)
		m_DragLeft = FALSE;
	if(m_bDraging)
		m_bDraging = FALSE;
}


int CVasView::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CScrollView::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	// TODO: Add your specialized creation code here
	CPaintDC dc(this);
	Cx=1024;
	Cy=768;
	m_MemDC.CreateCompatibleDC(&dc);
	m_Bitmap.CreateCompatibleBitmap(&dc,Cx,Cy);
	m_MemDC.SelectObject(&m_Bitmap);
	HBRUSH brush=(HBRUSH)::GetStockObject(WHITE_BRUSH);
	m_MemDC.SelectObject(brush);
	m_MemDC.PatBlt(0,0,Cx,Cy,PATCOPY);
	return 0;
}

void CVasView::DrawBitmap()
{
 	HBRUSH brush=(HBRUSH)::GetStockObject(WHITE_BRUSH);
	m_MemDC.SelectObject(brush);
	m_MemDC.PatBlt(0,0,Cx,Cy,PATCOPY);
	CSize sizeFileDib = m_Dibfile.GetDimensions();
    sizeFileDib.cx += m_SizeX;
    sizeFileDib.cy += m_SizeY;
	m_Dibfile.Draw(&m_MemDC, m_Org, sizeFileDib);
}
