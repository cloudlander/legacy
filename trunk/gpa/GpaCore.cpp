// GpaCore.cpp: implementation of the CGpaCore class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "GPA2.h"
#include "GpaCore.h"
#include "GpaDlg.h"
#include <mshtml.h>

#include "Utility.h"
#pragma comment(lib,"wininet")

#define IENH_EXT_EXPORTS
#include "../bin/ext/IENH_EXT.H"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

char GenPath[500];
CGpaCore g_gc;

unsigned __stdcall DoQueryGPA(void* arg);

BOOL Create(HWND hwnd,HWND hwndWB,void* p,int Sess)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
		
		HKEY key;
		DWORD disp;
		DWORD type = REG_SZ;
		TCHAR *szVal = _T("Module");
		TCHAR szBuf[260];
		DWORD size = 260;
		if(ERROR_SUCCESS == RegCreateKeyEx(HKEY_LOCAL_MACHINE,_T("Software\\iEnhance"),
									0,NULL,REG_OPTION_NON_VOLATILE,KEY_QUERY_VALUE,
									NULL,&key,&disp))
		{
			if(REG_CREATED_NEW_KEY == disp)		// 首次创建,不该发生!
			{
				AfxMessageBox("iEnhance程序已损坏，请重新安装");
				RegCloseKey(key);
				return FALSE;
			}

			if(ERROR_SUCCESS == RegQueryValueEx(key,szVal,NULL,&type,(BYTE*)szBuf,&size))
			{
				int i = strlen(szBuf);
				if(0 == i)
				{
				AfxMessageBox("iEnhance程序已损坏，请重新安装");
				RegCloseKey(key);
				return FALSE;
				}
				if('\\' != szBuf[i-1])
				{
					szBuf[i] = '\\';
					szBuf[i+1] = '\0';
				}
				strcpy(GenPath,szBuf);
			}
			else
			{
				AfxMessageBox("iEnhance程序已损坏，请重新安装");
				RegCloseKey(key);
				return FALSE;
			}
			RegCloseKey(key);
		}
		strcat(GenPath,"gpa.html");

	xCenter =::GetSystemMetrics(SM_CXSCREEN)/2;
	yCenter =::GetSystemMetrics(SM_CYSCREEN)/2;

	IWebBrowser2* pWeb = reinterpret_cast<IWebBrowser2*>(p);
	g_gc.m_hHost = hwnd;
	g_gc.m_hWebBrowser = hwndWB;
	g_gc.m_pFrameWB = pWeb;
	g_gc.m_Sess=Sess;
//	CGpaDlg* pDlg = new CGpaDlg(hwnd,hwndWB,pWeb);
	CGpaDlg dlg;
	if(0 == dlg.DoModal())
//	pDlg->DoModal();
	{	
		CTipWnd* pWnd = new CTipWnd;
		g_gc.m_TipWnd = pWnd;
	RECT rc;
	rc.left = 400;
	rc.top = 200;
	rc.right = 700;
	rc.bottom = 300;
	pWnd->CreateEx(0,NULL,"Wait",
				WS_BORDER|WS_POPUPWINDOW|WS_OVERLAPPED|WS_VISIBLE|WS_CHILD,0,0,0,0, 
				g_gc.m_hHost,NULL);
	pWnd->MoveWindow(xCenter-150,yCenter-50,300,100);
	::SetParent(pWnd->m_hWnd,g_gc.m_hWebBrowser);
		unsigned threadid;
		_beginthreadex(NULL,0,&DoQueryGPA,(void*)&g_gc,0,&threadid);
	}

	return FALSE;
}

unsigned __stdcall DoQueryGPA(void* arg)
{
	CoInitialize(NULL);
	
//	CTipWnd m_TipWnd;

	BOOL bFailed = FALSE;
	CGpaCore* pThis = (CGpaCore*)arg;
	HINTERNET hSession = InternetOpen("IE++",INTERNET_OPEN_TYPE_PRECONFIG,NULL,NULL,0);
	TCHAR buf[100];
	TCHAR prm[500];
	TCHAR header[500]= "Content-type: application/x-www-form-urlencoded\r\nContent-length:";
	TCHAR cgi[500] = _T("/good/owa/frame_identify");
	prm[0] = _T('\0');
	_stprintf(prm,_T("username=%s&pwd=%s&new_pwd=&con_new_pwd="),_T(pThis->m_StudentNo.c_str()),_T(pThis->m_Passwd.c_str()));
	_stprintf(buf,_T("%d\r\n\r\n"),_tcslen(prm));
	_tcscat(header,buf);
	pThis->m_TipWnd->SetStaticText("正在发送学号和密码信息");
//	m_TipWnd.SetStaticText("正在发送学号和密码信息");
	if(FALSE == pThis->PostData(hSession,cgi,header,prm))
	{
		bFailed = TRUE;
//		pThis->m_TipWnd.SetStaticText("发送失败");
	}
	else
	{
		MSHTML::IHTMLElementPtr pElement;
		pThis->m_TipWnd->SetStaticText("检查绩点的超链接");
//		m_TipWnd.SetStaticText("检查绩点的超链接");
		if(!pThis->GetScoreHref())
		{
			bFailed = TRUE;
//			pThis->m_TipWnd.SetStaticText("用户名，密码不正确，或者绩点链接已改变");
		}
		else
		{
			prm[0] = _T('\0');
			int i = pThis->m_szBuffer.find("/good/owa",0);
			tstring strBuf;
			strBuf = pThis->m_szBuffer.substr(i,pThis->m_szBuffer.size());
			i = strBuf.find("?",0);
			_tcscpy(cgi,strBuf.substr(0,i).c_str());
			_tcscpy(prm,strBuf.substr(i+1,strBuf.size()).c_str());
			_tcscpy(header,"Content-type: application/x-www-form-urlencoded\r\nContent-length:");
			_stprintf(buf,_T("%d\r\n\r\n"),_tcslen(prm));
			_tcscat(header,buf);
			pThis->m_TipWnd->SetStaticText("正在下载绩点数据");
//			m_TipWnd.SetStaticText("正在下载绩点数据");
			if(!pThis->PostData(hSession,cgi,header,prm))
				bFailed = TRUE;
			else
			{
				pThis->m_TipWnd->SetStaticText("正在生成统计页面");
//				m_TipWnd.SetStaticText("正在生成统计页面");
				pThis->ParseTable();
			}
		}
	}
	pThis->m_TipWnd->SetStaticText("任务成功完成");
//	m_TipWnd.SetStaticText("任务成功完成");
	InternetCloseHandle(hSession);
	CoUninitialize();
	pThis->QueryDone(bFailed);
	if(bFailed)
		return 1;
	else
		return 0;
}

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////


CGpaCore::~CGpaCore()
{

}

BOOL CGpaCore::PostData(HINTERNET hSess,TCHAR* cgi,TCHAR* header,TCHAR* param)
{
	TCHAR buf[32];
	m_szBuffer.erase(0,m_szBuffer.size());
	HINTERNET hConnect = InternetConnect(hSess,"202.120.224.22",6666,NULL,NULL,INTERNET_SERVICE_HTTP,0,NULL);
	HINTERNET hRequest = HttpOpenRequest(hConnect,"POST",cgi,NULL,NULL,(const char**)"*/*\0",0,NULL);
	if(NULL == hRequest)
	{
		ReportErrMsg();
		return FALSE;
	}
	if(HttpSendRequest(hRequest,header,_tcslen(header),(VOID*)param,_tcslen(param)))
    {
		DWORD buflen = 32 /* sizeof(buf)*/;
		BOOL isLength = HttpQueryInfo(hRequest,HTTP_QUERY_CONTENT_LENGTH,buf,&buflen,NULL);

		if(isLength) {
			DWORD filesize = atol(buf);
			char *content = new char[filesize+1];
			DWORD bytesread;

			BOOL bRead = InternetReadFile(hRequest,content,filesize,&bytesread);

			m_szBuffer =  content;
			delete []content;
		}
		else {
			char content[5000];
			DWORD bytesread;
			BOOL bRead;

			do {
				bRead = InternetReadFile(hRequest,content,5000,&bytesread);
				if(bytesread>0) {
					content[bytesread] = '\0';
					tstring tmp =  content;
					m_szBuffer += tmp;
				}
			} while(bRead&&bytesread>0);
		}
		InternetCloseHandle(hRequest);
		InternetCloseHandle(hConnect);
		return TRUE;
	}
	else 
	{
		InternetCloseHandle(hConnect);
		return FALSE;
	}
}

void CGpaCore::WriteDoc(MSHTML::IHTMLDocument3Ptr& pDoc3)
{
		MSHTML::IHTMLDocument2Ptr pDoc;

		HRESULT hr = CoCreateInstance(CLSID_HTMLDocument, NULL, CLSCTX_INPROC_SERVER, 
			IID_IHTMLDocument2, (void**)&pDoc);
		
		//put the code into SAFEARRAY and write it into document
		SAFEARRAY* psa = SafeArrayCreateVector(VT_VARIANT, 0, 1);
		VARIANT *param;
		bstr_t bsData((LPCTSTR)m_szBuffer.c_str());
		hr = SafeArrayAccessData(psa, (LPVOID*)&param);
		param->vt = VT_BSTR;
		param->bstrVal = (BSTR)bsData;
		BSTR str = SysAllocString(OLESTR("On"));
		hr = pDoc->put_designMode(str);
		hr = pDoc->write(psa);
		hr = pDoc->close();
		
		SafeArrayDestroy(psa);

		//I'll use IHTMLDocument3 to retrieve tags. Note it is available only in IE5+
		//If you don't want to use it, u can just run through all tags in HTML
		//(IHTMLDocument2->all property)
		pDoc3 = pDoc;
}

BOOL CGpaCore::GetScoreHref()
{
	MSHTML::IHTMLElementPtr pElement;
	MSHTML::IHTMLElementCollectionPtr pCollection;
	MSHTML::IHTMLDocument3Ptr pDoc3;
	WriteDoc(pDoc3);
	//display HREF parameter of every link (A tag) in ListBox
	//  此处可能有问题
//	MSHTML::IHTMLDocument2Ptr pDoc2;
//	IDispatch* pDisp;
//	pDoc2 = pDoc3;
	pCollection = pDoc3->getElementsByTagName(L"A");
//	pDoc2->get_all(&pCollection);
	for(long i=0; i<pCollection->length; i++){
//        _variant_t vtItem((long)i), vtEmpty;
		pElement = pCollection->item(i, (long)0);
//		pDisp = pCollection->item(vtItem, vtEmpty);
//		pDisp->QueryInterface(IID_IHTMLElement, (LPVOID*)&pElement);
//      pDisp->Release();
//		BSTR bstr;
//		pElement->get_tagName(&bstr);
//        _bstr_t bstrTagName(bstr);
//		if (!lstrcmpi(bstrTagName, _T("A")))
//		{
		if(pElement != NULL){
			//second parameter says that you want to get text inside attribute as is
			// 以后可能会改动判断条件，如果页面被重写的话
			if(0 == lstrcmp((LPCTSTR)bstr_t(pElement->innerText),"查询成绩"))
			{
				 m_szBuffer = (LPTSTR)bstr_t(pElement->getAttribute("href",2));
//				::MessageBox(m_hWnd,(LPCTSTR)m_szBuffer.c_str(),"Msg",MB_OK);
				return TRUE;
			}
		}
//		}
	}	
	return FALSE;
}

void CGpaCore::ParseTable()
{
	fstream outfile(GenPath,ios::out);
	GenTblHead(outfile);
	MSHTML::IHTMLElementPtr pElement;
	MSHTML::IHTMLElementCollectionPtr pCollection;
	MSHTML::IHTMLDocument3Ptr pDoc3;
	WriteDoc(pDoc3);
	pCollection = pDoc3->getElementsByTagName(L"TD");
	int j = 0;
	tstring buf;
	for(long i=1; i<pCollection->length; i++,j++){
		pElement = pCollection->item(i, (long)0);
		if(pElement != NULL){
			//second parameter says that you want to get text inside attribute as is
//			::MessageBox(NULL,(LPCTSTR)bstr_t(pElement->innerText),"Msg",MB_OK|MB_ICONINFORMATION);
			if(j<5)
			{
				buf += (LPTSTR)bstr_t(pElement->innerText);
				buf += "^";
			}
			else
			{
				GenRow(buf.substr(0,buf.size()-1),outfile);
				buf = (LPCTSTR)bstr_t(pElement->innerText);
				buf += "^";
				j = 0;
			}
		}
	}
	if(5==j)
		GenRow(buf.substr(0,buf.size()-1),outfile);
	GenTblTail(outfile);
	outfile.close();
}

void CGpaCore::GenTblHead(fstream& of)
{
	tstring str = "<HTML>\n\
<body background=>\n\
<head>\n\
<script language=\"JavaScript\" src=\"scripts/gpa.js\" type=\"text/JavaScript\">\n\
</script>\n\
<meta http-equiv=\"Content-Type\" content=\"text/html; charset=gb2312\"></head>\n\
<body background=>\n\
  <p>当前使用者:<font color=#0000ff>";
	str += m_StudentNo;
	str += "</font></p>\n\
<div align=\"center\">\n\
  <p>\n\
    <input name=\"disp\" type=\"text\" size=\"12\" maxlength=\"8\" readonly=\"true\">\n\
    &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp; \n\
    <input name=\"btn\" type=\"button\" onClick=\"ComputeGPA()\" value=\"计算绩点\">\n\
  </p>\n\
<TABLE  border=1 id = scoreTbl>\n\
<CAPTION>成绩单</CAPTION>\n\
<TD ROWSPAN=\"1\" NOWRAP>&nbsp;</TD>\n\
<TH ALIGN=\"center\" ROWSPAN=\"1\" COLSPAN=\"1\">课程编码</TH>\n\
<TH ALIGN=\"center\" ROWSPAN=\"1\" COLSPAN=\"1\">课程名称</TH>\n\
<TH ALIGN=\"center\" ROWSPAN=\"1\" COLSPAN=\"1\">学分数目</TH>\n\
<TH ALIGN=\"center\" ROWSPAN=\"1\" COLSPAN=\"1\">成绩</TH>\n\
<TH ALIGN=\"center\" ROWSPAN=\"1\" COLSPAN=\"1\">绩点</TH>\n\
<TH ALIGN=\"center\" ROWSPAN=\"1\" COLSPAN=\"1\">学期</TH>\n";
	of.write(str.c_str(),str.size());
}

void CGpaCore::GenRow(tstring& szData,fstream& of)
{
	tstring grade;
	tstring buf;
	int i;
	buf = "<TR>\n\
<TH><input  type=\"checkbox\" value=\"\" checked></TH>\n";
	of.write(buf.c_str(),buf.size());
	for(int j=0;j<4;j++)
	{
		i = szData.find("^",0);
		buf = "<td><font color=#0000FF>";
		buf += szData.substr(0,i);
		if(3 == j)
			grade = szData.substr(0,i);
		buf += "</font></td>\n";
		of.write(buf.c_str(),buf.size());
		szData = szData.substr(i+1,szData.size());
	}
	MGRADE::iterator mi;
	buf = "<td><font color=#0000FF>";
	mi = m_MapOfGrade.find(grade);
	if(mi != m_MapOfGrade.end())
		buf += mi->second;
	else
		buf += "0.0";
	buf += "</font></td>\n<td><font color=#0000FF>";
	i = szData.find("^",0);
	buf += szData.substr(0,i);
	buf += "</font></td>\n";
	of.write(buf.c_str(),buf.size());
	of.write("</TR>\n",6);
}

void CGpaCore::GenTblTail(fstream& of)
{
	tstring buf;
	buf = "</TABLE>\n\
<script language=\"JavaScript\" type=\"text/JavaScript\">\n\
ComputeGPA();\n\
</script>\n\
</div>\n\
</BODY>\n\
</HTML>\n";
	of.write(buf.c_str(),buf.size());
}

void CGpaCore::QueryDone(int res)
{
	variant_t vtEmtpy;
	if(res == 0)
		m_pFrameWB->Navigate(bstr_t(GenPath),&vtEmtpy,&vtEmtpy,&vtEmtpy,&vtEmtpy);
	else
		m_pFrameWB->put_StatusText(bstr_t("wrongly"));
	g_gc.m_TipWnd->SendMessage(WM_CLOSE,0,0);
	::SendMessage(g_gc.m_hHost,WM_IENHEXT_CLOSESESSION,g_gc.m_Sess,0);
}
