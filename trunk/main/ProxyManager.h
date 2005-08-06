// ProxyManager.h: interface for the CProxyManager class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_PROXYMANAGER_H__9118A929_5C9F_4DAF_901C_4D5C3BF5BB0E__INCLUDED_)
#define AFX_PROXYMANAGER_H__9118A929_5C9F_4DAF_901C_4D5C3BF5BB0E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class ProxyEntry
{
public:
	tstring name;		// 服务器名
	tstring port;		// 端口
	tstring type;		// 协议类型
	tstring comment;	// 描述

	ProxyEntry(TCHAR* n,TCHAR* p,TCHAR* t)
	{
		name = n;
		port = p;
		type = t;
		comment = _T("");
	}
	ProxyEntry(TCHAR* n,TCHAR* p,TCHAR* t,TCHAR* c)
	{
		name = n;
		port = p;
		type = t;
		comment = c;
	}
	ProxyEntry(){};
};

#define		COMPARE_HTTP		100
#define		COMPARE_HTTPS		200
#define		COMPARE_GOPHER		300
#define		COMPARE_SOCKS		400
#define		COMPARE_FTP			500

#define		HTTP				COMPARE_HTTP
#define		HTTPS				COMPARE_HTTPS
#define		GOPHER				COMPARE_GOPHER
#define		SOCKS				COMPARE_SOCKS
#define		FTP					COMPARE_FTP

class CProxyManager		// 代理服务器管理模块,详见开发计划 
{
public:
	CProxyManager();
	virtual ~CProxyManager();

private:

	vector<ProxyEntry> m_ProxyVec;
	
	ProxyEntry m_curHttpProxy;
	ProxyEntry m_curHttpsProxy;
	ProxyEntry m_curGopherProxy;
	ProxyEntry m_curSocksProxy;
	ProxyEntry m_curFtpProxy;

	BOOL m_bChanged;
	BOOL m_bReqFresh;
	BOOL m_bSetAll;
	
	int GetAddrIndex(TCHAR* sz);
	int GetPortIndex(TCHAR* sz);
	int GetTypeIndex(TCHAR* sz);

public:
	const TCHAR* GetCurProxy(int type);
	BOOL AddProxy(ProxyEntry pe);
	BOOL IsProxyEqual(const TCHAR* name,const TCHAR* port,int proxyType);
	BOOL Delete(int index);
	void SetAll(BOOL bSetAll=TRUE);
	BOOL IsProxyEnable();
	
	inline	void SetChanged(BOOL bChanged=TRUE){		m_bChanged = bChanged;	}
	inline	BOOL ReqFresh(){	return m_bReqFresh;	}
	inline	void SetReqFresh(BOOL bReqFresh=TRUE){	m_bReqFresh = bReqFresh;}
	inline	BOOL HasChanged(){	return m_bChanged;  }
	inline	BOOL IsSetAll(){	return m_bSetAll;	}
	BOOL ImportProxyList(const TCHAR* szFile);
	BOOL ExportProxyList(const TCHAR* szFile);
	inline	vector<ProxyEntry>& GetProxyList(){		return m_ProxyVec;		}

	BOOL SetProxy(const TCHAR* addr,const TCHAR* port,const TCHAR* type);
	
	BOOL EnableProxy(BOOL bEnable=TRUE);
	BOOL GetProxySettingFromIE();	
	BOOL Init();
	BOOL Store();
};

#endif // !defined(AFX_PROXYMANAGER_H__9118A929_5C9F_4DAF_901C_4D5C3BF5BB0E__INCLUDED_)
