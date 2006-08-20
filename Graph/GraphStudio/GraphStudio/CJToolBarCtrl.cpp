// CJToolBarCtrl.cpp : implementation file
//
// The majority of this source was copied from MFC, Visual C++ 6, to 
// extend functionality to Visual C++ 5 users, with some ideas taken
// from Luis Barreira's article 'Classes for new IE4 controls'
// http://www.codeguru.com/controls/ie4_controls_classes.shtml
//
// Copyright © 1998-1999 Kirk W. Stowell
//		mailto:kstowell@codejockeys.com
//		http://www.codejockeys.com/kstowell/
//
// This source code may be used in compiled form in any way you desire.
// Source file(s) may be redistributed unmodified by any means PROVIDING 
// they ARE NOT sold for profit without the authors expressed written 
// consent, and providing that this notice and the authors name and all 
// copyright notices remain intact. This software is by no means to be 
// included as part of any third party components library, or as part any
// development solution that offers MFC extensions that are sold for profit. 
// 
// If the source code is used in any commercial applications then a statement 
// along the lines of:
// 
// "Portions Copyright © 1998-99 Kirk Stowell" must be included in the startup 
// banner, "About" box or printed documentation. This software is provided 
// "as is" without express or implied warranty. Use it at your own risk! The 
// author(s) accepts no liability for any damage/loss of business that this 
// product may cause.
//
/////////////////////////////////////////////////////////////////////////////
/****************************************************************************
 *
 * $Date: 7/25/99 10:00p $
 * $Revision: 3 $
 * $Archive: /CodeJockey/CJLibrary/CJToolBarCtrl.cpp $
 *
 * $History: CJToolBarCtrl.cpp $
 * 
 * *****************  Version 3  *****************
 * User: Kirk Stowell Date: 7/25/99    Time: 10:00p
 * Updated in $/CodeJockey/CJLibrary
 * 
 * *****************  Version 2  *****************
 * User: Kirk Stowell Date: 7/25/99    Time: 12:30a
 * Updated in $/CodeJockey/CJLibrary
 * 
 * *****************  Version 1  *****************
 * User: Kirk Stowell Date: 7/14/99    Time: 10:29p
 * Created in $/CodeJockey/CJLibrary
 * Copied from MFC v6 and techno preview for v5. Added to extend
 * functionality to Visual C++ 5.0 users.
 *
 ***************************************************************************/

#include "stdafx.h"
#include "CJToolBarCtrl.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CCJToolBarCtrl

CCJToolBarCtrl::~CCJToolBarCtrl()
{
  DestroyWindow();
}


BEGIN_MESSAGE_MAP(CCJToolBarCtrl, CToolBarCtrl)
  //{{AFX_MSG_MAP(CCJToolBarCtrl)
  ON_WM_CREATE()
  //}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCJToolBarCtrl message handlers

CCJToolBarCtrl::CCJToolBarCtrl()
{
  // initialize common controls
  INITCOMMONCONTROLSEX icex;
  icex.dwSize = sizeof(icex);
  icex.dwICC = ICC_BAR_CLASSES;
  VERIFY(InitCommonControlsEx(&icex));
}

BOOL CCJToolBarCtrl::Create(DWORD dwStyle, const RECT& rect, CWnd* pParentWnd,
                            UINT nID)
{
  CWnd* pWnd = this;
  BOOL bCreated = pWnd->Create(TOOLBARCLASSNAME, NULL, dwStyle, rect, pParentWnd, nID);

  return bCreated;
}

int CCJToolBarCtrl::OnCreate(LPCREATESTRUCT lpcs)
{
  if (CWnd::OnCreate(lpcs) == -1)
    return -1;
  SetButtonStructSize(sizeof(TBBUTTON));
  return 0;
}

HRESULT CCJToolBarCtrl::GetDropTarget(IDropTarget** ppDropTarget) const
{
  ASSERT(::IsWindow(m_hWnd));
  ASSERT(ppDropTarget);
  return (HRESULT) ::SendMessage(m_hWnd, TB_GETOBJECT, (WPARAM)&IID_IDropTarget, (LPARAM)ppDropTarget);
}
