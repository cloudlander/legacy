
// The following ifdef block is the standard way of creating macros which make exporting 
// from a DLL simpler. All files within this DLL are compiled with the WREC_EXPORTS
// symbol defined on the command line. this symbol should not be defined on any project
// that uses this DLL. This way any other project whose source files include this file see 
// WREC_API functions as being imported from a DLL, wheras this DLL sees symbols
// defined with this macro as being exported.
#ifdef WREC_EXPORTS
#define WREC_API __declspec(dllexport)
#else
#define WREC_API __declspec(dllimport)
#endif

/*
// This class is exported from the wRec.dll
class WREC_API CWRec {
public:
	CWRec(void);
	// TODO: add your methods here.
};
*/
//extern WREC_API int nWRec;
WREC_API BOOL RecStart(int type);

WREC_API BOOL RecStop(int type);

WREC_API BOOL mp3RecInit(int nDeviceID,int nChannels,int nSamplePerSecond,int nBitsPerSample, int nBitRate, BOOL bVBR, TCHAR* szFile);

WREC_API BOOL wavRecInit(int nDeviceID,int nChannels,int nSamplePerSecond,int nBitsPerSample, TCHAR* szFile);

WREC_API int  GetDeviceCount();