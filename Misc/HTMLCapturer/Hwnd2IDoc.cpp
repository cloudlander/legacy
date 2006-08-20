// Hwnd2IDoc.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

class elem
{
public:
	string title;
	HWND hwnd;

	elem(TCHAR* t,HWND h)
	{
		title = t;
		hwnd = h;
	}
};
	
vector<elem> g_vec;

void ShowContent(CComPtr<IHTMLDocument2>& pDoc)
{
	CComPtr<IDispatch> pDisp;
	CComPtr<IHTMLElementCollection> pElemColl;
	BSTR bstr;
	HRESULT	hr = pDoc->get_all(&pElemColl);
    if (FAILED(hr))
        return;

    long lNumElems = 0;
	int i;
    pElemColl->get_length(&lNumElems);
    for (i = 0; i < lNumElems; i++)
    {
           _variant_t vtItem((long)i), vtEmpty;
           hr = pElemColl->item(vtItem, vtEmpty, &pDisp);

            if (FAILED(hr))
               break;

            // Get the IHTMLElement interface
            CComPtr<IHTMLElement> pElem;

            hr = pDisp->QueryInterface(IID_IHTMLElement, (LPVOID*)&pElem);
            pDisp.Release();

            if (FAILED(hr))
               break;

			hr = pElem->get_tagName((BSTR*)&bstr);
            if (FAILED(hr))
               break;
			bstr_t str(bstr);
			if(((char*)str) != 0 && 0 == _tcscmp("P",(char*)str))
			{
				hr=pElem->get_innerText((BSTR*)&bstr);
	            if (FAILED(hr))
		           break;
				str=bstr_t(bstr);
				if(((char*)str) != 0)
					cout<<(char*)str<<endl;
			}
	}
}

BOOL CALLBACK EnumChildProc(HWND hwnd,LPARAM lParam)
{
	TCHAR	buf[100];

	::GetClassName( hwnd, (LPTSTR)&buf, 100 );
	if ( _tcscmp( buf, _T("Internet Explorer_Server") ) == 0 )
	{
		CoInitialize( NULL );
		// Explicitly load MSAA so we know if it's installed
		HINSTANCE hInst = ::LoadLibrary( _T("OLEACC.DLL") );
		if ( hInst != NULL )
		{
			if ( hwnd != NULL )
			{
				CComPtr<IHTMLDocument2> spDoc;
				LRESULT lRes;
					
				UINT nMsg = ::RegisterWindowMessage( _T("WM_HTML_GETOBJECT") );
				::SendMessageTimeout( hwnd, nMsg, 0L, 0L, SMTO_ABORTIFHUNG, 1000, (DWORD*)&lRes );

				LPFNOBJECTFROMLRESULT pfObjectFromLresult = (LPFNOBJECTFROMLRESULT)::GetProcAddress( hInst, _T("ObjectFromLresult") );
				if ( pfObjectFromLresult != NULL )
				{
					HRESULT hr;
					hr = (*pfObjectFromLresult)( lRes, IID_IHTMLDocument2, 0, (void**)&spDoc );
					if ( SUCCEEDED(hr) )
						ShowContent(spDoc);
				}
			} 
			::FreeLibrary( hInst );
		} // else Active Accessibility is not installed
		CoUninitialize();
	}
	return TRUE;
};


void CaptureContent(HWND hwnd)
{
	::EnumChildWindows( hwnd, EnumChildProc, 0);
}

BOOL CALLBACK EnumProc(HWND hwnd,LPARAM lParam)
{
	TCHAR buf[255];
	if(::IsWindow(hwnd) && ::IsWindowVisible(hwnd))
	{
		if(::GetWindowText(hwnd,buf,255) > 0 && buf[0] != _T('\0') && 0 != _tcscmp(buf,"Program Manager"))
			g_vec.push_back(elem(buf,hwnd));			
	}
	return TRUE;
}

int main(int argc, char* argv[])
{
	while(true)
	{
		g_vec.clear();
		cout<<endl<<"Available topmost windows:"<<endl;
		::EnumWindows(EnumProc,0);
		for(int i=0;i<g_vec.size();i++)
			cout<<i<<":  "<<g_vec[i].title<<endl;
		cout<<"Enter the number of Window u wanna catch its contents: (-1 to quit)";
		cin>>i;
		if(i>=0 && i<g_vec.size())
			CaptureContent(g_vec[i].hwnd);
		else
			break;
	}
	return 0;
}
