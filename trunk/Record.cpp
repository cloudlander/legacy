// Record.cpp: implementation of the CRecord class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Record.h"
#include <process.h>
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CRecord::CRecord()
{
	waveFormat.wFormatTag = WAVE_FORMAT_PCM;
	waveFormat.cbSize = 0;
	waveFormat.nChannels = 2;
	waveFormat.nSamplesPerSec = 44100;
	waveFormat.wBitsPerSample = 16;
	waveFormat.nBlockAlign = waveFormat.nChannels * (waveFormat.wBitsPerSample/8);
	waveFormat.nAvgBytesPerSec = waveFormat.nSamplesPerSec * waveFormat.nBlockAlign;

	m_pBuffer = NULL;
	pWaveHeader = NULL;
	WaveInHandle = NULL;
	m_nBufSize = 1024;
	m_nBufs = 3;
	DeviceID = WAVE_MAPPER;
}

CRecord::CRecord(int nDeviceID,int nChannels,int nSamplePerSecond,int nBitsPerSample)
{
	waveFormat.wFormatTag = WAVE_FORMAT_PCM;
	waveFormat.cbSize = 0;
	waveFormat.nChannels = nChannels;
	waveFormat.nSamplesPerSec = nSamplePerSecond;
	waveFormat.wBitsPerSample = nBitsPerSample;
	waveFormat.nBlockAlign = waveFormat.nChannels * (waveFormat.wBitsPerSample/8);
	waveFormat.nAvgBytesPerSec = waveFormat.nSamplesPerSec * waveFormat.nBlockAlign;

	m_pBuffer = NULL;
	pWaveHeader = NULL;
	WaveInHandle = NULL;
	m_nBufSize = 1024;
	m_nBufs = 3;
	DeviceID = nDeviceID;
}

CRecord::~CRecord()
{
	if(pWaveHeader)
		delete[] pWaveHeader;
	if(m_pBuffer)
		VirtualFree(m_pBuffer, 0, MEM_RELEASE);
}

int CRecord::GetDeviceCount()
{
//	return mixerGetNumDevs();	
	MMRESULT err;
	HMIXER	MixerHandle;
	MIXERLINE	mixerLine;
	if(!Open(0))
		return -1;
	err = mixerOpen(&MixerHandle, (DWORD)WaveInHandle, 0, 0, MIXER_OBJECTF_HWAVEIN);
	if(err)
	{
		ReportError("Device does not have mixer support!",err);
		Close();
		return -1;
	}
	mixerLine.cbStruct = sizeof(MIXERLINE);
	mixerLine.dwComponentType = MIXERLINE_COMPONENTTYPE_DST_WAVEIN;
	err = mixerGetLineInfo((HMIXEROBJ)MixerHandle, &mixerLine, MIXER_GETLINEINFOF_COMPONENTTYPE);
	if (err)
	{
		ReportError("Device does not have a WAVE recording control!", err);
		Close();
		return -1;
	}
	if(!Close())
		return -1;
	return mixerLine.cConnections;
}

BOOL CRecord::Open(DWORD dwCallback)
{
	MMRESULT err;
	DWORD fdwOpen=CALLBACK_NULL;
	if(0 != dwCallback)
		fdwOpen = CALLBACK_THREAD;
	err = waveInOpen(&WaveInHandle, WAVE_MAPPER, &waveFormat,dwCallback, 0,fdwOpen);
	if (err)
	{
		ReportError("Can't open WAVE In Device!",err);
		return FALSE;
	}
	return TRUE;
}

BOOL CRecord::Close()
{
	int FirstReport=0;
	MMRESULT err;
	do
	{
		err = waveInClose(WaveInHandle);
		if (err) 
		{
			if(0 == FirstReport)
				ReportError("Can't close WAVE In Device!",err);
			else if(1000 == FirstReport)
				return FALSE;
		}
		FirstReport++;
	} while (err);
	return TRUE;
}
BOOL CRecord::Init()
{
	MMRESULT err;
	unsigned int ThreadID;
	if(!PreInit())
	{
		ReportError("Error preinit");
		return FALSE;
	}
	if(!SetBuffer())
	{
		ReportError("Error setting buffer");
		return FALSE;
	}
//*
	if(0==_beginthreadex(NULL,0,&waveInProc,(void*)this,0,&ThreadID))
	{
		ReportError("Error creating thread");
		return FALSE;
	}
//*/
	if(0==_beginthreadex(NULL,0,&waveDataProc,(void*)this,0,&wDPThreadID))
	{
		ReportError("Error creating thread");
		return FALSE;
	}
	if(0==ThreadID || !Open(ThreadID))
		return FALSE;
	pWaveHeader = new WAVEHDR[m_nBufs];
	if(NULL == pWaveHeader)
	{
		ReportError(ERR_MEMORY);
		return FALSE;
	}
	memset(pWaveHeader,0,m_nBufs*sizeof(WAVEHDR));
	m_pBuffer = (BYTE*)VirtualAlloc(0, m_nBufSize*m_nBufs, MEM_COMMIT, PAGE_READWRITE);
	if(NULL == m_pBuffer)
	{
		ReportError(ERR_MEMORY);
		return FALSE;
	}
	for(int i=0;i<m_nBufs;i++)
	{
		pWaveHeader[i].dwBufferLength = m_nBufSize;
		pWaveHeader[i].lpData = (char*)m_pBuffer + i * m_nBufSize;
		pWaveHeader[i].dwUser = i;
		err = waveInPrepareHeader(WaveInHandle, &pWaveHeader[i], sizeof(WAVEHDR));
		if(err)
		{
			ReportError("Error preparing WAVEHDR",err);
			return FALSE;
		}
		err = waveInAddBuffer(WaveInHandle, &pWaveHeader[i], sizeof(WAVEHDR));
		if(err)
		{
			ReportError("Error queueing WAVEHDR",err);
			return FALSE;
		}
	}
	return TRUE;
}

BOOL CRecord::Start()
{
	MMRESULT err;
	m_bInRecord = TRUE;
	err = waveInStart(WaveInHandle);
	if(err)
	{
		ReportError("Error starting record",err);
		return FALSE;
	}
	return TRUE;
}

BOOL CRecord::Stop()
{
	MMRESULT err;
	m_bInRecord = FALSE;	
	err = waveInReset(WaveInHandle);
	if(err)
	{
		ReportError("Error resetting ",err);
		return FALSE;
	}
	while(m_nDone < m_nBufs)
		Sleep(0);
	if(!PostStop())
	{
		ReportError("Error poststop");
		return FALSE;
	}
	for(int i=0;i<m_nBufs;i++)
	{
		err = waveInUnprepareHeader(WaveInHandle, &pWaveHeader[i], sizeof(WAVEHDR));
		if(err)
		{
			ReportError("Error unpreparing WAVEHDR",err);
			return FALSE;
		}
	}
	if(!Close())
		return FALSE;
	return TRUE;
}

void CRecord::ReportError(int errCode)
{
#ifndef _NOERROR
	TCHAR errText[256];
	switch(errCode)
	{
	case ERR_MEMORY:
		_tcscpy(errText,"Memory not available");
		break;
	case ERR_HANDLE:
		_tcscpy(errText,"Handle not valid");
		break;
	default:
		_tcscpy(errText,"Unknown Error");
		break;
	}
	MessageBox(NULL,errText,"Error",MB_OK|MB_ICONSTOP|MB_APPLMODAL);
#endif
}

void CRecord::ReportError(TCHAR* msg,int errCode)
{
#ifndef _NOERROR
	TCHAR* errText = new TCHAR[_tclen(msg)+1];
	_tcscpy(errText,msg);
	MessageBox(NULL,errText,"Error",MB_OK|MB_ICONSTOP|MB_APPLMODAL);
	delete errText;
#endif
}

unsigned WINAPI CRecord::waveInProc(LPVOID arg)
{
	CRecord* pThis = reinterpret_cast<CRecord*>(arg);
	MSG		msg;
	LPWAVEHDR pHdr;
	BOOL bLastBlock=FALSE;
//	MMRESULT err;
//	MessageBox(HWND_DESKTOP,"In thread","Test",MB_OK);
	while (GetMessage(&msg, 0, 0, 0) == 1)
	{
		switch (msg.message)
		{
			/* A buffer has been filled by the driver */
			case MM_WIM_DATA:
			{
				pHdr = (WAVEHDR*)msg.lParam;
/*
				if(NULL == pHdr)
					pThis->ReportError(ERR_HANDLE);
				err = waveInUnprepareHeader(pThis->WaveInHandle,pHdr,sizeof(WAVEHDR));
				if(err)
					pThis->ReportError("Error in Recording(UnprepareHeader)",err);
*/
/*				if(pHdr->dwBytesRecorded)
				{
					if(!pThis->m_bInRecord )
						bLastBlock=TRUE;
					if(!pThis->ProcessData((BYTE*)pHdr->lpData,pHdr->dwBytesRecorded,bLastBlock))
						pThis->ReportError("Error in processing data");
				}
*/
				if (pThis->m_bInRecord)
				{
/*
					err = waveInPrepareHeader(pThis->WaveInHandle,pHdr,sizeof(WAVEHDR));
					if(err)
						pThis->ReportError("Error in Recording(UnprepareHeader)",err);
					else
*/
						pThis->m_nCurBuf=pHdr->dwUser;
						PostThreadMessage(pThis->wDPThreadID,WM_DATA_READY,0,pThis->m_nCurBuf);	
						waveInAddBuffer(pThis->WaveInHandle,pHdr,sizeof(WAVEHDR));
				}
				else
					pThis->m_nDone++;
				break;
			}
			case MM_WIM_OPEN:
			{
				pThis->m_nDone = 0;
				pThis->m_nCurBuf=0;
				break;
			}
			case MM_WIM_CLOSE:
			{
				PostThreadMessage(pThis->wDPThreadID,WM_FINISH,0,0);
				return 0;
			}
		}
	}
	return 0;
}

unsigned WINAPI CRecord::waveDataProc(LPVOID arg)
{
	CRecord* pThis = reinterpret_cast<CRecord*>(arg);
	MSG		msg;
	int i;
	while (GetMessage(&msg, 0, 0, 0) == 1)
	{
		switch(msg.message)
		{
		case WM_DATA_READY:
			{
				i=(UINT)msg.lParam;
				pThis->ProcessData(pThis->m_pBuffer+i*pThis->m_nBufSize,(pThis->pWaveHeader[i]).dwBytesRecorded,FALSE);
				break;
			}
		case WM_FINISH:
			return 0;
		default:
			break;
		}
	}
	return 0;
}

CMp3Record::CMp3Record(int nDeviceID,int nChannels,int nSamplePerSecond,int nBitsPerSample,int nBitRate,BOOL bVBR,TCHAR* szFile)
						:CRecord(nDeviceID,nChannels,nSamplePerSecond,nBitsPerSample)
{
	OutFileHandle = INVALID_HANDLE_VALUE;
	m_pMp3Buffer = NULL;
	hDLL=NULL;
	m_szFileName = szFile;
	memset(&beConfig,0,sizeof(beConfig));					// clear all fields
	// use the LAME config structure
	beConfig.dwConfig = BE_CONFIG_LAME;
	beConfig.format.LHV1.dwStructVersion	= 1;
	beConfig.format.LHV1.dwStructSize		= sizeof(beConfig);		
	beConfig.format.LHV1.dwSampleRate		= nSamplePerSecond;		// INPUT FREQUENCY
	beConfig.format.LHV1.dwReSampleRate		= 0;					// DON"T RESAMPLE
	beConfig.format.LHV1.nMode				= nChannels == 2 ? BE_MP3_MODE_JSTEREO : BE_MP3_MODE_MONO;	
																	// OUTPUT IN STREO
	beConfig.format.LHV1.dwBitrate			= nBitRate;				// MINIMUM BIT RATE
	beConfig.format.LHV1.nPreset			= LQP_R3MIX;			// QUALITY PRESET SETTING
	beConfig.format.LHV1.dwMpegVersion		= MPEG1;				// MPEG VERSION (I or II)
	beConfig.format.LHV1.dwPsyModel			= 0;					// USE DEFAULT PSYCHOACOUSTIC MODEL 
	beConfig.format.LHV1.dwEmphasis			= 0;					// NO EMPHASIS TURNED ON
	beConfig.format.LHV1.bOriginal			= TRUE;					// SET ORIGINAL FLAG
//	beConfig.format.LHV1.dwMaxBitrate		= 320;					// MAXIMUM BIT RATE
//	beConfig.format.LHV1.bCRC				= TRUE;					// INSERT CRC
//	beConfig.format.LHV1.bCopyright			= TRUE;					// SET COPYRIGHT FLAG	
//	beConfig.format.LHV1.bPrivate			= TRUE;					// SET PRIVATE FLAG
	beConfig.format.LHV1.bWriteVBRHeader	= FALSE;					// YES, WRITE THE XING VBR HEADER
	beConfig.format.LHV1.bEnableVBR			= bVBR;					// USE VBR
	beConfig.format.LHV1.nVBRQuality		= 5;					// SET VBR QUALITY
	beConfig.format.LHV1.bNoRes				= TRUE;					// No Bit resorvoir
// Preset Test
//	beConfig.format.LHV1.nPreset			= LQP_PHONE;
}

CMp3Record::~CMp3Record()
{
	if (OutFileHandle != INVALID_HANDLE_VALUE) 
	{
		CloseHandle(OutFileHandle);
		OutFileHandle = INVALID_HANDLE_VALUE;
	}
	if(m_pMp3Buffer)
		VirtualFree(m_pMp3Buffer, 0, MEM_RELEASE);
#ifdef _MULT_THREAD
	if(m_pHandles)
		delete[] m_pHandles;
#endif
}

BOOL CMp3Record::PreInit()
{
	OutFileHandle = CreateFile(m_szFileName,GENERIC_WRITE, 0, 0, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, 0);
	if(INVALID_HANDLE_VALUE == OutFileHandle)
	{
		ReportError(ERR_HANDLE);
		return FALSE;
	}
	hDLL = LoadLibrary("lame_enc.dll");
	if( NULL == hDLL )
	{
		ReportError("Error loading lame_enc.DLL");
		return FALSE;
	}

	// Get Interface functions from the DLL
	beInitStream	= (BEINITSTREAM) GetProcAddress(hDLL, TEXT_BEINITSTREAM);
	beEncodeChunk	= (BEENCODECHUNK) GetProcAddress(hDLL, TEXT_BEENCODECHUNK);
	beDeinitStream	= (BEDEINITSTREAM) GetProcAddress(hDLL, TEXT_BEDEINITSTREAM);
	beCloseStream	= (BECLOSESTREAM) GetProcAddress(hDLL, TEXT_BECLOSESTREAM);
	beVersion		= (BEVERSION) GetProcAddress(hDLL, TEXT_BEVERSION);
	beWriteVBRHeader= (BEWRITEVBRHEADER) GetProcAddress(hDLL,TEXT_BEWRITEVBRHEADER);
	beWriteInfoTag  = (BEWRITEINFOTAG) GetProcAddress(hDLL,TEXT_BEWRITEINFOTAG);

	// Check if all interfaces are present
	if(!beInitStream || !beEncodeChunk || !beDeinitStream || !beCloseStream || !beVersion || !beWriteVBRHeader)
	{
		ReportError("Unable to get LAME interfaces");
		return FALSE;
	}
	// Get the version number
//	beVersion( &Version );
	return TRUE;
}

BOOL CMp3Record::SetBuffer()
{
	// Init the MP3 Stream
	DWORD dwSamples;
	DWORD dwMP3Buffer;
	err = beInitStream(&beConfig, &dwSamples, &dwMP3Buffer, &hbeStream);
	// Check result
	if(err != BE_ERR_SUCCESSFUL)
	{
		ReportError("Error opening encoding stream ", err);
		return FALSE;
	}
	m_nMp3BufSize = dwMP3Buffer;
	m_nNSamples = 100;
	m_nSamples = dwSamples;
	m_nBufSize = 2 * dwSamples * m_nNSamples;
	m_nBufs = 10;
#ifdef _MULT_THREAD
	m_pMp3Buffer = (BYTE*)VirtualAlloc(0, dwMP3Buffer*m_nNSamples, MEM_COMMIT, PAGE_READWRITE);
#else
	m_pMp3Buffer = (BYTE*)VirtualAlloc(0, dwMP3Buffer, MEM_COMMIT, PAGE_READWRITE);
#endif
	if(NULL == m_pMp3Buffer)
	{
		ReportError(ERR_MEMORY);
		return FALSE;
	}

#ifdef _MULT_THREAD
	m_pHandles = new HANDLE[m_nNSamples/10];
	for(int i=0;i<m_nNSamples/10;i++)
	{
		m_pHandles[i]=CreateEvent(NULL,FALSE,TRUE,NULL);
		if(NULL == m_pHandles[i])
		{
			ReportError(ERR_HANDLE);
			return FALSE;
		}
	}
	m_pThreadID = new unsigned int[m_nNSamples/10];
	for(i=0;i<m_nNSamples/10;i++)
	{
		if(0==_beginthreadex(NULL,0,&DataProc,(void*)this,0,&m_pThreadID[i]))
		{
			ReportError("Error creating thread");
			return FALSE;
		}
	}
#endif
	return TRUE;
}

BOOL CMp3Record::PostStop()
{
	DWORD dwWrite;
	DWORD dwEncoded;
	err = beDeinitStream(hbeStream,m_pMp3Buffer, &dwEncoded);
	// Check result
	if(err != BE_ERR_SUCCESSFUL)
	{
		beCloseStream(hbeStream);
		ReportError("beExitStream failed", err);
		return FALSE;
	}
	// Are there any bytes returned from the DeInit call?
	// If so, write them to disk
	if( dwEncoded )
	{
		if(!WriteFile(OutFileHandle,m_pMp3Buffer,dwEncoded,&dwWrite,0) ||
			dwWrite != dwEncoded)
		{
			ReportError("Output file write error");
			return FALSE;
		}
	}
	// close the MP3 Stream
	beCloseStream( hbeStream );
	if (OutFileHandle != INVALID_HANDLE_VALUE) 
	{
		CloseHandle(OutFileHandle);
		OutFileHandle = INVALID_HANDLE_VALUE;
	}
#ifdef _MULT_THREAD
	for(int i=0;i<m_nNSamples/10;i++)
		PostThreadMessage(m_pThreadID[i],WM_MP3_FINISH,0,0);
#endif
	return TRUE;
}

BOOL CMp3Record::ProcessData(BYTE* data,int size,BOOL bLastBlock)
{
	DWORD dwWrite;
	int Offset=2*m_nSamples;
#ifdef _MULT_THREAD
	dwEncoded = 0;
	for(int i=0;i<size/Offset/10 ;i++)
	{
		ResetEvent(m_pHandles[i]);
		PostThreadMessage(m_pThreadID[i],WM_MP3_DATA_READY,0,i);
	}
	WaitForMultipleObjects(i,m_pHandles,TRUE,INFINITE);
	if(!WriteFile(OutFileHandle,m_pMp3Buffer,dwEncoded,&dwWrite,0) ||
		dwWrite != dwEncoded)
	{
		ReportError("Output file write error");
		return FALSE;
	}
#else
	for(int i=0;i<size/Offset;i++)
	{
		err = beEncodeChunk(hbeStream, m_nSamples, (PSHORT)(data+i*Offset), m_pMp3Buffer, &dwEncoded);
		// Check result
		if(err != BE_ERR_SUCCESSFUL)
		{
			beCloseStream(hbeStream);
			ReportError("beEncodeChunk() failed", err);
			return FALSE;
		}
		if(!WriteFile(OutFileHandle,m_pMp3Buffer,dwEncoded,&dwWrite,0) ||
			dwWrite != dwEncoded)
		{
			ReportError("Output file write error");
			return FALSE;
		}
	}
#endif
	return TRUE;
}

#ifdef _MULT_THREAD
unsigned WINAPI CMp3Record::DataProc(LPVOID arg)
{
	CMp3Record* pThis = reinterpret_cast<CMp3Record*>(arg);
	BE_ERR		err;

	MSG		msg;
	DWORD Encoded;
	int i;
	int Offset=2*pThis->m_nSamples;
	while (GetMessage(&msg, 0, 0, 0) == 1)
	{
		switch(msg.message)
		{
		case WM_MP3_DATA_READY:
			{
				i=(int)msg.lParam;
				for(int j=0;j<10;j++)
				{
					err = pThis->beEncodeChunk(pThis->hbeStream, pThis->m_nSamples, 
					(PSHORT)(pThis->m_pBuffer+(i*10+j)*Offset), 
					pThis->m_pMp3Buffer+(i*10+j)*pThis->m_nMp3BufSize, &Encoded);
					// Check result
					if(err != BE_ERR_SUCCESSFUL)
					{	
						pThis->beCloseStream(pThis->hbeStream);
						pThis->ReportError("beEncodeChunk() failed", err);
					}
					pThis->dwEncoded=pThis->dwEncoded+Encoded;
				}
				SetEvent(pThis->m_pHandles[i]);
				break;
			}
		case WM_MP3_FINISH:
			return 0;
		default:
			break;
		}
	}
	return 0;
}
#endif

//////////////////////////////////////////////////////////////////////
// CWaveRecord Class
//////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CWaveRecord::CWaveRecord(int nDeviceID,int nChannels,int nSamplePerSecond,int nBitsPerSample,TCHAR* szFile)
						:CRecord(nDeviceID,nChannels,nSamplePerSecond,nBitsPerSample)
{
	m_szFileName = szFile;
	ZeroMemory(&m_MMCKInfoParent,sizeof(MMCKINFO));
	ZeroMemory(&m_MMCKInfoChild,sizeof(MMCKINFO));
	m_hFile = NULL;
}

CWaveRecord::~CWaveRecord()
{
	if(m_hFile)
	{
		::mmioAscend(m_hFile, &m_MMCKInfoChild, 0);
		::mmioAscend(m_hFile, &m_MMCKInfoParent, 0);
		::mmioClose(m_hFile, 0);
		m_hFile = NULL;
	}	
}

BOOL CWaveRecord::PreInit()
{
	if(m_hFile) 
		return FALSE;	
	
	// open file
	m_hFile = ::mmioOpen(m_szFileName,NULL, MMIO_CREATE|MMIO_WRITE|MMIO_EXCLUSIVE | MMIO_ALLOCBUF);
	if(m_hFile == NULL) 
	{
		ReportError(ERR_HANDLE);
		return FALSE;
	}

	ZeroMemory(&m_MMCKInfoParent, sizeof(MMCKINFO));
	m_MMCKInfoParent.fccType = mmioFOURCC('W','A','V','E');

	MMRESULT mmResult = ::mmioCreateChunk( m_hFile,&m_MMCKInfoParent, MMIO_CREATERIFF);
	
	ZeroMemory(&m_MMCKInfoChild, sizeof(MMCKINFO));
	m_MMCKInfoChild.ckid = mmioFOURCC('f','m','t',' ');
	m_MMCKInfoChild.cksize = sizeof(WAVEFORMATEX) + waveFormat.cbSize;
	mmResult = ::mmioCreateChunk(m_hFile, &m_MMCKInfoChild, 0);
	mmResult = ::mmioWrite(m_hFile, (char*)&waveFormat, sizeof(WAVEFORMATEX) + waveFormat.cbSize); 
	mmResult = ::mmioAscend(m_hFile, &m_MMCKInfoChild, 0);
	m_MMCKInfoChild.ckid = mmioFOURCC('d', 'a', 't', 'a');
	mmResult = ::mmioCreateChunk(m_hFile, &m_MMCKInfoChild, 0);

	return TRUE;
	
}

BOOL CWaveRecord::SetBuffer()
{
	m_nBufSize = waveFormat.nAvgBytesPerSec;
	m_nBufs = 10;
	return TRUE;
}

BOOL CWaveRecord::PostStop()
{
	if(m_hFile)
	{
		::mmioAscend(m_hFile, &m_MMCKInfoChild, 0);
		::mmioAscend(m_hFile, &m_MMCKInfoParent, 0);
		::mmioClose(m_hFile, 0);
		m_hFile = NULL;
	}	
	return TRUE;
}

BOOL CWaveRecord::ProcessData(BYTE* data,int size,BOOL bLastBlock)
{
	int length = mmioWrite(m_hFile, (const char*)data, size);
	if(length == size)
		return TRUE;
	else
		return FALSE;
}

