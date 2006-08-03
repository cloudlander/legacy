// WorkQueue.cpp: implementation of the CWorkQueue class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "wmd5.h"
#include "WorkQueue.h"
#include "wmd5dlg.h"
#include "md5.h"
#include "tipdlg.h"
#include "filefindex.h"
#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif
void
DisplayErrorText()
{
	DWORD dwLastError=GetLastError();
    HMODULE hModule = NULL; // default to system source
    LPSTR MessageBuffer;
    DWORD dwBufferLength;
	
    DWORD dwFormatFlags = FORMAT_MESSAGE_ALLOCATE_BUFFER |
        FORMAT_MESSAGE_IGNORE_INSERTS |
        FORMAT_MESSAGE_FROM_SYSTEM ;
	
    //
    // If dwLastError is in the network range, 
    //  load the message source.
    //
	
	//
    // Call FormatMessage() to allow for message 
    //  text to be acquired from the system 
    //  or from the supplied module handle.
    //
	
    if(dwBufferLength = FormatMessageA(
        dwFormatFlags,
        hModule, // module to get message from (NULL == system)
        dwLastError,
        MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), // default language
        (LPSTR) &MessageBuffer,
        0,
        NULL
        ))
    {
		AfxMessageBox(MessageBuffer);
		/*
        //
        // Output message string on stderr.
        //
        DWORD dwBytesWritten;
        WriteFile(
            GetStdHandle(STD_ERROR_HANDLE),
            MessageBuffer,
            dwBufferLength,
            &dwBytesWritten,
            NULL
            );
*/		
        //
        // Free the buffer allocated by the system.
        //
        LocalFree(MessageBuffer);
    }
	
    //
    // If we loaded a message source, unload it.
    //
}

LONG GetRegValue(HKEY key, LPCTSTR subkey, LPCTSTR name,LPTSTR retdata)
{
    HKEY hkey;
    LONG retval = RegOpenKeyEx(key, subkey, 0, KEY_QUERY_VALUE, &hkey);
    if (retval == ERROR_SUCCESS) {
        long datasize = MAX_PATH;
        TCHAR data[MAX_PATH];
		DWORD type=REG_SZ;
        RegQueryValueEx(hkey,name,NULL,&type, (LPBYTE)data, (LPDWORD)&datasize);
        _tcsncpy(retdata,data,MAX_PATH);
        RegCloseKey(hkey);
    }
	
    return retval;
}


CString Digest2String(unsigned char digest[16])
{
	CString str;
	char s[3];
	s[2]='\0';
	int t=0;
	for(int i=0;i<16;i++,t++)
	{
		sprintf(s,"%02x",digest[i]);
		str+=s;
	}
	return str;
}

UINT WorkThread(LPVOID param)
{
	WorkControl* thisWork=reinterpret_cast<WorkControl*>(param);
	CWorkQueue* pQueue=thisWork->m_pQueue;
	JobUnit* curJob;
	MD5* md5sess=new MD5;
	int jobIndex;
	int incSize=thisWork->m_PageSize;

#ifdef SHOW_TIME_LEFT	
	DWORD lastTickCount,curTickCount;
#endif

#ifdef USE_MEMORY_MAP
	LONGLONG numPages;
	LONGLONG length_read;
	LONGLONG file_size;
#else
	LONGLONG numPages,file_size;file_size;	// suppress warning msg
	unsigned int length_read;
#endif

	unsigned char* pbuf;
	unsigned char md5sum[16];
	int count;

#ifdef USE_MEMORY_MAP
	HANDLE hmap;
#endif

	while(1)
	{
		if(CANCELED==pQueue->GetStatus())
		{
			delete md5sess;
			return CANCELED;
		}
		curJob=pQueue->GetNextJob(jobIndex);
		if(DONE==pQueue->GetStatus() || NULL==curJob )
		{
			delete md5sess;
			return DONE;
		}

		if(curJob && CANCELED==curJob->GetStatus() || SKIPPED==curJob->GetStatus())
		{
			goto done;
		}

		curJob->SetStatus(RUNNING);
//		not work on WinXP,ft
		thisWork->m_pUpdater->m_pJobList->SetItemState(jobIndex,1,LVIS_SELECTED);
		thisWork->m_pUpdater->m_pJobList->SetItemState(jobIndex,1,LVIS_FOCUSED);
		thisWork->m_pUpdater->m_pJobList->SetHotItem(jobIndex);
		
#ifdef SHOW_TIME_LEFT		
		lastTickCount=GetTickCount();
#endif

#ifdef USE_MEMORY_MAP
		/* use Win32 Memory Map to do md5sum */
		hmap=CreateFileMapping((HANDLE)curJob->m_pOpenFile->m_hFile,NULL,
									PAGE_READONLY|SEC_COMMIT|SEC_NOCACHE,0,0,NULL);
		if(hmap==(HANDLE)NULL)
		{
			curJob->SetStatus(FAILURE);
			DisplayErrorText();
			goto done;
			ASSERT(0);		// need do more error reporting
		}
/*
		pbuf=(unsigned char*)MapViewOfFile(hmap,FILE_MAP_READ,0,0,0);
		if(NULL==pbuf)
		{
			DisplayErrorText();
			VERIFY(CloseHandle(hmap));
			curJob->SetStatus(FAILURE);
			goto done;
			ASSERT(0);
		}
*/
		file_size=curJob->m_pOpenFile->GetLength();
		numPages=file_size/incSize+((file_size%incSize)>0);
#endif	
		count=0;
		length_read=0;		
		md5sess->SessionInit();
#ifdef USE_MEMORY_MAP
		while(length_read+incSize <= file_size)
		{
			if(SKIPPED==curJob->GetStatus())
				break;
//			md5sess->SessionUpdate(pbuf+length_read,incSize);
//*
			pbuf=(unsigned char*)MapViewOfFile(hmap,FILE_MAP_READ,0,length_read,incSize);
			if(NULL==pbuf)
			{
				DisplayErrorText();
				VERIFY(CloseHandle(hmap));
				curJob->SetStatus(FAILURE);
				goto done;
				ASSERT(0);
			}
			md5sess->SessionUpdate(pbuf,incSize);
//*/
			length_read+=incSize;
			VERIFY(UnmapViewOfFile(pbuf));
			thisWork->m_pUpdater->SetNewPos(jobIndex,(count++)*100/numPages);
		}
#else
		int lastPos;
		int curPos;
		FILE* file;
		lastPos=-1;
		pbuf=new unsigned char[incSize];
		if ((file = fopen ((LPCTSTR)(curJob->filePath), "rb")) == NULL)
		{
			curJob->SetStatus(FAILURE);
			goto done;
		}

/*
		fseek(file,0,SEEK_END);
		file_size=ftell(file);
		fseek(file,0,SEEK_SET);
		numPages=file_size/incSize+((file_size%incSize)>0);
*/
		numPages=curJob->filePages;
		while (length_read = fread (pbuf, 1, incSize, file))
		{
			if(SKIPPED==curJob->GetStatus() || CANCELED==curJob->GetStatus())
				break;
			md5sess->SessionUpdate(pbuf,length_read);
			curPos=(count++)*100/numPages;
			if(curPos>lastPos)
			{
				thisWork->m_pUpdater->SetNewPos(jobIndex,curPos);
				lastPos=curPos;
			}
		}
		md5sess->SessionFinal(md5sum);
		fclose (file);
		thisWork->m_pUpdater->SetNewPos(jobIndex,100);

#endif		
done:	
#ifdef USE_MEMORY_MAP
		if(RUNNING==curJob->GetStatus())
		{
			pbuf=(unsigned char*)MapViewOfFile(hmap,FILE_MAP_READ,0,length_read,file_size-length_read);
			if(file_size>length_read)
//				md5sess->SessionUpdate(pbuf+length_read,file_size-length_read);
				md5sess->SessionUpdate(pbuf,file_size-length_read);
			md5sess->SessionFinal(md5sum);
			CString digest=Digest2String(md5sum);
			curJob->digest=digest;
			thisWork->m_pUpdater->m_pJobList->SetItemText(jobIndex,0,digest);
			if(""!=curJob->compareMd5)
			{
				if(curJob->compareMd5.Compare(digest))
					thisWork->m_pUpdater->m_pJobList->SetItemText(jobIndex,0,_T("Not Match"));
				else
					thisWork->m_pUpdater->m_pJobList->SetItemText(jobIndex,0,_T("Match"));				
			}
			curJob->SetStatus(DONE);
			VERIFY(UnmapViewOfFile(pbuf));
		}
		else 
#else
		if(RUNNING==curJob->GetStatus())
		{
			CString digest=Digest2String(md5sum);
			curJob->digest=digest;
			thisWork->m_pUpdater->m_pJobList->SetItemText(jobIndex,0,digest);
			curJob->SetStatus(DONE);			
			if(""!=curJob->compareMd5)
			{
				if(curJob->compareMd5.Compare(digest))
				{
					thisWork->m_pUpdater->m_pJobList->SetItemText(jobIndex,0,_T("Not Match"));
					curJob->SetStatus(NOTMATCH);
				}
				else
				{
					thisWork->m_pUpdater->m_pJobList->SetItemText(jobIndex,0,_T("Match"));				
					curJob->SetStatus(MATCH);
				}
			}
		}
		else
#endif
		if(SKIPPED==curJob->GetStatus())
		{
			thisWork->m_pUpdater->m_pJobList->SetItemText(jobIndex,0,"Skipped");
#ifdef USE_MEMORY_MAP
			//		VERIFY(UnmapViewOfFile(pbuf));
			VERIFY(CloseHandle(hmap));
#endif
		}
		else if(CANCELED==curJob->GetStatus())
		{
			thisWork->m_pUpdater->m_pJobList->SetItemText(jobIndex,0,"Canceled");
		}
		else if(FAILURE==curJob->GetStatus())
		{
			thisWork->m_pUpdater->m_pJobList->SetItemText(jobIndex,0,"Error");
		}
		thisWork->m_pUpdater->m_pJobList->Scroll(CSize(0,10));
		pQueue->IncFinishJobs();
		pQueue->IncDonePages(numPages);
		pQueue->Update();
		TRACE("%s,%d\n",curJob->filePath,curJob->status);
#ifdef SHOW_TIME_LEFT
		curTickCount=GetTickCount();
		curTickCount-=lastTickCount;
		if(curTickCount>10 && numPages>=2)
		{
			pQueue->TimeLeft((double)(curTickCount)/1000/numPages);
		}
#endif
	}
	
	return DONE;
}


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CWorkQueue::CWorkQueue()
{
	m_nFinishJobs=0;
	m_nNextJob=0;
	m_nTotPages=0;
	status=READY;
	m_pUpdater=NULL;
	m_pDlg=NULL;
	m_SysAllocPages=64*1024;		// default page size	
	InitializeCriticalSection(&sche_cs);
	InitializeCriticalSection(&stat_cs);
}

CWorkQueue::~CWorkQueue()
{
	// should do some cleanup
	int i;
	for(i=0;i<m_vecThreads.size();i++)
	{
		delete m_vecThreads[i];
	}
	for(i=0;i<m_vecJobs.size();i++)
	{
		TRACE(m_vecJobs[i]->filePath);
		delete m_vecJobs[i];
	}
	DeleteCriticalSection(&sche_cs);
	DeleteCriticalSection(&stat_cs);
}

bool CWorkQueue::Initialize(MyCommandLineInfo* cmdline,CWmd5Dlg* dlg)
{
	dlg->m_pQueue=this;
	m_pDlg=dlg;
	m_pDlg->SetWindowText(_T("Collecting files"));
	m_pUpdater=&dlg->m_QueueProgress;

	mode=cmdline->m_mode;
	
	CStdioFile stdf;
	if(""==cmdline->m_strFileName)		// should do more error checking
	{
		/*
		CFileDialog openfile(TRUE);
		openfile.DoModal();
		CFileException ex;
		if(FALSE==stdf.Open(openfile.GetFileName(),CFile::modeRead,&ex))
		{
			TCHAR* err=new TCHAR[1024];
			ex.GetErrorMessage(err,1024);
			AfxMessageBox(err);
			m_pDlg->SendMessage(WM_DESTROY);
		}
		*/
		return false;
	}
	else
	{
		CFileException ex;
		if(FALSE==stdf.Open(cmdline->m_strFileName,CFile::modeRead,&ex))
		{
			TCHAR* err=new TCHAR[1024];
			ex.GetErrorMessage(err,1024);
			AfxMessageBox(err);
			m_pDlg->SendMessage(WM_DESTROY);
		}
	}
	
	status=RUNNING;
	
	if(CHECKMD5==mode)
	{
		CString fpath=stdf.GetFilePath();
		fpath.Delete(fpath.ReverseFind(_T('\\'))+1,stdf.GetFileName().GetLength());
		m_szRootDir=fpath;
		FetchCompareJobs(stdf);
		stdf.Close();
	}
	else
	{
		FetchMd5Jobs(stdf);
		stdf.Close();
		// now delete the temp file
		DeleteFile(cmdline->m_strFileName);
	}

	if(CANCELED==status)
		return false;

#ifdef SHOW_TIME_LEFT	
	m_pTime=&dlg->m_TimeLeft;
#endif

	// determine multithread number
	int temp=1;
	TCHAR buf[MAX_PATH];
	if(ERROR_SUCCESS==GetRegValue(HKEY_LOCAL_MACHINE,_T("SOFTWARE\\EasyMD5"),_T("MT"),buf))
	{
		temp=_ttoi(buf);
		if(temp<=0)
			temp=1;
	}
	// determine alloc page size
	if(ERROR_SUCCESS==GetRegValue(HKEY_LOCAL_MACHINE,_T("SOFTWARE\\EasyMD5"),_T("PageSize"),buf))
	{
		m_SysAllocPages=_ttoi(buf);
		if(m_SysAllocPages<=0 || m_SysAllocPages>=1024 || 0!=m_SysAllocPages%4)
			m_SysAllocPages=64*1024;
		else
			m_SysAllocPages*=1024;
	}

	
/*
	CString s;
	s.Format("%d,%d",temp,m_SysAllocPages);
	AfxMessageBox(s);
*/

	WorkControl* w;
	for(int i=0;i<temp;i++)
	{
		w=new WorkControl;
		w->m_pUpdater->m_pJobList=&dlg->m_JobList;
		w->m_pThread=AfxBeginThread(&WorkThread,reinterpret_cast<LPVOID>(w),THREAD_PRIORITY_NORMAL,0,CREATE_SUSPENDED);
		w->m_pThread->m_bAutoDelete=FALSE;	// if set TRUE, error will occur on MultiCore/HT machines
		w->m_pQueue=this;
		w->m_PageSize=m_SysAllocPages;
		m_vecThreads.push_back(w);
	}
	return true;
}

BOOL CWorkQueue::FetchCompareJobs(CStdioFile& stdf)
{
	CString szBuf;
	CString str;
	CFileFindEx finder;
	while(stdf.ReadString(szBuf))
	{
		if(CANCELED==status)
		{
			finder.Close();
			return FALSE;
		}

		// Linux md5sum text type support 
		if(32==szBuf.Find(_T(" *")) || 32==szBuf.Find(_T("  ")))
		{
			str=szBuf;
			str.Delete(0,34);

			JobUnit* ju;
			ju=new JobUnit;

			ju->compareMd5=szBuf.Left(32);

			if(m_szRootDir!=_T("") && _T(':')==str[1] && _T('\\')==str[2])
			{
				m_szRootDir=_T("");
			}
			ju->fileName=str.Right(str.GetLength() - str.ReverseFind(_T('\\')) -1);
			ju->filePath=m_szRootDir+str;

			LONGLONG file_size=0;
/*
			CFile of;
			CFileException ex;
			if(FALSE==of.Open(ju->filePath,CFile::modeRead,&ex))
			{
				ju->status=FAILURE;
				TCHAR* err=new TCHAR[1024];
				ex.GetErrorMessage(err,1024);
				AfxMessageBox(err);
			}
			else
			{
				LONGLONG size=of.GetLength();
				if(size)		
				{
					file_size=size;			// cast 64 bits
//					ju->m_pOpenFile=of;
				}
				else	// reject zero length file
				{
					ju->status=SKIPPED;
				}
				of.Close();
			}
*/
			if(TRUE==finder.FindFile(ju->filePath))
			{
				finder.FindNextFile();
				if(! finder.IsDots() && ! finder.IsDirectory())
				{
					finder.GetLength64(file_size);

					/*	skiping zero length file is not good as it seems
					if(0==file_size)	
						ju->status=SKIPPED;
					*/
				}
				else
				{
					file_size=0;
					ju->status=FAILURE;
				}
			}
			else
			{
				file_size=0;
				ju->status=FAILURE;
			}

			ju->filePages=file_size/m_SysAllocPages + (file_size%m_SysAllocPages > 0);
			m_nTotPages+=ju->filePages;
			m_pDlg->m_JobList.InsertItem(m_vecJobs.size(),_T("Ready"));
			m_pDlg->m_JobList.SetItemText(m_vecJobs.size(),1,ju->fileName);
			m_pDlg->m_JobList.SetItemData(m_vecJobs.size(),(DWORD)_tcsdup((LPCTSTR)ju->filePath));
			m_vecJobs.push_back(ju);
		}
	}
	finder.Close();
	return TRUE;
}

BOOL CWorkQueue::FetchMd5Jobs(CStdioFile& stdf)
{
	BOOL driverpath=FALSE;
	CString str;
	CString szBuf;
	CFileFindEx finder;
	m_szRootDir="";			// should detect the root dir
	while(stdf.ReadString(szBuf))
	{
		if(CANCELED==status)
			return FALSE;

		int index=szBuf.ReverseFind(_T('\\'));
		if(index==(szBuf.GetLength()-1))
		{
			driverpath=TRUE;
			if(!FetchJobs(szBuf))
			{
				ASSERT(0);
			}
			continue;
		}
		else if(""==m_szRootDir)
			m_szRootDir=szBuf.Left(index+1);
		/* CFileFinder has a buffer overrun bug, trim szBuf at MAX_PATH */
		/* a potential buffer overflow attack way*/
		/* think of debugging an application to see if it uses FileFinder */
		if(_tcslen(szBuf)>=MAX_PATH)
			szBuf.SetAt(MAX_PATH-1,_T('\0'));
		if(FALSE==finder.FindFile(szBuf))
			continue;
		finder.FindNextFile();
		ASSERT(!finder.IsDots());
		if(finder.IsDirectory())
		{
			if(!FetchJobs(finder.GetFilePath()))
			{
				ASSERT(0);
			}
		}
		else		// add new file to job queue
		{
			LONGLONG file_size;
			JobUnit* ju;
			ju=new JobUnit;
			ju->fileName=finder.GetFileName();
			ju->filePath=finder.GetFilePath();
			finder.GetLength64(file_size);
			ju->filePages=file_size/m_SysAllocPages + (file_size%m_SysAllocPages > 0);
			m_nTotPages+=ju->filePages;
			m_pDlg->m_JobList.InsertItem(m_vecJobs.size(),"Ready");
			m_pDlg->m_JobList.SetItemText(m_vecJobs.size(),1,ju->fileName);
			m_pDlg->m_JobList.SetItemData(m_vecJobs.size(),(DWORD)_tcsdup((LPCTSTR)ju->filePath));
			m_vecJobs.push_back(ju);
			finder.Close();
		}
	}
	return TRUE;
}

BOOL CWorkQueue::FetchJobs(CString& dir)
{
	CFileFindEx finder;
	CString strWildcard = dir;
	strWildcard+=_T("\\*");

	// start working for files
	BOOL bWorking = finder.FindFile(strWildcard);
	CString str;
	while (bWorking)
	{
		if(CANCELED==status)
			return FALSE;

		bWorking = finder.FindNextFile();
		
		// skip . and .. files; otherwise, we'd
		// recur infinitely!
		
		if (finder.IsDots())
			continue;
		
		// if it's a directory, recursively search it
		
		if (finder.IsDirectory())
		{
			str = finder.GetFilePath();
			FetchJobs(str);
		}
		else		// add new file to job queue
		{
			LONGLONG file_size;
			JobUnit* ju;
			ju=new JobUnit;
			ju->fileName=finder.GetFileName();
			ju->filePath=finder.GetFilePath();
			finder.GetLength64(file_size);
			ju->filePages=file_size/m_SysAllocPages + (file_size%m_SysAllocPages > 0);
			m_nTotPages+=ju->filePages;
			m_pDlg->m_JobList.InsertItem(m_vecJobs.size(),"Ready");
			m_pDlg->m_JobList.SetItemText(m_vecJobs.size(),1,ju->fileName);
			m_pDlg->m_JobList.SetItemData(m_vecJobs.size(),(DWORD)_tcsdup((LPCTSTR)ju->filePath));
			m_vecJobs.push_back(ju);
		}
	}	
	finder.Close();
	return TRUE;
}

/*
	very simple scheduling 
	more advanced scheduler requires synchronization among all the threads
*/
JobUnit* CWorkQueue::GetNextJob(int &index)
{
	EnterCriticalSection(&sche_cs);
	index=m_nNextJob;
	LeaveCriticalSection(&sche_cs);
	
	if(m_nNextJob<m_vecJobs.size()) // && status!=DONE && status!=CANCELED) seems useless
		return m_vecJobs[m_nNextJob++];
	else
		return NULL;
}

void CWorkQueue::Run()
{
	m_pUpdater->SetRange(0,m_vecJobs.size());	
//	m_pUpdater->SetRange(0,m_nTotPages);
	for(int i=0;i<m_vecThreads.size();i++)
		m_vecThreads[i]->m_pThread->ResumeThread();
	CString str;
	str.Format(_T("Completed: %d/%d"),m_nFinishJobs,m_vecJobs.size());
	m_pDlg->SetWindowText(str);
#ifdef SHOW_TIME_LEFT	
	TimeLeft(0.05);		// experience parameter
#endif
}

void CWorkQueue::Update()
{
#if 0	
	if(FINISH==status)		// obsolete state
		return;
#endif
	if(DONE==GetStatus() || CANCELED==GetStatus())
		return;
	if(m_nFinishJobs==m_vecJobs.size())
		SetStatus(DONE);
	m_pUpdater->SetPos(m_nFinishJobs);
	CString str;
	str.Format(_T("Completed: %d/%d"),m_nFinishJobs,m_vecJobs.size());
	m_pDlg->SetWindowText(str);
#if 0
	if(status==DONE || status==CANCELED)
	{
		status=FINISH;
#ifdef SHOW_TIME_LEFT
		m_pTime->SetWindowText(_T("0:0:0"));
#endif
		return;
	}
#endif
}

void CWorkQueue::DoSaveResult()
{
	if(CHECKMD5==mode)
		return;
	if(0==m_vecJobs.size())	// skip if no jobs
		return;
	OPENFILENAME ofn;
	TCHAR szFile[260] = _T("md5sum.txt\0");      // buffer for file name
	// Initialize OPENFILENAME
	ZeroMemory(&ofn, sizeof(OPENFILENAME));
	ofn.lStructSize = sizeof(OPENFILENAME);
	ofn.lpstrTitle = _T("Save Md5sum Result As");
	ofn.hwndOwner = m_pDlg->m_hWnd;
	ofn.lpstrFile = szFile;
	ofn.nMaxFile = sizeof(szFile);
	ofn.lpstrFilter = _T("All\0*.*\0Text File\0*.txt\0MD5 File\0*.md5\0");
	ofn.nFilterIndex = 2;
	ofn.lpstrInitialDir = m_szRootDir;
	ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST | OFN_OVERWRITEPROMPT | OFN_SHOWHELP;
	// Display the Open dialog box. 
	if (GetSaveFileName(&ofn)==TRUE) 
	{
		try
		{
			CStdioFile outfile(ofn.lpstrFile,CFile::modeWrite|CFile::modeCreate);
			CString str;
			CString filePath;
			for(int i=0;i<m_vecJobs.size();i++)
			{
				if(DONE==m_vecJobs[i]->GetStatus())
				{
					str=m_vecJobs[i]->digest;
					str+=CString(_T(" *"));
					filePath=m_vecJobs[i]->filePath;
					if(""!=m_szRootDir)
						filePath.Delete(0,m_szRootDir.GetLength());
					str+=filePath;
					outfile.WriteString(str+CString(_T("\n")));
				}
					
			}
			outfile.Close();
		}
		catch (CFileException* p) 
		{
			p->ReportError();
			p->Delete();
		}
		catch(CException* e)
		{
			e->ReportError();
			e->Delete();
		}
		AfxMessageBox(CString(ofn.lpstrFile)+CString(_T(" saved successfully")));
	}
}

#ifdef SHOW_TIME_LEFT
void CWorkQueue::TimeLeft(double speed)
{
	unsigned donePages=m_nDonePages;
	if(DONE==status || FINISH==status)
	{
		m_pTime->SetWindowText(_T("0:0:0"));
		return;
	}
	int timeLeft=speed*(m_nTotPages-donePages);
	int hoursLeft=timeLeft/3600;
	int minsLeft=(timeLeft-hoursLeft*3600)/60;
	int secsLeft=timeLeft-hoursLeft*3600-minsLeft*60;
	CString str;
	str.Format("%d:%d:%d",hoursLeft,minsLeft,secsLeft);
	m_pTime->SetWindowText(str);
}
#endif

void CWorkQueue::Wait()
{
	HANDLE* waitArray=new HANDLE[m_vecThreads.size()];
	for(int i=0;i<m_vecThreads.size();i++)
		waitArray[i]=m_vecThreads[i]->m_pThread->m_hThread;
	//if CWinThread::m_bAutoDelete is set true, error will occur on MultiCore/HT machines
	//when only 1 job is active and other threads kill themselves, causing handle invalid
	DWORD retVal=WaitForMultipleObjects(m_vecThreads.size(),waitArray,TRUE,INFINITE);
	if(WAIT_OBJECT_0 != retVal)
		DisplayErrorText();
}

void CWorkQueue::ShowResult()
{
	int succ=0,errors=0,matches=0,notmatches=0;
	int i;
	if(CHECKMD5==mode)
	{
		for(i=0;i<m_vecJobs.size();i++)
		{
			if(MATCH==m_vecJobs[i]->status)
			{
				matches++;
				succ++;
			}
			else if(NOTMATCH==m_vecJobs[i]->status)
			{
				notmatches++;
				succ++;
			}
			else
			{
				/*
				CString s;
				s.Format("%s,%d",m_vecJobs[i]->filePath,m_vecJobs[i]->status);
				AfxMessageBox(s);
				*/
				errors++;
			}
		}
		CString s1,s2,s3,s4;
		s1.Format(_T("%d"),succ);
		s2.Format(_T("%d"),errors);
		s3.Format(_T("%d"),matches);
		s4.Format(_T("%d"),notmatches);
		m_pDlg->ShowResult(s1,s2,s3,s4);
	}
	else
	{                                                                                           
		for(i=0;i<m_vecJobs.size();i++)
		{
			if(DONE==m_vecJobs[i]->status)
			{
				succ++;
			}
			else
			{
				errors++;
			}
		}
		CString s1,s2;
		s1.Format(_T("%d"),succ);
		s2.Format(_T("%d"),errors);
		m_pDlg->ShowResult(s1,s2);
	}
}

void CWorkQueue::SuspendWork()
{
	for(int i=0;i<m_vecThreads.size();i++)
		SuspendThread(m_vecThreads[i]->m_pThread->m_hThread);
}

void CWorkQueue::ResumeWork()
{
	for(int i=0;i<m_vecThreads.size();i++)
		ResumeThread(m_vecThreads[i]->m_pThread->m_hThread);
}
