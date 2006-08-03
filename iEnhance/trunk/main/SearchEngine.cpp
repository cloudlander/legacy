// SearchEngine.cpp: implementation of the CSearchEngine class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "shlwapi.h"
#include "ieplusatl.h"
#include "SearchEngine.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
//#define new DEBUG_NEW
#endif

inline BYTE toHex(const BYTE &x)
{
       return x > 9 ? x + 55: x + 48;
}

void URLEncode(const BYTE* sIn,BYTE* sOut)
{
       LPBYTE pOutTmp = NULL;
       LPBYTE pOutBuf = NULL;
       const BYTE*  pInTmp = NULL;
       const BYTE* pInBuf =sIn;

       //alloc out buffer
       pOutBuf = sOut;
       if(pOutBuf)
       {
              pInTmp   = pInBuf;
              pOutTmp = pOutBuf;

              // do encoding

              while (*pInTmp)
              {
                     if(isalnum(*pInTmp))
                            *pOutTmp++ = *pInTmp;
                     else
                            if(isspace(*pInTmp))
                                   *pOutTmp++ = '+';
                            else
                            {
                                   *pOutTmp++ = '%';
                                   *pOutTmp++ = toHex(*pInTmp>>4);
                                   *pOutTmp++ = toHex(*pInTmp%16);
                            }
                     pInTmp++;
              }
              *pOutTmp = '\0';
       }
}

//
// This file is part of 
// Terra Informatica Lightweight Embeddable HTMEngine control SDK
// Created by Andrew Fedoniouk @ TerraInformatica.com
//
namespace aux 
{
  // byte_buffer class is an in-memory dynamic buffer implementation.
  class byte_buffer 
  {
    unsigned char*  _body;
    size_t          _allocated;
    size_t          _size;   
    unsigned char *reserve(size_t size)
    {
      size_t newsize = _size + size;
      if( newsize > _allocated ) 
      {
        _allocated *= 2;
        if(_allocated < newsize) 
           _allocated = newsize;
        unsigned char *newbody = new unsigned char[_allocated];
        memcpy(newbody,_body,_size);
        delete[] _body;
        _body = newbody;
      }
      return _body + _size;
    }  
   public:    
      byte_buffer():_size(0)      
        { _body = new unsigned char[_allocated = 256]; }
    ~byte_buffer()                { delete[] _body;  }    
    const unsigned char * data()  {  
             if(_size == _allocated) reserve(1); 
             _body[_size] = '\0'; return _body; 
    }    
    size_t length() const         { return _size; }    
    void push(unsigned char c)    { *reserve(1) = c; ++_size; }
    void push(const unsigned char *pc, size_t sz) 
        { memcpy(reserve(sz),pc,sz); _size += sz; }  
  };
}
namespace utf8
{
  // UTF8 stream  // class T must have two methods:
  //   void push(unsigned char c)
  //   void push(const unsigned char *pc, size_t sz)
  // bool X - true - XML markup character conversion 
  // (characters '<','>',etc).
  //          false - no conversion at all.   
template <class T, bool X = true>
  class ostream_t : public T
  {
  public:
    ostream_t()
    { 
      // utf8 byte order mark
      static unsigned char BOM[] = { 0xEF, 0xBB, 0xBF };
      T::push(BOM, sizeof(BOM));
    }    
    // intended to handle only ascii-7 strings
    // use this for markup output 
    ostream_t& operator << (const char* str) 
    { 
      T::push((const unsigned char*)str,strlen(str)); return *this; 
    }    
    // use UNICODE chars for value output
    ostream_t& operator << (const wchar_t* wstr)
    {
      const wchar_t *pc = wstr;
      for(unsigned int c = *pc; c ; c = *(++pc)) 
      {
        if(X)
          switch(c) 
          {
              case '<': *this << "<"; continue;
              case '>': *this << ">"; continue;
              case '&': *this << "&amp;"; continue;
              case '"': *this << "&quot;"; continue;
              case '\'': *this << "&apos;"; continue;
          }
        if (c < (1 << 7)) {
         T::push (c);
        } else if (c < (1 << 11)) {
         T::push ((c >> 6) | 0xc0);
         T::push ((c & 0x3f) | 0x80);
        } else if (c < (1 << 16)) {
         T::push ((c >> 12) | 0xe0);
         T::push (((c >> 6) & 0x3f) | 0x80);
         T::push ((c & 0x3f) | 0x80);
        } else if (c < (1 << 21)) {
         T::push ((c >> 18) | 0xe0);
         T::push (((c >> 12) & 0x3f) | 0x80);
         T::push (((c >> 6) & 0x3f) | 0x80);
         T::push ((c & 0x3f) | 0x80);
        }
      }
      return *this;
    }
  };  
 // raw ASCII/UNICODE -> UTF8 converter 
  typedef ostream_t<aux::byte_buffer,false> ostream;
 // ASCII/UNICODE -> UTF8 converter with XML support
  typedef ostream_t<aux::byte_buffer,true> oxstream;
}



//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////


CSearchEngine::~CSearchEngine()
{

}

BOOL CSearchEngine::Init(HWND hCombo)
{
	HKEY key;
	DWORD disp,type;
	tstring Profile=GD_MODULE_PATH;
	Profile+=_T("Search.ini");
	TCHAR szBuf[MAX_TEXT_LENGTH];
	m_bChanged = FALSE;
	m_bReqFresh=TRUE;
	BOOL bChecked=FALSE;
	unsigned long size = 2048;
	TCHAR* buf=new TCHAR[size];
	TCHAR* pbuf=buf;
	if(m_VecSe.size() > 0)
	{
		m_VecSe.clear();
	}
	m_nDef=-1;
	int i;
	HANDLE hFile=CreateFile(Profile.c_str(),GENERIC_READ,FILE_SHARE_READ,
		NULL,OPEN_EXISTING,FILE_ATTRIBUTE_ARCHIVE,NULL);
	if(INVALID_HANDLE_VALUE==hFile)
			return FALSE;
	else
		CloseHandle(hFile);
	i=GetPrivateProfileSectionNames(buf,size,Profile.c_str());
	if(i==size-2)	return FALSE;		// too big!
	while(_T('\0')!=pbuf[0])
	{
		if(!bChecked)		// haven't find the "main section" yet
		{
			m_nDef=GetPrivateProfileInt(pbuf,_T("Default"),-1,Profile.c_str());
			if(m_nDef>=0)	// found
			{
				bChecked=TRUE;
				pbuf+=_tcslen(pbuf)+1;
				continue;
			}
		}
		GetPrivateProfileString(pbuf,_T("URL"),_T("x5n&Qc"),szBuf,250,Profile.c_str());
		if(0!=_tcscmp(_T("x5n&Qc"),szBuf))
			m_VecSe.push_back(SE(pbuf,szBuf));
		pbuf+=_tcslen(pbuf)+1;
	}
	if(m_nDef >= m_VecSe.size() || m_nDef<0)
		m_nDef = 0;

//  初始化组合框列表
	m_bTyped = FALSE;
	if(NULL == hCombo)
		return TRUE;
	m_hCombo = hCombo;
	if(ERROR_SUCCESS != RegCreateKeyEx(HKEY_CURRENT_USER,GD_TYPED_REG,
									0,NULL,REG_OPTION_NON_VOLATILE,KEY_QUERY_VALUE|KEY_SET_VALUE,
									NULL,&key,&disp))
		return FALSE;
	if(REG_CREATED_NEW_KEY == disp)		// 首次创建
		return TRUE;
	::SendMessage(hCombo,CB_RESETCONTENT,0,0);

	COMBOBOXEXITEM ce;
	ZeroMemory(&ce,sizeof(ce));
	ce.mask = CBEIF_TEXT;
	DWORD tmp=10;
	i = 0;
	while(TRUE)
	{
		size = MAX_TEXT_LENGTH;
		if(ERROR_SUCCESS==SHEnumValue(key,i,szBuf,&size,&type,NULL,&tmp))
		{
			ce.pszText = szBuf;
			ce.cchTextMax = _tclen(szBuf);
			ce.iItem = i;
			::SendMessage(hCombo,CBEM_INSERTITEM,0,(LPARAM)&ce);
//			::SendMessage(hCombo,CB_INSERTSTRING,0,(LPARAM)szBuf);
			i++;
		}
		else
			break;
	}
	RegCloseKey(key);
	return TRUE;
}

BOOL CSearchEngine::Delete(int index)
{
	vector<SE>::iterator vse;
	int i;
	for(i=0,vse = m_VecSe.begin();vse!=m_VecSe.end() && i<index;i++,vse++);
	if(vse == m_VecSe.end())
		return FALSE;
	else
	{
		m_VecSe.erase(vse);
		m_bChanged = TRUE;
		m_bReqFresh = TRUE;
	}
	return TRUE;
}

BOOL CSearchEngine::Store()
{
	tstring Profile=GD_MODULE_PATH;
	Profile+=_T("Search.ini");
	DeleteFile(Profile.c_str());
	int i = 0;
	TCHAR szNum[10];
	int size = m_VecSe.size();
	szNum[0]=_T('\n');
	_itot(m_nDef,szNum,10);
	WritePrivateProfileString(_T("iEnhance"),_T("Default"),szNum,Profile.c_str());
	while(i <  size)
	{
		WritePrivateProfileString(m_VecSe[i].name.c_str(),_T("URL"),m_VecSe[i].url.c_str(),Profile.c_str());
		i++;
	}
	return TRUE;
}

void CSearchEngine::DoSearch(int index,TCHAR* query)
{
	if(NULL == query || query[0] == _T('\0'))
		return;
// not insert support Added here:
	
	if(NULL != m_hCombo)
	{
		if( CB_ERR == ::SendMessage(m_hCombo,CB_FINDSTRINGEXACT,-1,(LPARAM)query))
		{
			if((query[0]!=_T('n') || query[1]!=_T(' ')) && query[2]!=_T('\0'))
			{
				COMBOBOXEXITEM ce;
				ZeroMemory(&ce,sizeof(ce));
				ce.mask = CBEIF_TEXT;
				ce.pszText = query;
				ce.cchTextMax = _tclen(query);
				ce.iItem = ::SendMessage(m_hCombo,CB_GETCOUNT,0,0);
				::SendMessage(m_hCombo,CBEM_INSERTITEM,0,(LPARAM)&ce);
				m_bTyped = TRUE;
			}
			else
			{
				if(query[2]==_T('\0'))	return;
				query=query+2;
			}
		}
	}
	
	int i = index;

	if(-1 == index)
		i = m_nDef;

	variant_t vtEmpty;
	// we want to convert an MBCS string in lpszA
	int nLen = MultiByteToWideChar(CP_ACP, 0,query, -1, NULL, NULL);
	LPWSTR lpszW = new WCHAR[nLen];
	MultiByteToWideChar(CP_ACP, 0, query, -1, lpszW, nLen);
	BYTE* enc_query;
	if(0==_tcscmp(m_VecSe[i].name.c_str(),_T("Google")))
	{
		utf8::ostream utf8_query;
		utf8_query<<lpszW;
		enc_query=new BYTE[utf8_query.length()*3];
		URLEncode(utf8_query.data(),enc_query);
	}
	else
	{
		enc_query=new BYTE[2*nLen];
		_tcscpy((char*)enc_query,(const char*)query);
	}
//	TCHAR* nav=new TCHAR[_tcslen(query)+m_VecSe[i].url.size()+1];
	TCHAR* nav=new TCHAR[_tcslen((const char*)enc_query)+m_VecSe[i].url.size()+1];

	// free the string
	delete[] lpszW;

	nav[0] = _T('\0');
	int d = m_VecSe[i].url.find(_T("^"),0);
	if(-1 == d)
	{
		_stprintf(nav,m_VecSe[i].url.c_str(),enc_query);
		m_pWB->Navigate(bstr_t(nav),&vtEmpty,&vtEmpty,&vtEmpty,&vtEmpty);
	}
	else
	{
		tstring pd = m_VecSe[i].url.substr(d+1,m_VecSe[i].url.size());
		_stprintf(nav,pd.c_str(),query);

/*		
		SAFEARRAYBOUND rgsabound;
		rgsabound.cElements = lstrlen(nav);
		rgsabound.lLbound = 0;
*/
		
		LPVARIANT pvtPost;
		VARIANT vtPost;
		LPSAFEARRAY psa;
		psa = ::SafeArrayCreateVector(VT_UI1,0,lstrlen(nav));
		void* pvDestData;
		pvtPost = &vtPost;
		VariantInit(pvtPost);
				//	vtPost.vt = VT_UI1|VT_ARRAY;
		::SafeArrayAccessData(psa,&pvDestData);
				//	int x = ::SafeArrayGetElemsize(vtPost.parray);
		memcpy(pvDestData,nav,lstrlen(nav));
		::SafeArrayUnaccessData(psa);
		V_VT(pvtPost) = VT_UI1|VT_ARRAY;
		V_ARRAY(pvtPost) = psa;

		TCHAR header[500]= "Content-type: application/x-www-form-urlencoded\r\n";//Content-length:";
				//	_stprintf(buf,_T("%d\r\n\r\n"),_tcslen(nav));
				//	_tcscat(header,buf);
		VARIANT vtHeaders;
		vtHeaders.vt = VT_BSTR;
		USES_CONVERSION;
		vtHeaders.bstrVal = ::SysAllocString(T2COLE(header));

/*		VARIANT vtFrame;
		vtFrame.vt = VT_BSTR;
		vtFrame.bstrVal = ::SysAllocString(T2COLE("_BLANK"));
*/
		m_pWB->Navigate(bstr_t(m_VecSe[i].url.substr(0,d).c_str()),&vtEmpty,&vtEmpty,&vtPost,&vtHeaders);
		delete[] nav;
	}
}

BOOL CSearchEngine::Add(const TCHAR *name, const TCHAR *url)
{
	if(NULL == name || NULL == url)
		return FALSE;
	m_VecSe.push_back(SE(name,url));
	m_bReqFresh = TRUE;
	m_bChanged = TRUE;
	return TRUE;
}

BOOL CSearchEngine::SetDefault(int index)
{
	if(index <0 || index >= m_VecSe.size())
		return FALSE;
	m_nDef = index;
	m_bChanged = TRUE;
	m_bReqFresh = TRUE;
	return TRUE;
}

BOOL CSearchEngine::StoreTyped()
{
	if(NULL == m_hCombo || m_bTyped == FALSE)
		return TRUE;
	HKEY key;
	DWORD disp;
	int size;
	TCHAR szBuf[MAX_TEXT_LENGTH];
	int i;
	RegDeleteKey(HKEY_CURRENT_USER,GD_TYPED_REG);
	if(ERROR_SUCCESS != RegCreateKeyEx(HKEY_CURRENT_USER,GD_TYPED_REG,
									0,NULL,REG_OPTION_NON_VOLATILE,KEY_SET_VALUE,
									NULL,&key,&disp))
		return FALSE;
	COMBOBOXEXITEM ce;
	ZeroMemory(&ce,sizeof(ce));
	ce.mask = CBEIF_TEXT;
	size = ::SendMessage(m_hCombo,CB_GETCOUNT,0,0);
	i = 0;
	while(i <  size)
	{
//		::SendMessage(m_hCombo,CB_GETLBTEXT,i,(LPARAM)szBuf);
//		RegSetValueEx(key,szBuf,NULL,REG_SZ,NULL,0);
		ce.pszText = szBuf;
		ce.cchTextMax = MAX_TEXT_LENGTH;
		ce.iItem = i;
		::SendMessage(m_hCombo,CBEM_GETITEM,0,(LPARAM)&ce);
		RegSetValueEx(key,ce.pszText,NULL,REG_NONE,NULL,0);
		szBuf[0] = _T('\0');
		i++;
	}
	RegFlushKey(key);
	RegCloseKey(key);
	return TRUE;
}
