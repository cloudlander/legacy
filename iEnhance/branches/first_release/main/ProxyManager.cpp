// ProxyManager.cpp: implementation of the CProxyManager class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include <wininet.h>

#define MAX_PROXY_LENGTH	200

#pragma comment(lib,"wininet")

#include "ProxyManager.h"
#include "Utility.h"

/*
	Old implementation is not as good as using WININET API
	Disbled original code
*/
#define USE_WININET

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CProxyManager::CProxyManager()
{
	m_bChanged = m_bReqFresh = FALSE;
	m_bSetAll = FALSE;

}

CProxyManager::~CProxyManager()
{
	if(m_ProxyVec.size()>0)		// 应该不会执行到，仅为debug
		m_ProxyVec.empty();
}

BOOL CProxyManager::EnableProxy(BOOL bEnable/* =TRUE */)
{
#ifndef USE_WININET
	DWORD disp;
	HKEY key;
	DWORD val = 0;
	if(ERROR_SUCCESS != RegCreateKeyEx(HKEY_CURRENT_USER,GD_SYSTEM_PROXY_REG,
									0,NULL,REG_OPTION_NON_VOLATILE,KEY_SET_VALUE,
									NULL,&key,&disp))
		return FALSE;
	if(bEnable)
	{
		val = 1;
	}
	if(ERROR_SUCCESS != RegSetValueEx(key,_T("ProxyEnable"),0,REG_DWORD,(BYTE*)&val,sizeof(DWORD)))
	{
		RegCloseKey(key);
		return FALSE;
	}
	RegFlushKey(key);
	RegCloseKey(key);
	BOOL x= ( InternetSetOption(NULL,INTERNET_OPTION_SETTINGS_CHANGED,NULL,0) && 
				InternetSetOption(NULL,INTERNET_OPTION_REFRESH,NULL,0));
	return x;
#else
	TCHAR active_if[MAX_TEXT_LENGTH];
	if(ERROR_SUCCESS!=GetActiveRasInterface(active_if,MAX_TEXT_LENGTH))
	{
		ReportErrMsg("In CProxyManager::EnableProxy");
		return FALSE;
	}

	INTERNET_PER_CONN_OPTION_LIST    List;
	INTERNET_PER_CONN_OPTION         Option[1];
	unsigned long                    nSize = sizeof(INTERNET_PER_CONN_OPTION_LIST);

	Option[0].dwOption = INTERNET_PER_CONN_FLAGS;
	Option[0].Value.dwValue = (bEnable ? PROXY_TYPE_PROXY : PROXY_TYPE_DIRECT);

	List.dwSize = sizeof(INTERNET_PER_CONN_OPTION_LIST);
	List.pszConnection = (active_if[0] ? active_if : NULL);
	List.dwOptionCount = 1;
	List.dwOptionError = 0;
	List.pOptions = Option;

	if(!InternetSetOption(NULL, INTERNET_OPTION_PER_CONNECTION_OPTION, &List, nSize))
	{
		ReportErrMsg("In CProxyManager::EnableProxy");
		return FALSE;
	}
	return InternetSetOption(NULL,INTERNET_OPTION_REFRESH,NULL,0);

#endif
}

// 返回对应字段后的第一个字符位置
// 支持的Entry格式为：
//		x.x.x.x:xxxxx@XXXXX (....
int CProxyManager::GetAddrIndex(TCHAR* sz)
{
	if(*sz)
	{
		for(int i=0;sz[i] && i<_tcslen(sz);i++)
		{
			if(sz[i] == _T(':') || sz[i] == _T(' ') || sz[i] == _T('\t') || sz[i] == _T('\n'))
				return i;
		}
//		return -1;
	}
	return -1;
}

int CProxyManager::GetPortIndex(TCHAR* sz)
{
	if(*sz)
	{
		for(int i=0;sz[i] && i<_tcslen(sz);i++)
		{
			if(sz[i] == _T('@')|| sz[i] == _T(' ') || sz[i] == _T('\t') || sz[i] == _T('\n'))
				return i;
		}
//		return -1;
	}
	return -1;
}

int CProxyManager::GetTypeIndex(TCHAR* sz)
{
	if(*sz)
	{
		for(int i=0;sz[i] && i<_tcslen(sz);i++)
		{
			if(sz[i] == _T('(')|| sz[i] == _T(' ') || sz[i] == _T('\t') || sz[i] == _T('\n'))
				return i;
		}
//		return -1;
	}
	return -1;
}

BOOL CProxyManager::GetProxySettingFromIE()
{
	TCHAR szBuf[250];
	szBuf[0]='\0';
//  导入IE代理设置
#ifndef USE_WININET
	HKEY key;
	DWORD disp;
	DWORD type =REG_SZ;
	UINT size;
	key=NULL;
	if(ERROR_SUCCESS != RegCreateKeyEx(HKEY_CURRENT_USER,GD_SYSTEM_PROXY_REG,
									0,NULL,REG_OPTION_NON_VOLATILE,KEY_QUERY_VALUE,
									NULL,&key,&disp))
	{
		return FALSE;
	}
	size = 250;
	if(ERROR_SUCCESS != RegQueryValueEx(key,"ProxyServer",NULL,&type,(BYTE*)szBuf,&size))
	{
		RegCloseKey(key);
		return TRUE;
	}
	RegCloseKey(key);	
	
#else
	TCHAR active_if[MAX_TEXT_LENGTH];
	if(ERROR_SUCCESS!=GetActiveRasInterface(active_if,MAX_TEXT_LENGTH))
	{
		ReportErrMsg("In CProxyManager::GetProxySettingFromIE");
		return FALSE;
	}

	INTERNET_PER_CONN_OPTION_LIST    List;
	INTERNET_PER_CONN_OPTION         Option[1];
	unsigned long                    nSize = sizeof(INTERNET_PER_CONN_OPTION_LIST);

	Option[0].dwOption = INTERNET_PER_CONN_PROXY_SERVER;

	List.dwSize = sizeof(INTERNET_PER_CONN_OPTION_LIST);
	List.pszConnection = (active_if[0] ? active_if : NULL);
	List.dwOptionCount = 1;
	List.dwOptionError = 0;
	List.pOptions = Option;

	if(!InternetQueryOption(NULL, INTERNET_OPTION_PER_CONNECTION_OPTION, &List, &nSize))
	{
		ReportErrMsg("In CProxyManager::GetProxySettingFromIE");
		return FALSE;
	}
	if(Option[0].Value.pszValue)
		_tcscpy(szBuf,Option[0].Value.pszValue);
	else
		return TRUE;
 
#endif
	tstring str;
	tstring temp;
	tstring temp2;
	int i=-1,j;
	str = szBuf;
	i = str.find(_T(";"),0);
	if(i<0 )//&& -1 ==  str.find(_T(":"),0))		// 所有协议使用相同的代理
	{
		m_bSetAll = TRUE;
		i = str.find(_T(":"),0);
		if(i<0)
		{
			m_curHttpProxy.name = str;
			m_curHttpProxy.port = _T("80");

			m_curHttpsProxy.name = str;
			m_curHttpsProxy.port = _T("80");
			
			m_curFtpProxy.name = str;
			m_curFtpProxy.port = _T("80");
			
			m_curSocksProxy.name = str;
			m_curSocksProxy.port = _T("80");
			
			m_curGopherProxy.name = str;
			m_curGopherProxy.port = _T("80");
		}
		else
		{
			temp = str.substr(0,i);
			temp2 = str.substr(i+1,str.size());
			m_curHttpProxy.name = temp;
			m_curHttpProxy.port = temp2;

			m_curHttpsProxy.name = temp;
			m_curHttpsProxy.port = temp2;

			m_curFtpProxy.name = temp;
			m_curFtpProxy.port = temp2;

			m_curSocksProxy.name = temp;
			m_curSocksProxy.port = temp2;

			m_curGopherProxy.name = temp;
			m_curGopherProxy.port = temp2;
		}
	}
	else
	{
		while(1)
		{
			if(i>=0)
				temp2 = str.substr(0,i);
			else
				temp2 = str;

			temp = temp2.substr(0,temp2.find(_T("="),0));
			
			if(0 == temp.compare(0,5,_T("https")))		// 注意此处stl中i=-1时可能造成错误!
			{
				j = temp2.find(_T(":"),0);
				if(j>0)
					m_curHttpsProxy.port = temp2.substr(j+1,temp2.size());
				else
					m_curHttpsProxy.port = _T("80");
				temp2 = temp2.substr(temp2.find(_T("="),0)+1,temp2.size());
				m_curHttpsProxy.name = temp2.substr(0,temp2.find(_T(":"),0));
			}
			else if(0 == temp.compare(0,4,_T("http")))
			{
				j = temp2.find(_T(":"),0);
				if(j>0)
					m_curHttpProxy.port = temp2.substr(j+1,temp2.size());
				else
					m_curHttpProxy.port = _T("80");
				temp2 = temp2.substr(temp2.find(_T("="),0)+1,temp2.size());
				m_curHttpProxy.name = temp2.substr(0,temp2.find(_T(":"),0));
			}
			else if(0 == temp.compare(0,6,_T("gopher")))
			{
				j = temp2.find(_T(":"),0);
				if(j>0)
					m_curGopherProxy.port = temp2.substr(j+1,temp2.size());
				else
					m_curGopherProxy.port = _T("80");
				temp2 = temp2.substr(temp2.find(_T("="),0)+1,temp2.size());
				m_curGopherProxy.name = temp2.substr(0,temp2.find(_T(":"),0));
			}
			else if(0 == temp.compare(0,3,_T("ftp")))
			{
				j = temp2.find(_T(":"),0);
				if(j>0)
					m_curFtpProxy.port = temp2.substr(j+1,temp2.size());
				else
					m_curFtpProxy.port = _T("80");
				temp2 = temp2.substr(temp2.find(_T("="),0)+1,temp2.size());
				m_curFtpProxy.name = temp2.substr(0,temp2.find(_T(":"),0));
			}
			else if(0 == temp.compare(0,5,_T("socks")))
			{
				j = temp2.find(_T(":"),0);
				if(j>0)
					m_curSocksProxy.port = temp2.substr(j+1,temp2.size());
				else
					m_curSocksProxy.port = _T("80");
				temp2 = temp2.substr(temp2.find(_T("="),0)+1,temp2.size());
				m_curSocksProxy.name = temp2.substr(0,temp2.find(_T(":"),0));
			}
			
			if(i<0)
				break;

			str = str.substr(i+1,str.size());

			i = str.find(_T(";"),0);
		}
	}
	return TRUE;
}

//****************
//		注意注册表需要的是REG_SZ类型，所以此程序用UNICODE编译时回有问题，流待以后解决
BOOL CProxyManager::Init()
{
	HKEY key;
	DWORD disp;
	TCHAR szNum[10];
	TCHAR szBuf[250];
	tstring str;
	tstring temp;
	tstring temp2;
	TCHAR szVal[10] = _T("Entry");
	int i=1;
	int Addrindex,Portindex,Typeindex;
	m_bReqFresh = TRUE;
	DWORD type =REG_SZ;
	unsigned long size = 250;
	
	if(m_ProxyVec.size() > 0)
	{
		m_ProxyVec.clear();
	}

	if(ERROR_SUCCESS != RegCreateKeyEx(HKEY_CURRENT_USER,GD_PROXY_REG,
									0,NULL,REG_OPTION_NON_VOLATILE,KEY_QUERY_VALUE,
									NULL,&key,&disp))
		return FALSE;
	if(REG_CREATED_NEW_KEY == disp)		// 首次创建
	{
		RegCloseKey(key);
		return TRUE;
	}
	while(TRUE)
	{
		szVal[5] = _T('\0');
		_itot(i,szNum,10);
		_tcscat(szVal,szNum);
		size = 250;
		if(ERROR_SUCCESS == RegQueryValueEx(key,szVal,NULL,&type,(BYTE*)szBuf,&size))
		{
			if((*szBuf))
			{	
				Addrindex = GetAddrIndex(szBuf);
				szBuf[Addrindex] = _T('\0');
				Portindex = GetPortIndex(szBuf+Addrindex+1);
				(szBuf+Addrindex+1)[Portindex] = _T('\0');
				Typeindex = GetTypeIndex(szBuf+Addrindex+Portindex+2);
				(szBuf+Addrindex+Portindex+2)[Typeindex] = _T('\0');
				if((szBuf+Addrindex+Portindex+Typeindex+3)[0] == '&')
				{
					(szBuf+Addrindex+Portindex+Typeindex+3)[21] = _T('\0');			// 仅存放20字节的comment
					m_ProxyVec.push_back(ProxyEntry(szBuf,szBuf+Addrindex+1,
						szBuf+Addrindex+Portindex+2,szBuf+Addrindex+Portindex+Typeindex+4));
				}
				else
					m_ProxyVec.push_back(ProxyEntry(szBuf,szBuf+Addrindex+1,
						szBuf+Addrindex+Portindex+2));
			}
		}
		else
			break;
		i++;
	}
	RegCloseKey(key);
	return TRUE;
}

BOOL CProxyManager::Store()
{
	int i = 1;
	HKEY key;
	DWORD disp;
	TCHAR szNum[10];
	TCHAR szBuf[150];
	TCHAR szVal[30] = _T("Entry");
	int size = m_ProxyVec.size();
	RegDeleteKey(HKEY_CURRENT_USER,GD_PROXY_REG);
	if(ERROR_SUCCESS != RegCreateKeyEx(HKEY_CURRENT_USER,GD_PROXY_REG,
									0,NULL,REG_OPTION_NON_VOLATILE,KEY_SET_VALUE,
									NULL,&key,&disp))
		return FALSE;
	while(i-1 <  size)
	{
		szVal[5] = _T('\0');
		_itot(i,szNum,10);
		_tcscat(szVal,szNum);
		_tcscpy(szBuf,m_ProxyVec[i-1].name.c_str());
		_tcscat(szBuf,_T(":"));
		_tcscat(szBuf,m_ProxyVec[i-1].port.c_str());
		_tcscat(szBuf,_T("@"));
		_tcscat(szBuf,m_ProxyVec[i-1].type.c_str());
		if(m_ProxyVec[i-1].comment.size() > 0)
		{
			_tcscat(szBuf,_T(" &"));							// 保存comment
			_tcscat(szBuf,m_ProxyVec[i-1].comment.c_str());
		}
		RegSetValueEx(key,szVal,NULL,REG_SZ,(BYTE*)szBuf,_tcslen(szBuf));
		szBuf[0] = _T('\0');
		i++;
	}
	RegFlushKey(key);
	RegCloseKey(key);
	return TRUE;
}

BOOL CProxyManager::SetProxy(const TCHAR* addr,const TCHAR* port,const TCHAR* type)
{
//	TCHAR szBuf[200];
//	TCHAR szBuf2[200];
//	TCHAR* pch = NULL,*pch2 = szBuf;
	DWORD regtype =REG_SZ;
//	unsigned long size = sizeof(szBuf);
	int i = 0;
	BOOL bFound = FALSE;
//	szBuf[0] = _T('\0');
//	szBuf2[0] = _T('\0');
	tstring buffer;
	tstring temp = type;
	tstring temp2;
	if(m_bSetAll)
	{
		if(NULL != addr && NULL != port)
		{
			buffer = tstring(addr) + tstring(_T(":")) + tstring(port);
			temp2 = addr;
			temp = port;
		}
		else
		{
			buffer = _T("");
			temp2 = _T("");
			temp = _T("");
		}
		m_curHttpProxy.name = temp2;
		m_curHttpProxy.port = temp;

		m_curHttpsProxy.name = temp2;
		m_curHttpsProxy.port = temp;
			
		m_curFtpProxy.name = temp2;
		m_curFtpProxy.port = temp;
			
		m_curSocksProxy.name = temp2;
		m_curSocksProxy.port = temp;
			
		m_curGopherProxy.name = temp2;
		m_curGopherProxy.port = temp;
	}
	else if(0 == temp.compare(_T("https")))
	{
		if(NULL == addr)
			m_curHttpsProxy.name = _T("");
		else
			m_curHttpsProxy.name = tstring(addr);
		if(NULL == port)
			m_curHttpsProxy.port = _T("");
		else
			m_curHttpsProxy.port = tstring(port);
			
			if(m_curFtpProxy.name.size() > 0)
				buffer = tstring(_T("ftp=")) + m_curFtpProxy.name +
					tstring(_T(":")) + m_curFtpProxy.port;
			if(m_curGopherProxy.name.size() > 0)
			{
				if(buffer.size() > 0)
					buffer +=tstring(_T(";"));
				buffer +=tstring(_T("gopher=")) + m_curGopherProxy.name +
					tstring(_T(":")) + m_curGopherProxy.port;
			}
			if(m_curHttpProxy.name.size() > 0)
			{
				if(buffer.size() > 0)
					buffer +=tstring(_T(";"));
				buffer +=tstring(_T("http=")) + m_curHttpProxy.name +
					tstring(_T(":")) + m_curHttpProxy.port;
			}
			if(NULL != addr)
			{
				if(buffer.size() > 0)
					buffer +=tstring(_T(";"));
				buffer +=tstring(_T("https=")) + tstring(addr) + tstring(_T(":")) +
					tstring(port);
			}
			if(m_curSocksProxy.name.size() > 0)
			{
				if(buffer.size() > 0)
					buffer +=tstring(_T(";"));
				buffer +=tstring(_T("socks=")) + m_curSocksProxy.name +
					tstring(_T(":")) + m_curSocksProxy.port;
			}
	}
	else if(0 == temp.compare(_T("http")))
	{
		if(NULL == addr)
			m_curHttpProxy.name = _T("");
		else
			m_curHttpProxy.name = tstring(addr);
		if(NULL == port)
			m_curHttpProxy.port = _T("");
		else
			m_curHttpProxy.port = tstring(port);
			
			if(m_curFtpProxy.name.size() > 0)
				buffer = tstring(_T("ftp=")) + m_curFtpProxy.name +
					tstring(_T(":")) + m_curFtpProxy.port;
			if(m_curGopherProxy.name.size() > 0)
			{
				if(buffer.size() > 0)
					buffer +=tstring(_T(";"));
				buffer +=tstring(_T("gopher=")) + m_curGopherProxy.name +
					tstring(_T(":")) + m_curGopherProxy.port;
			}
			if(NULL != addr)
			{
				if(buffer.size() > 0)
					buffer +=tstring(_T(";"));
				buffer +=tstring(_T("http=")) + tstring(addr) + tstring(_T(":")) +
					tstring(port);
			}
			if(m_curHttpsProxy.name.size() > 0)
			{
				if(buffer.size() > 0)
					buffer +=tstring(_T(";"));
				buffer +=tstring(_T("https=")) + m_curHttpsProxy.name +
					tstring(_T(":")) + m_curHttpsProxy.port;
			}
			if(m_curSocksProxy.name.size() > 0)
			{
				if(buffer.size() > 0)
					buffer +=tstring(_T(";"));
				buffer +=tstring(_T("socks=")) + m_curSocksProxy.name +
					tstring(_T(":")) + m_curSocksProxy.port;
			}
	}
	else if(0 == temp.compare(_T("ftp")))
	{
		if(NULL == addr)
			m_curFtpProxy.name = _T("");
		else
			m_curFtpProxy.name = tstring(addr);
		if(NULL == port)
			m_curFtpProxy.port = _T("");
		else
			m_curFtpProxy.port = tstring(port);
			
			if(NULL != addr)
				buffer = tstring(_T("ftp=")) + tstring(addr) +
					tstring(_T(":")) + tstring(port);
			if(m_curGopherProxy.name.size() > 0)
			{
				if(buffer.size() > 0)
					buffer +=tstring(_T(";"));
				buffer +=tstring(_T("gopher=")) + m_curGopherProxy.name +
					tstring(_T(":")) + m_curGopherProxy.port;
			}
			if(m_curHttpProxy.name.size() > 0)
			{
				if(buffer.size() > 0)
					buffer +=tstring(_T(";"));
				buffer +=tstring(_T("http=")) + m_curHttpProxy.name +
					tstring(_T(":")) + m_curHttpProxy.port;
			}
			if(m_curHttpsProxy.name.size() > 0)
			{
				if(buffer.size() > 0)
					buffer +=tstring(_T(";"));
				buffer +=tstring(_T("https=")) + m_curHttpsProxy.name +
					tstring(_T(":")) + m_curHttpsProxy.port;
			}
			if(m_curSocksProxy.name.size() > 0)
			{
				if(buffer.size() > 0)
					buffer +=tstring(_T(";"));
				buffer +=tstring(_T("socks=")) + m_curSocksProxy.name +
					tstring(_T(":")) + m_curSocksProxy.port;
			}
	}
	else if(0 == temp.compare(_T("gopher")))
	{
		if(NULL == addr)
			m_curGopherProxy.name = _T("");
		else
			m_curGopherProxy.name = tstring(addr);
		if(NULL == port)
			m_curGopherProxy.port = _T("");
		else
			m_curGopherProxy.port = tstring(port);
			
			if(m_curFtpProxy.name.size() > 0)
				buffer = tstring(_T("ftp=")) + m_curFtpProxy.name +
					tstring(_T(":")) + m_curFtpProxy.port;
			if(NULL !=addr)
			{
				if(buffer.size() > 0)
					buffer +=tstring(_T(";"));
				buffer +=tstring(_T("gopher=")) + tstring(addr) +
					tstring(_T(":")) + tstring(port);
			}
			if(m_curHttpProxy.name.size() > 0)
			{
				if(buffer.size() > 0)
					buffer +=tstring(_T(";"));
				buffer +=tstring(_T("http=")) + m_curHttpProxy.name +
					tstring(_T(":")) + m_curHttpProxy.port;
			}
			if(m_curHttpsProxy.name.size() > 0)
			{
				if(buffer.size() > 0)
					buffer +=tstring(_T(";"));
				buffer +=tstring(_T("https=")) + m_curHttpsProxy.name +
					tstring(_T(":")) + m_curHttpsProxy.port;
			}
			if(m_curSocksProxy.name.size() > 0)
			{
				if(buffer.size() > 0)
					buffer +=tstring(_T(";"));
				buffer +=tstring(_T("socks=")) + m_curSocksProxy.name +
					tstring(_T(":")) + m_curSocksProxy.port;
			}
	}
	else if(0 == temp.compare(_T("socks")))
	{
		if(NULL == addr)
			m_curSocksProxy.name = _T("");
		else
			m_curSocksProxy.name = tstring(addr);
		if(NULL == port)
			m_curSocksProxy.port = _T("");
		else
			m_curSocksProxy.port = tstring(port);
			
			if(m_curFtpProxy.name.size() > 0)
				buffer = tstring(_T("ftp=")) + m_curFtpProxy.name +
					tstring(_T(":")) + m_curFtpProxy.port;
			if(m_curGopherProxy.name.size() > 0)
			{
				if(buffer.size() > 0)
					buffer +=tstring(_T(";"));
				buffer +=tstring(_T("gopher=")) + m_curGopherProxy.name +
					tstring(_T(":")) + m_curGopherProxy.port;
			}
			if(m_curHttpProxy.name.size() > 0)
			{
				if(buffer.size() > 0)
					buffer +=tstring(_T(";"));
				buffer +=tstring(_T("http=")) + m_curHttpProxy.name +
					tstring(_T(":")) + m_curHttpProxy.port;
			}
			if(m_curHttpsProxy.name.size() > 0)
			{
				if(buffer.size() > 0)
					buffer +=tstring(_T(";"));
				buffer +=tstring(_T("https=")) + m_curHttpsProxy.name +
					tstring(_T(":")) + m_curHttpsProxy.port;
			}
			if(NULL != addr)
			{
				if(buffer.size() > 0)
					buffer +=tstring(_T(";"));
				buffer +=tstring(_T("socks=")) + tstring(addr) +
					tstring(_T(":")) + tstring(port);
			}
	}
	
#ifndef USE_WININET	
	DWORD disp;
	HKEY key;

	if(ERROR_SUCCESS != RegCreateKeyEx(HKEY_CURRENT_USER,GD_SYSTEM_PROXY_REG,
									0,NULL,REG_OPTION_NON_VOLATILE,KEY_QUERY_VALUE|KEY_SET_VALUE,
									NULL,&key,&disp))
			return FALSE;
//	if(ERROR_SUCCESS != RegQueryValueEx(key,"ProxyServer",NULL,&regtype,(BYTE*)szBuf,&size))
//	{
//		if(ERROR_SUCCESS != RegSetValueEx(key,"ProxyServer",NULL,REG_SZ,(LPBYTE)szBuf,_tcslen(szBuf)))
//		{
//			RegCloseKey(key);
//			return FALSE;
//		}
//	}
	RegSetValueEx(key,"ProxyServer",NULL,REG_SZ,(LPBYTE)buffer.c_str(),buffer.size());
	RegFlushKey(key);
	RegCloseKey(key);
	return ( InternetSetOption(NULL,INTERNET_OPTION_SETTINGS_CHANGED,NULL,0) && 
				InternetSetOption(NULL,INTERNET_OPTION_REFRESH,NULL,0));
#else
	TCHAR active_if[MAX_TEXT_LENGTH];
	if(ERROR_SUCCESS!=GetActiveRasInterface(active_if,MAX_TEXT_LENGTH))
	{
		ReportErrMsg("In CProxyManager::SetProxy");
		return FALSE;
	}

	INTERNET_PER_CONN_OPTION_LIST    List;
	INTERNET_PER_CONN_OPTION         Option[1];
	unsigned long                    nSize = sizeof(INTERNET_PER_CONN_OPTION_LIST);

	Option[0].dwOption = INTERNET_PER_CONN_PROXY_SERVER;
	Option[0].Value.pszValue = (PCHAR)buffer.c_str();

	List.dwSize = sizeof(INTERNET_PER_CONN_OPTION_LIST);
	List.pszConnection = (active_if[0] ? active_if : NULL);
	List.dwOptionCount = 1;
	List.dwOptionError = 0;
	List.pOptions = Option;

	if(!InternetSetOption(NULL, INTERNET_OPTION_PER_CONNECTION_OPTION, &List, nSize))
	{
		ReportErrMsg("In CProxyManager::SetProxy");
		return FALSE;
	}
	return InternetSetOption(NULL,INTERNET_OPTION_REFRESH,NULL,0);
#endif
}


// 此处为何fstream打开文件失败？
BOOL CProxyManager::ImportProxyList(const TCHAR* szFile)
{
	int indexAddr,indexPort,indexType;
	TCHAR buf[MAX_PROXY_LENGTH];
	TCHAR typebuf[10];
	FILE* file = fopen(szFile,"r");
//	impFile.open("E:\\M.txt",ios::in);
//	impFile.seekg(0,ios::beg);
//	if(ios::goodbit & impFile.rdstate())	
//	{
//		while(0 != impFile.eof())
		while(!feof(file))
		{
//			impFile.get(buf,MAX_PROXY_LENGTH);
			buf[0] = _T('\0');
			fgets(buf,MAX_PROXY_LENGTH,file);

			if(*buf != _T('\n') && *buf != _T('\0') && *buf != _T(' ') && *buf != _T('\t') )//&& _istdigit(*buf))
			{	// 暂时不对有效性进行检查
				indexAddr = GetAddrIndex(buf);
				if(indexAddr == -1)
					m_ProxyVec.push_back(ProxyEntry(buf,_T("80"),_T("http")));
				else if(indexAddr >= 0 && buf[indexAddr] != _T(':') && buf[indexAddr]!=_T(' '))	// 使用缺省的端口80和协议http
				{
					buf[indexAddr] = _T('\0');
					m_ProxyVec.push_back(ProxyEntry(buf,_T("80"),_T("http")));
				}
				else
				{
					buf[indexAddr] = _T('\0');
					indexPort = GetPortIndex(buf + indexAddr + 1);
					if(indexPort == -1)
						m_ProxyVec.push_back(ProxyEntry(buf,buf+indexAddr+1,_T("http")));
					else if(indexPort >= 0 && (buf+indexAddr+1)[indexPort] != _T('@'))			// 使用缺省协议http
					{
						(buf+indexAddr+1)[indexPort] = _T('\0');
						m_ProxyVec.push_back(ProxyEntry(buf,buf+indexAddr+1,_T("http")));
					}
					else
					{
						(buf+indexAddr+1)[indexPort] = _T('\0');
						indexType = GetTypeIndex(buf+indexAddr+indexPort+2);
						(buf+indexAddr+indexPort+2)[indexType] = _T('\0');
						typebuf[0] = _T('\0');
						if(0 == _tcsnicmp(buf+indexAddr+indexPort+2,_T("http"),4))
							_tcscpy(typebuf,_T("http"));
						else if(0 == _tcsnicmp(buf+indexAddr+indexPort+2,_T("ftp"),3))
							_tcscpy(typebuf,_T("ftp"));
						else if(0 == _tcsnicmp(buf+indexAddr+indexPort+2,_T("socks"),5))
							_tcscpy(typebuf,_T("socks"));
						else if(0 == _tcsnicmp(buf+indexAddr+indexPort+2,_T("https"),5))
							_tcscpy(typebuf,_T("https"));
						else if(0 == _tcsnicmp(buf+indexAddr+indexPort+2,_T("gopher"),6))
							_tcscpy(typebuf,_T("gopher"));
						else
							_tcscpy(typebuf,_T("invalid protocol"));
						m_ProxyVec.push_back(ProxyEntry(buf,buf+indexAddr+1,typebuf));
					}
				}
			}
		}
//		impFile.close();
		fclose(file);
		m_bChanged = TRUE;
		m_bReqFresh = TRUE;
		return TRUE;
//	}
//	impFile.close();
	fclose(file);
	return FALSE;
}

BOOL CProxyManager::ExportProxyList(const TCHAR* szFile)
{
	fstream of;
	of.open(szFile,ios::out|ios::trunc);
	if(of.rdstate() != ios::goodbit)
		return FALSE;
	tstring buf;
	for(int i=0;i<m_ProxyVec.size();i++)
	{
		buf=m_ProxyVec[i].name+tstring(_T(":"))
			+m_ProxyVec[i].port+tstring(_T("@"))+m_ProxyVec[i].type+tstring(_T("\n"));
		of.write(buf.c_str(),buf.size());
	}
	of.close();
	return TRUE;
}

BOOL CProxyManager::IsProxyEnable()
{
#ifndef USE_WININET
	DWORD disp;
	DWORD regtype = REG_DWORD;
	DWORD buf=0;
	unsigned long size = sizeof(DWORD);
	HKEY key;
	if(ERROR_SUCCESS != RegCreateKeyEx(HKEY_CURRENT_USER,GD_SYSTEM_PROXY_REG,
									0,NULL,REG_OPTION_NON_VOLATILE,KEY_QUERY_VALUE,
									NULL,&key,&disp))
	{
		return FALSE;
	}
	if(ERROR_SUCCESS != RegQueryValueEx(key,"ProxyEnable",NULL,&regtype,(BYTE*)&buf,&size))
	{
		RegCloseKey(key);
		return FALSE;
	}
	RegCloseKey(key);	
	return (1 == buf);
#else
	TCHAR active_if[MAX_TEXT_LENGTH];
	if(ERROR_SUCCESS!=GetActiveRasInterface(active_if,MAX_TEXT_LENGTH))
	{
		ReportErrMsg("In CProxyManager::IsProxyEnable");
		return FALSE;
	}

	INTERNET_PER_CONN_OPTION_LIST    List;
	INTERNET_PER_CONN_OPTION         Option[1];
	unsigned long                    nSize = sizeof(INTERNET_PER_CONN_OPTION_LIST);

	Option[0].dwOption = INTERNET_PER_CONN_FLAGS;

	List.dwSize = sizeof(INTERNET_PER_CONN_OPTION_LIST);
	List.pszConnection = (active_if[0] ? active_if : NULL);
	List.dwOptionCount = 1;
	List.dwOptionError = 0;
	List.pOptions = Option;

	if(!InternetQueryOption(NULL, INTERNET_OPTION_PER_CONNECTION_OPTION, &List, &nSize))
	{
		ReportErrMsg("In CProxyManager::IsProxyEnable");
		return FALSE;
	}
	if((Option[0].Value.dwValue & PROXY_TYPE_PROXY) == PROXY_TYPE_PROXY)
		return TRUE;
	else
		return FALSE;
#endif
}

void CProxyManager::SetAll(BOOL bSetAll)
{
	m_bSetAll = bSetAll;
}

BOOL CProxyManager::Delete(int index)
{
	vector<ProxyEntry>::iterator vit;
	int i;
	for(i=0,vit = m_ProxyVec.begin();vit!=m_ProxyVec.end() && i<index;i++,vit++);
	if(vit == m_ProxyVec.end())
		return FALSE;
	else
	{
		m_ProxyVec.erase(vit);
		SetReqFresh();
		SetChanged();
	}
	return TRUE;
}

BOOL CProxyManager::IsProxyEqual(const TCHAR* name,const TCHAR* port,int proxyType)
{
	switch(proxyType)
	{
	case COMPARE_HTTPS:
		return 0 == m_curHttpsProxy.name.compare(name) &&
				0 == m_curHttpsProxy.port.compare(port)
				? TRUE : FALSE;
	case COMPARE_HTTP:
		return 0 == m_curHttpProxy.name.compare(name) &&
				0 == m_curHttpProxy.port.compare(port)
				? TRUE : FALSE;
	case COMPARE_GOPHER:
		return 0 == m_curGopherProxy.name.compare(name) &&
				0 == m_curGopherProxy.port.compare(port)
				? TRUE : FALSE;
	case COMPARE_FTP:
		return 0 == m_curFtpProxy.name.compare(name) &&
				0 == m_curFtpProxy.port.compare(port)
				? TRUE : FALSE;
	case COMPARE_SOCKS:
		return 0 == m_curSocksProxy.name.compare(name) &&
				0 == m_curSocksProxy.port.compare(port)
				? TRUE : FALSE;
	default:
		return FALSE;
	}
}

BOOL CProxyManager::AddProxy(ProxyEntry pe)
{
	m_ProxyVec.push_back(pe);
	m_bReqFresh = TRUE;
	m_bChanged = TRUE;
	return TRUE;
}

const TCHAR* CProxyManager::GetCurProxy(int type)
{
	switch(type)
	{
	case COMPARE_HTTPS:
//		return (m_curHttpsProxy.name + tstring(":") + m_curHttpsProxy.port).c_str();
		return m_curHttpsProxy.name.c_str();
	case COMPARE_HTTP:
//return (m_curHttpProxy.name + tstring(":") + m_curHttpProxy.port).c_str();
		return m_curHttpProxy.name.c_str();
	case COMPARE_GOPHER:
//		return (m_curGopherProxy.name + tstring(":") + m_curGopherProxy.port).c_str();
		return m_curGopherProxy.name.c_str();
	case COMPARE_FTP:
//		return (m_curFtpProxy.name + tstring(":") + m_curFtpProxy.port).c_str();
		return m_curFtpProxy.name.c_str();
	case COMPARE_SOCKS:
//		return (m_curSocksProxy.name + tstring(":") + m_curSocksProxy.port).c_str();
		return m_curSocksProxy.name.c_str();
	default:
		return FALSE;
	}
}
