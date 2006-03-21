// wRec.cpp : Defines the entry point for the DLL application.
//

#include "stdafx.h"
#include "wRec.h"
#include "Record.h"

CRecord*	g_pRecrd = NULL;
CMp3Record* g_pMp3Recrd = NULL;
CWaveRecord* g_pWaveRecrd = NULL;

BOOL APIENTRY DllMain( HANDLE hModule, 
                       DWORD  ul_reason_for_call, 
                       LPVOID lpReserved
					 )
{
    switch (ul_reason_for_call)
	{
		case DLL_PROCESS_ATTACH:
		case DLL_THREAD_ATTACH:
		case DLL_THREAD_DETACH:
		case DLL_PROCESS_DETACH:
			break;
    }
    return TRUE;
}


// This is an example of an exported variable
//WREC_API int nWRec=0;

// This is an example of an exported function.
/*WREC_API int fnWRec(void)
{
	return 42;
}
*/
// This is the constructor of a class that has been exported.
// see wRec.h for the class definition
/*CWRec::CWRec()
{ 
	return; 
}
*/
WREC_API int GetDeviceCount()
{
	int num;
	if(NULL != g_pRecrd)
		return -1;
	g_pRecrd = new CRecord();
	num = g_pRecrd->GetDeviceCount();
	delete g_pRecrd;
	g_pRecrd = NULL;
	return num;
}

WREC_API BOOL RecStart(int type)
{
	CRecord* pRecrd;
	switch(type)
	{
	case 0:		// wave
		pRecrd = dynamic_cast<CRecord*>(g_pWaveRecrd);
		break;
	case 1:		// mp3
		pRecrd = dynamic_cast<CRecord*>(g_pMp3Recrd);
		break;
	default:
		pRecrd = NULL;
		break;
	}
	if(NULL == pRecrd)	return FALSE;
	if(!pRecrd->Start())
	{
		delete pRecrd;
		switch(type)
		{
		case 0:		// wave
			g_pWaveRecrd=NULL;
			break;
		case 1:		// mp3
			g_pMp3Recrd=NULL;
			break;
		}
		return FALSE;
	}
	return TRUE;
}

WREC_API BOOL RecStop(int type)
{
	CRecord* pRecrd;
	switch(type)
	{
	case 0:		// wave
		pRecrd = dynamic_cast<CRecord*>(g_pWaveRecrd);
		break;
	case 1:		// mp3
		pRecrd = dynamic_cast<CRecord*>(g_pMp3Recrd);
		break;
	default:
		pRecrd = NULL;
		break;
	}
	if(NULL == pRecrd)	return FALSE;
	if(!pRecrd->Stop())
	{
		delete pRecrd;
		switch(type)
		{
		case 0:		// wave
			g_pWaveRecrd=NULL;
			break;
		case 1:		// mp3
			g_pMp3Recrd=NULL;
			break;
		}
		return FALSE;
	}
	else
	{
		delete pRecrd;
		switch(type)
		{
		case 0:		// wave
			g_pWaveRecrd=NULL;
			break;
		case 1:		// mp3
			g_pMp3Recrd=NULL;
			break;
		}
		return TRUE;
	}
	return TRUE;
}

WREC_API BOOL mp3RecInit(int nDeviceID,int nChannels,int nSamplePerSecond,int nBitsPerSample, int nBitRate, BOOL bVBR, TCHAR* szFile)
{
	if(NULL == g_pMp3Recrd)
	{	
		if(-1==nDeviceID)
			nDeviceID = WAVE_MAPPER;
		g_pMp3Recrd = new CMp3Record(nDeviceID,nChannels,nSamplePerSecond,nBitsPerSample,
								nBitRate,bVBR,szFile);	
	}
	else
		return FALSE;
	if(!g_pMp3Recrd->Init())
	{
		delete g_pMp3Recrd;
		g_pMp3Recrd = NULL;
		return FALSE;
	}
	return TRUE;
}


WREC_API BOOL wavRecInit(int nDeviceID,int nChannels,int nSamplePerSecond,int nBitsPerSample, TCHAR* szFile)
{
	if(NULL == g_pWaveRecrd)
	{	
		if(-1==nDeviceID)
			nDeviceID = WAVE_MAPPER;
		g_pWaveRecrd = new CWaveRecord(nDeviceID,nChannels,nSamplePerSecond,nBitsPerSample,
								szFile);	
	}
	else
		return FALSE;
	if(!g_pWaveRecrd->Init())
	{
		delete g_pWaveRecrd;
		g_pWaveRecrd = NULL;
		return FALSE;
	}
	return TRUE;
}

