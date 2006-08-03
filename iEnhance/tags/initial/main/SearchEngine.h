// SearchEngine.h: interface for the CSearchEngine class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SEARCHENGINE_H__CDFC390C_A205_4BEB_BED3_6733BE26FAD5__INCLUDED_)
#define AFX_SEARCHENGINE_H__CDFC390C_A205_4BEB_BED3_6733BE26FAD5__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class SE
{
public:
	tstring name;
	tstring url;

	SE(const TCHAR* n,const TCHAR* u)
	{
		name = n;
		url = u;
	}
};

class CSearchEngine  
{
public:
	CSearchEngine(IWebBrowser2* p)
	{
		m_pWB = p;
		m_hCombo = NULL;
	}
	
	virtual ~CSearchEngine();

private:
	int m_nDef;
	HWND m_hCombo;
	vector<SE> m_VecSe;
	IWebBrowser2Ptr m_pWB;
	BOOL m_bChanged;
	BOOL m_bReqFresh;
	BOOL m_bTyped;
public:
	BOOL StoreTyped();
	BOOL SetDefault(int index);
	BOOL Add(const TCHAR* name,const TCHAR* url);
	void DoSearch(int index,TCHAR* query);
	BOOL Store();
	BOOL Delete(int index);
	BOOL Init(HWND hCombo);
	
	inline vector<SE>& GetSE()
	{
		return m_VecSe;
	}

	inline int GetDefault()
	{
		return m_nDef;
	}

	inline int GetSize()
	{
		return m_VecSe.size();
	}

	inline const TCHAR* GetName(int index)
	{
		return m_VecSe[index].name.c_str();
	}
	
	inline const TCHAR* GetUrl(int index)
	{
		return m_VecSe[index].url.c_str();
	}
	
	inline BOOL ReqFresh()
	{
		return m_bReqFresh;
	}

	inline BOOL HasChanged()
	{
		return m_bChanged;
	}

	inline void SetChanged(BOOL bChanged=TRUE)
	{
		m_bChanged = bChanged;
	}

	inline void SetReqFresh(BOOL bChanged=FALSE)
	{
		m_bReqFresh = bChanged;
	}
};

#endif // !defined(AFX_SEARCHENGINE_H__CDFC390C_A205_4BEB_BED3_6733BE26FAD5__INCLUDED_)
