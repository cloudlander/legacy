// Record.h: interface for the CRecord class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_RECORD_H__9B0ED60A_A88B_45CD_A9A3_23E5FBA8DC67__INCLUDED_)
#define AFX_RECORD_H__9B0ED60A_A88B_45CD_A9A3_23E5FBA8DC67__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include <mmsystem.h>
#include "BladeMp3EncDll.h"

#define		WM_DATA_READY	WM_USER+100
#define		WM_FINISH		WM_USER+101

#define		ERR_MEMORY		0x100
#define		ERR_HANDLE		0x101

class CRecord  
{
public:
	CRecord(int nDeviceID,int nChannels,int nSamplePerSecond,int nBitsPerSample);
	CRecord();
	virtual ~CRecord();
	virtual BOOL PreInit()
	{
		return TRUE;
	}
	BOOL Init();
	BOOL Start();
	BOOL Stop();
	virtual BOOL PostStop()
	{
		return TRUE;
	}
	void ReportError(int errCode);
	void ReportError(TCHAR* msg,int errCode=0);
	virtual BOOL SetBuffer()
	{
		return TRUE;
	}
	virtual BOOL ProcessData(BYTE* data,int size,BOOL bLastBlock)
	{
		return TRUE;
	}
	int GetDeviceCount();

	static unsigned WINAPI waveInProc(LPVOID arg);
	static unsigned WINAPI waveDataProc(LPVOID arg);

protected:
	int m_nBufs;
	int m_nBufSize;
	BYTE* m_pBuffer;
	WAVEFORMATEX waveFormat;
	HWAVEIN		WaveInHandle;
	WAVEHDR*	pWaveHeader;
	UINT DeviceID;
	int m_nCurBuf;
private:
	BOOL Open(DWORD dwCallback);
	BOOL Close();
	
	int m_nDone;
	BOOL m_bInRecord;
	unsigned int wDPThreadID;
};

#ifdef _MULT_THREAD
#define		WM_MP3_DATA_READY		WM_USER+200
#define		WM_MP3_FINISH			WM_USER+201
#endif

class CMp3Record:public CRecord
{
public:
	CMp3Record(int nDeviceID,int nChannels,int nSamplePerSecond,int nBitsPerSample,int nBitRate,BOOL bVBR,TCHAR* szFile);
	~CMp3Record();
	BOOL PreInit();
	BOOL PostStop();
	BOOL SetBuffer();
	BOOL ProcessData(BYTE* data,int size,BOOL bLastBlock);
protected:
	HINSTANCE	hDLL			;
	BE_VERSION	Version			;
	BE_CONFIG	beConfig		;
	HBE_STREAM	hbeStream		;
	BE_ERR		err				;
	
	BEINITSTREAM		beInitStream;
	BEENCODECHUNK		beEncodeChunk;
	BEDEINITSTREAM		beDeinitStream;
	BECLOSESTREAM		beCloseStream;
	BEVERSION			beVersion;
	BEWRITEVBRHEADER	beWriteVBRHeader;
	BEWRITEINFOTAG		beWriteInfoTag;

	BYTE*	m_pMp3Buffer;
	HANDLE OutFileHandle;
	TCHAR* m_szFileName;
#ifdef _MULT_THREAD
	static unsigned WINAPI DataProc(LPVOID arg);
#endif
private:
	DWORD dwEncoded;
	int m_nSamples;
	int m_nNSamples;
	int m_nMp3BufSize;
#ifdef _MULT_THREAD
	HANDLE* m_pHandles;
	unsigned int* m_pThreadID;
#endif
};

class CWaveRecord : public CRecord  
{
public:
	CWaveRecord(int nDeviceID,int nChannels,int nSamplePerSecond,int nBitsPerSample,TCHAR* szFile);
	virtual ~CWaveRecord();

	BOOL PreInit();
	BOOL PostStop();
	BOOL SetBuffer();
	BOOL ProcessData(BYTE* data,int size,BOOL bLastBlock);

protected:
	TCHAR*		m_szFileName;
	HMMIO		m_hFile;
	MMCKINFO	m_MMCKInfoChild;
	MMCKINFO	m_MMCKInfoParent;
};

#endif // !defined(AFX_RECORD_H__9B0ED60A_A88B_45CD_A9A3_23E5FBA8DC67__INCLUDED_)
