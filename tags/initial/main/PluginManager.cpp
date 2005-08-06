// PluginManager.cpp: implementation of the CPluginManager class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Utility.h"
#include "PluginManager.h"
#include <exdispid.h>

#pragma comment(lib,"version")
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CPluginManager::CPluginManager()
{

}

CPluginManager::~CPluginManager()
{
//  if added, there will be a crash,why?
//	for(int i=0;i<vecPlugin.size();i++)
//		FreeLibrary(vecPlugin[i].hDll);
}

BOOL CPluginManager::GetDllDescription(const TCHAR* szFileName,TCHAR* Desc)
{
	DWORD dw;
	int size=GetFileVersionInfoSize((TCHAR*)szFileName,&dw);
	if(0==size)	return FALSE;
	char* pBlock=new char[size];
	if(FALSE==GetFileVersionInfo((TCHAR*)szFileName,dw,size,pBlock))
	{
		delete[] pBlock;
		return FALSE;
	}
	char SubBlock[MAX_TEXT_LENGTH];
	UINT cbTranslate;
	char* lpBuffer;
	UINT dwBytes;

	struct LANGANDCODEPAGE {
		WORD wLanguage;
		WORD wCodePage;
	} *lpTranslate;
	
	// Read the list of languages and code pages.
	
	VerQueryValue(pBlock, 
		TEXT("\\VarFileInfo\\Translation"),
		(LPVOID*)&lpTranslate,
		&cbTranslate);
	
	// Read the file description for each language and code page.
		
	for(int i=0; i < (cbTranslate/sizeof(struct LANGANDCODEPAGE)); i++ )
	{
		wsprintf( SubBlock, 
            _T("\\StringFileInfo\\%04x%04x\\FileDescription"),
            lpTranslate[i].wLanguage,
            lpTranslate[i].wCodePage);
		
		// Retrieve file description for language and code page "i". 
		VerQueryValue(pBlock, 
			SubBlock, 
			(LPVOID*)&lpBuffer, 
			&dwBytes); 
	}
	_tcsncpy(Desc,lpBuffer,dwBytes);
	delete[] pBlock;
	return dwBytes!=0 ? TRUE : FALSE;
}

BOOL CPluginManager::Init(HMENU menu)
{
	TCHAR desc[MAX_TEXT_LENGTH];
	hmenu=menu;
	WIN32_FIND_DATA FileData;
	HANDLE hSearch;
	HMODULE hDll;
	BOOL bFinished=FALSE;
	int i=0;
	MENUITEMINFO mii;
	mii.cbSize = sizeof(MENUITEMINFO);
	mii.fMask = MIIM_TYPE | MIIM_ID;
	mii.fType = MFT_STRING;
	tstring DllDir;
	tstring Dlls(GD_MODULE_PATH);
	Dlls+=_T("*.dll");
	vecPlugin.clear();
	vecSession.clear();
	hSearch=FindFirstFile(Dlls.c_str(),&FileData);
	if (hSearch == INVALID_HANDLE_VALUE) 
		return FALSE; 
	while(!bFinished)
	{
		DllDir=GD_MODULE_PATH;
		tstring FileName=DllDir+tstring(FileData.cFileName);
		hDll=LoadLibrary(FileName.c_str());
		if(NULL!=hDll)
		{
			// start get all dll's interface 
			MAINPROC mp=(MAINPROC)GetProcAddress(hDll,_T("Create"));
			if(NULL!=mp)
			{
				vecPlugin.push_back(Plugin(hDll,mp));
				mii.wID=IDM_PLUGIN_BASE+i;
				if(TRUE==CPluginManager::GetDllDescription(FileName.c_str(),desc))
				{
					mii.cch=_tcslen(desc);
					mii.dwTypeData=desc;
				}
				else
				{
					tstring tmp=FileData.cFileName;
					tmp=tmp.substr(0,tmp.rfind(_T(".")));
					_tcsncpy(desc,tmp.c_str(),tmp.size());
					desc[tmp.size()]=_T('\0');
					mii.cch=tmp.size();
					mii.dwTypeData=desc;
				}
				InsertMenuItem(menu,2,TRUE,&mii);
				i++;
			}
		}
		if (!FindNextFile(hSearch, &FileData)) 
		{
			if (GetLastError() == ERROR_NO_MORE_FILES) 
			{ 
				bFinished = TRUE; 
			} 
			else 
			{ 
				ReportErrMsg();
				return FALSE;
			} 
		}
	} 
	if (!FindClose(hSearch)) 
	{ 
		ReportErrMsg();
		return FALSE;
	} 
	if(0==i)
	{
		DeleteMenu(menu,1,MF_BYPOSITION);
	}
	return TRUE;
}

MAINPROC CPluginManager::Execute(int id)
{
	return vecPlugin[id-IDM_PLUGIN_BASE]._proc_main;
}

void CPluginManager::Call(DISPID msg,DISPPARAMS* pDispParams)
{
	switch(msg)
	{
	case DISPID_BEFORENAVIGATE2:
		EnumSessionBEFORENAVIGATE2("OnBeforeNavigate2",pDispParams);
		break;
	case DISPID_COMMANDSTATECHANGE:
		EnumSessionCOMMANDSTATECHANGE("OnCommandStateChange",pDispParams);
		break;
	case DISPID_DOCUMENTCOMPLETE:
		EnumSessionDOCUMENTCOMPLETE("OnDocumentComplete",pDispParams);
		break;
	case DISPID_DOWNLOADBEGIN:
		EnumSessionDOWNLOADBEGIN("OnDownloadBegin",pDispParams);
		break;
	case DISPID_DOWNLOADCOMPLETE:
		EnumSessionDOWNLOADCOMPLETE("OnDownloadComplete",pDispParams);
		break;
	case DISPID_NAVIGATECOMPLETE2:
		EnumSessionNAVIGATECOMPLETE2("OnNavigateComplete2",pDispParams);
		break;
	case DISPID_NEWWINDOW2:
		EnumSessionNEWWINDOW2("OnNewWindow2",pDispParams);
		break;
	case DISPID_ONFULLSCREEN:
		EnumSessionFULLSCREEN("OnFullScreen",pDispParams);
		break;
	case DISPID_ONMENUBAR:
		EnumSessionMENUBAR("OnMenuBar",pDispParams);
		break;
	case DISPID_ONQUIT:
		EnumSessionQUIT("OnQuit",pDispParams);
		break;
	case DISPID_ONSTATUSBAR:
		EnumSessionSTATUSBAR("OnStatusBar",pDispParams);
		break;
	case DISPID_ONTHEATERMODE:
		EnumSessionTHEATERMODE("OnTheaterMode",pDispParams);
		break;
	case DISPID_ONTOOLBAR:
		EnumSessionTOOLBAR("OnToolBar",pDispParams);
		break;
	case DISPID_ONVISIBLE:
		EnumSessionVISIBLE("OnVisible",pDispParams);
		break;
	case DISPID_PROGRESSCHANGE:
		EnumSessionPROGRESSCHANGE("OnProgressChange",pDispParams);
		break;
	case DISPID_PROPERTYCHANGE:
		EnumSessionPROPERTYCHANGE("OnPropertyChange",pDispParams);
		break;
	case DISPID_STATUSTEXTCHANGE:
		EnumSessionSTATUSTEXTCHANGE("OnStatusTextChange",pDispParams);
		break;
	case DISPID_TITLECHANGE:
		EnumSessionTITLECHANGE("OnTitleChange",pDispParams);
		break;
	default:
		return;
	}
}

void CPluginManager::EnumSessionBEFORENAVIGATE2( char* FuncName,DISPPARAMS* pDispParams)
{
	for(int i=0;i<vecSession.size();i++)
	{
		if(vecSession[i]>=0)
		{
			if(vecPlugin[vecSession[i]].hDll!=NULL)
			{
				if(vecPlugin[vecSession[i]]._proc_beforenavigate2==(BEFORENAVIGATE2)-1)
					continue;
				if(vecPlugin[vecSession[i]]._proc_beforenavigate2==NULL)
				{
					vecPlugin[vecSession[i]]._proc_beforenavigate2=(BEFORENAVIGATE2)GetProcAddress(vecPlugin[vecSession[i]].hDll,FuncName);
					if(NULL==vecPlugin[vecSession[i]]._proc_beforenavigate2)
					{
						vecPlugin[vecSession[i]]._proc_beforenavigate2=(BEFORENAVIGATE2)-1;
						continue;
					}
				}
				vecPlugin[vecSession[i]]._proc_beforenavigate2
						(pDispParams->rgvarg[6].pdispVal,
						pDispParams->rgvarg[5].pvarVal,
						pDispParams->rgvarg[4].pvarVal,
						pDispParams->rgvarg[3].pvarVal,
						pDispParams->rgvarg[2].pvarVal,
						pDispParams->rgvarg[1].pvarVal,
						pDispParams->rgvarg[0].pboolVal);
			}
		}
	}
}

void CPluginManager::EnumSessionCOMMANDSTATECHANGE( char* FuncName,DISPPARAMS* pDispParams)
{
	for(int i=0;i<vecSession.size();i++)
	{
		if(vecSession[i]>=0)
		{
			if(vecPlugin[vecSession[i]].hDll!=NULL)
			{
				if(vecPlugin[vecSession[i]]._proc_commandstatechange==(COMMANDSTATECHANGE)-1)
					continue;
				if(vecPlugin[vecSession[i]]._proc_commandstatechange==NULL)
				{
					vecPlugin[vecSession[i]]._proc_commandstatechange=(COMMANDSTATECHANGE)GetProcAddress(vecPlugin[vecSession[i]].hDll,FuncName);
					if(NULL==vecPlugin[vecSession[i]]._proc_commandstatechange)
					{
						vecPlugin[vecSession[i]]._proc_commandstatechange=(COMMANDSTATECHANGE)-1;
						continue;
					}
				}
				vecPlugin[vecSession[i]]._proc_commandstatechange
						(pDispParams->rgvarg[1].lVal,
						pDispParams->rgvarg[0].boolVal);
			}
		}
	}
}

void CPluginManager::EnumSessionDOCUMENTCOMPLETE( char* FuncName,DISPPARAMS* pDispParams)
{
	for(int i=0;i<vecSession.size();i++)
	{
		if(vecSession[i]>=0)
		{
			if(vecPlugin[vecSession[i]].hDll!=NULL)
			{
				if(vecPlugin[vecSession[i]]._proc_documentcomplete==(DOCUMENTCOMPLETE)-1)
					continue;
				if(vecPlugin[vecSession[i]]._proc_documentcomplete==NULL)
				{
					vecPlugin[vecSession[i]]._proc_documentcomplete=(DOCUMENTCOMPLETE)GetProcAddress(vecPlugin[vecSession[i]].hDll,FuncName);
					if(NULL==vecPlugin[vecSession[i]]._proc_documentcomplete)
					{
						vecPlugin[vecSession[i]]._proc_documentcomplete=(DOCUMENTCOMPLETE)-1;
						continue;
					}
				}
				vecPlugin[vecSession[i]]._proc_documentcomplete
					(pDispParams->rgvarg[1].pdispVal,
					pDispParams->rgvarg[0].pvarVal);
			}
		}
	}
}

void CPluginManager::EnumSessionDOWNLOADBEGIN( char* FuncName,DISPPARAMS* pDispParams)
{
	for(int i=0;i<vecSession.size();i++)
	{
		if(vecSession[i]>=0)
		{
			if(vecPlugin[vecSession[i]].hDll!=NULL)
			{
				if(vecPlugin[vecSession[i]]._proc_downloadbegin==(DOWNLOADBEGIN)-1)
					continue;
				if(vecPlugin[vecSession[i]]._proc_downloadbegin==NULL)
				{
					vecPlugin[vecSession[i]]._proc_downloadbegin=(DOWNLOADBEGIN)GetProcAddress(vecPlugin[vecSession[i]].hDll,FuncName);
					if(NULL==vecPlugin[vecSession[i]]._proc_downloadbegin)
					{
						vecPlugin[vecSession[i]]._proc_downloadbegin=(DOWNLOADBEGIN)-1;
						continue;
					}
				}
				vecPlugin[vecSession[i]]._proc_downloadbegin();
			}
		}
	}
}

void CPluginManager::EnumSessionDOWNLOADCOMPLETE( char* FuncName,DISPPARAMS* pDispParams)
{
	for(int i=0;i<vecSession.size();i++)
	{
		if(vecSession[i]>=0)
		{
			if(vecPlugin[vecSession[i]].hDll!=NULL)
			{
				if(vecPlugin[vecSession[i]]._proc_downloadcomplete==(DOWNLOADCOMPLETE)-1)
					continue;
				if(vecPlugin[vecSession[i]]._proc_downloadcomplete==NULL)
				{
					vecPlugin[vecSession[i]]._proc_downloadcomplete=(DOWNLOADCOMPLETE)GetProcAddress(vecPlugin[vecSession[i]].hDll,FuncName);
					if(NULL==vecPlugin[vecSession[i]]._proc_downloadcomplete)
					{
						vecPlugin[vecSession[i]]._proc_downloadcomplete=(DOWNLOADCOMPLETE)-1;
						continue;
					}
				}
				vecPlugin[vecSession[i]]._proc_downloadcomplete();
			}
		}
	}
}

void CPluginManager::EnumSessionNAVIGATECOMPLETE2( char* FuncName,DISPPARAMS* pDispParams)
{
	for(int i=0;i<vecSession.size();i++)
	{
		if(vecSession[i]>=0)
		{
			if(vecPlugin[vecSession[i]].hDll!=NULL)
			{
				if(vecPlugin[vecSession[i]]._proc_navigatecomplete2==(NAVIGATECOMPLETE2)-1)
					continue;
				if(vecPlugin[vecSession[i]]._proc_navigatecomplete2==NULL)
				{
					vecPlugin[vecSession[i]]._proc_navigatecomplete2=(NAVIGATECOMPLETE2)GetProcAddress(vecPlugin[vecSession[i]].hDll,FuncName);
					if(NULL==vecPlugin[vecSession[i]]._proc_navigatecomplete2)
					{
						vecPlugin[vecSession[i]]._proc_navigatecomplete2=(NAVIGATECOMPLETE2)-1;
						continue;
					}
				}
				vecPlugin[vecSession[i]]._proc_navigatecomplete2
					(pDispParams->rgvarg[1].pdispVal,
					pDispParams->rgvarg[0].pvarVal);
			}
		}
	}
}

void CPluginManager::EnumSessionNEWWINDOW2( char* FuncName,DISPPARAMS* pDispParams)
{
	for(int i=0;i<vecSession.size();i++)
	{
		if(vecSession[i]>=0)
		{
			if(vecPlugin[vecSession[i]].hDll!=NULL)
			{
				if(vecPlugin[vecSession[i]]._proc_newwindow2==(NEWWINDOW2)-1)
					continue;
				if(vecPlugin[vecSession[i]]._proc_newwindow2==NULL)
				{
					vecPlugin[vecSession[i]]._proc_newwindow2=(NEWWINDOW2)GetProcAddress(vecPlugin[vecSession[i]].hDll,FuncName);
					if(NULL==vecPlugin[vecSession[i]]._proc_newwindow2)
					{
						vecPlugin[vecSession[i]]._proc_newwindow2=(NEWWINDOW2)-1;
						continue;
					}
				}
				vecPlugin[vecSession[i]]._proc_newwindow2
					(pDispParams->rgvarg[1].ppdispVal,
					pDispParams->rgvarg[0].pboolVal);
			}
		}
	}
}

void CPluginManager::EnumSessionFULLSCREEN( char* FuncName,DISPPARAMS* pDispParams)
{
	for(int i=0;i<vecSession.size();i++)
	{
		if(vecSession[i]>=0)
		{
			if(vecPlugin[vecSession[i]].hDll!=NULL)
			{
				if(vecPlugin[vecSession[i]]._proc_fullscreen==(FULLSCREEN)-1)
					continue;
				if(vecPlugin[vecSession[i]]._proc_fullscreen==NULL)
				{
					vecPlugin[vecSession[i]]._proc_fullscreen=(FULLSCREEN)GetProcAddress(vecPlugin[vecSession[i]].hDll,FuncName);
					if(NULL==vecPlugin[vecSession[i]]._proc_fullscreen)
					{
						vecPlugin[vecSession[i]]._proc_fullscreen=(FULLSCREEN)-1;
						continue;
					}
				}
				vecPlugin[vecSession[i]]._proc_fullscreen(pDispParams->rgvarg[0].boolVal);
			}
		}
	}
}

void CPluginManager::EnumSessionMENUBAR( char* FuncName,DISPPARAMS* pDispParams)
{
	for(int i=0;i<vecSession.size();i++)
	{
		if(vecSession[i]>=0)
		{
			if(vecPlugin[vecSession[i]].hDll!=NULL)
			{
				if(vecPlugin[vecSession[i]]._proc_menubar==(MENUBAR)-1)
					continue;
				if(vecPlugin[vecSession[i]]._proc_menubar==NULL)
				{
					vecPlugin[vecSession[i]]._proc_menubar=(MENUBAR)GetProcAddress(vecPlugin[vecSession[i]].hDll,FuncName);
					if(NULL==vecPlugin[vecSession[i]]._proc_menubar)
					{
						vecPlugin[vecSession[i]]._proc_menubar=(MENUBAR)-1;
						continue;
					}
				}
				vecPlugin[vecSession[i]]._proc_menubar(pDispParams->rgvarg[0].boolVal);
			}
		}
	}
}

void CPluginManager::EnumSessionQUIT( char* FuncName,DISPPARAMS* pDispParams)
{
	for(int i=0;i<vecSession.size();i++)
	{
		if(vecSession[i]>=0)
		{
			if(vecPlugin[vecSession[i]].hDll!=NULL)
			{
				if(vecPlugin[vecSession[i]]._proc_quit==(QUIT)-1)
					continue;
				if(vecPlugin[vecSession[i]]._proc_quit==NULL)
				{
					vecPlugin[vecSession[i]]._proc_quit=(QUIT)GetProcAddress(vecPlugin[vecSession[i]].hDll,FuncName);
					if(NULL==vecPlugin[vecSession[i]]._proc_quit)
					{
						vecPlugin[vecSession[i]]._proc_quit=(QUIT)-1;
						continue;
					}
				}
				vecPlugin[vecSession[i]]._proc_quit();
			}
		}
	}
}

void CPluginManager::EnumSessionSTATUSBAR( char* FuncName,DISPPARAMS* pDispParams)
{
	for(int i=0;i<vecSession.size();i++)
	{
		if(vecSession[i]>=0)
		{
			if(vecPlugin[vecSession[i]].hDll!=NULL)
			{
				if(vecPlugin[vecSession[i]]._proc_statusbar==(STATUSBAR)-1)
					continue;
				if(vecPlugin[vecSession[i]]._proc_statusbar==NULL)
				{
					vecPlugin[vecSession[i]]._proc_statusbar=(STATUSBAR)GetProcAddress(vecPlugin[vecSession[i]].hDll,FuncName);
					if(NULL==vecPlugin[vecSession[i]]._proc_statusbar)
					{
						vecPlugin[vecSession[i]]._proc_statusbar=(STATUSBAR)-1;
						continue;
					}
				}
				vecPlugin[vecSession[i]]._proc_statusbar(pDispParams->rgvarg[0].boolVal);
			}
		}
	}
}

void CPluginManager::EnumSessionTHEATERMODE( char* FuncName,DISPPARAMS* pDispParams)
{
	for(int i=0;i<vecSession.size();i++)
	{
		if(vecSession[i]>=0)
		{
			if(vecPlugin[vecSession[i]].hDll!=NULL)
			{
				if(vecPlugin[vecSession[i]]._proc_theatermode==(THEATERMODE)-1)
					continue;
				if(vecPlugin[vecSession[i]]._proc_theatermode==NULL)
				{
					vecPlugin[vecSession[i]]._proc_theatermode=(THEATERMODE)GetProcAddress(vecPlugin[vecSession[i]].hDll,FuncName);
					if(NULL==vecPlugin[vecSession[i]]._proc_theatermode)
					{
						vecPlugin[vecSession[i]]._proc_theatermode=(THEATERMODE)-1;
						continue;
					}
				}
				vecPlugin[vecSession[i]]._proc_theatermode(pDispParams->rgvarg[0].boolVal);
			}
		}
	}
}

void CPluginManager::EnumSessionTOOLBAR( char* FuncName,DISPPARAMS* pDispParams)
{
	for(int i=0;i<vecSession.size();i++)
	{
		if(vecSession[i]>=0)
		{
			if(vecPlugin[vecSession[i]].hDll!=NULL)
			{
				if(vecPlugin[vecSession[i]]._proc_toolbar==(TOOLBAR)-1)
					continue;
				if(vecPlugin[vecSession[i]]._proc_toolbar==NULL)
				{
					vecPlugin[vecSession[i]]._proc_toolbar=(TOOLBAR)GetProcAddress(vecPlugin[vecSession[i]].hDll,FuncName);
					if(NULL==vecPlugin[vecSession[i]]._proc_toolbar)
					{
						vecPlugin[vecSession[i]]._proc_toolbar=(TOOLBAR)-1;
						continue;
					}
				}
				vecPlugin[vecSession[i]]._proc_toolbar(pDispParams->rgvarg[0].boolVal);
			}
		}
	}
}

void CPluginManager::EnumSessionVISIBLE( char* FuncName,DISPPARAMS* pDispParams)
{
	for(int i=0;i<vecSession.size();i++)
	{
		if(vecSession[i]>=0)
		{
			if(vecPlugin[vecSession[i]].hDll!=NULL)
			{
				if(vecPlugin[vecSession[i]]._proc_visible==(VISIBLE)-1)
					continue;
				if(vecPlugin[vecSession[i]]._proc_visible==NULL)
				{
					vecPlugin[vecSession[i]]._proc_visible=(VISIBLE)GetProcAddress(vecPlugin[vecSession[i]].hDll,FuncName);
					if(NULL==vecPlugin[vecSession[i]]._proc_visible)
					{
						vecPlugin[vecSession[i]]._proc_visible=(VISIBLE)-1;
						continue;
					}
				}
				vecPlugin[vecSession[i]]._proc_visible(pDispParams->rgvarg[0].boolVal);
			}
		}
	}
}

void CPluginManager::EnumSessionPROGRESSCHANGE( char* FuncName,DISPPARAMS* pDispParams)
{
	for(int i=0;i<vecSession.size();i++)
	{
		if(vecSession[i]>=0)
		{
			if(vecPlugin[vecSession[i]].hDll!=NULL)
			{
				if(vecPlugin[vecSession[i]]._proc_progresschange==(PROGRESSCHANGE)-1)
					continue;
				if(vecPlugin[vecSession[i]]._proc_progresschange==NULL)
				{
					vecPlugin[vecSession[i]]._proc_progresschange=(PROGRESSCHANGE)GetProcAddress(vecPlugin[vecSession[i]].hDll,FuncName);
					if(NULL==vecPlugin[vecSession[i]]._proc_progresschange)
					{
						vecPlugin[vecSession[i]]._proc_progresschange=(PROGRESSCHANGE)-1;
						continue;
					}
				}
				vecPlugin[vecSession[i]]._proc_progresschange
						(pDispParams->rgvarg[1].lVal,
						pDispParams->rgvarg[0].lVal);
			}
		}
	}
}

void CPluginManager::EnumSessionPROPERTYCHANGE( char* FuncName,DISPPARAMS* pDispParams)
{
	for(int i=0;i<vecSession.size();i++)
	{
		if(vecSession[i]>=0)
		{
			if(vecPlugin[vecSession[i]].hDll!=NULL)
			{
				if(vecPlugin[vecSession[i]]._proc_propertychange==(PROPERTYCHANGE)-1)
					continue;
				if(vecPlugin[vecSession[i]]._proc_propertychange==NULL)
				{
					vecPlugin[vecSession[i]]._proc_propertychange=(PROPERTYCHANGE)GetProcAddress(vecPlugin[vecSession[i]].hDll,FuncName);
					if(NULL==vecPlugin[vecSession[i]]._proc_propertychange)
					{
						vecPlugin[vecSession[i]]._proc_propertychange=(PROPERTYCHANGE)-1;
						continue;
					}
				}
				vecPlugin[vecSession[i]]._proc_propertychange(pDispParams->rgvarg[0].bstrVal);
			}
		}
	}
}

void CPluginManager::EnumSessionSTATUSTEXTCHANGE( char* FuncName,DISPPARAMS* pDispParams)
{
	for(int i=0;i<vecSession.size();i++)
	{
		if(vecSession[i]>=0)
		{
			if(vecPlugin[vecSession[i]].hDll!=NULL)
			{
				if(vecPlugin[vecSession[i]]._proc_statustextchange==(STATUSTEXTCHANGE)-1)
					continue;
				if(vecPlugin[vecSession[i]]._proc_statustextchange==NULL)
				{
					vecPlugin[vecSession[i]]._proc_statustextchange=(STATUSTEXTCHANGE)GetProcAddress(vecPlugin[vecSession[i]].hDll,FuncName);
					if(NULL==vecPlugin[vecSession[i]]._proc_statustextchange)
					{
						vecPlugin[vecSession[i]]._proc_statustextchange=(STATUSTEXTCHANGE)-1;
						continue;
					}
				}
				vecPlugin[vecSession[i]]._proc_statustextchange(pDispParams->rgvarg[0].bstrVal);
			}
		}
	}
}

void CPluginManager::EnumSessionTITLECHANGE( char* FuncName,DISPPARAMS* pDispParams)
{
	for(int i=0;i<vecSession.size();i++)
	{
		if(vecSession[i]>=0)
		{
			if(vecPlugin[vecSession[i]].hDll!=NULL)
			{
				if(vecPlugin[vecSession[i]]._proc_titlechange==(TITLECHANGE)-1)
					continue;
				if(vecPlugin[vecSession[i]]._proc_titlechange==NULL)
				{
					vecPlugin[vecSession[i]]._proc_titlechange=(TITLECHANGE)GetProcAddress(vecPlugin[vecSession[i]].hDll,FuncName);
					if(NULL==vecPlugin[vecSession[i]]._proc_titlechange)
					{
						vecPlugin[vecSession[i]]._proc_titlechange=(TITLECHANGE)-1;
						continue;
					}
				}
				vecPlugin[vecSession[i]]._proc_titlechange(pDispParams->rgvarg[0].bstrVal);
			}
		}
	}
}

void CPluginManager::SetSessionActive(int id,BOOL bActive)
{
	vecPlugin[id-IDM_PLUGIN_BASE].bActive=bActive;
	if(bActive)
	{
		EnableMenuItem(hmenu,id,MF_BYCOMMAND|MF_GRAYED);
		vecSession.push_back(id-IDM_PLUGIN_BASE);
	}
}

void CPluginManager::CloseSession(int sess)
{
	int i=sess-IDM_PLUGIN_BASE;
	if(TRUE==vecPlugin[i].bActive)
	{
		vecPlugin[i].bActive=FALSE;
		EnableMenuItem(hmenu,i+IDM_PLUGIN_BASE,MF_BYCOMMAND|MF_ENABLED);
		vector<int>::iterator iter;
		for(iter=vecSession.begin();iter!=vecSession.end();iter++)
			if(i==*iter)
			{
				vecSession.erase(iter);
				break;
			}
	}
}