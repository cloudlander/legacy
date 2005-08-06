#ifndef _OPTIONDATA_H
#define _OPTIONDATA_H
#include "ProxyManager.h"
#include "SearchEngine.h"
class COptionData
{
public:
	CSearchEngine* m_pSE;
	CProxyManager* m_pPM;
	BOOL& m_bBeep;
	int&  m_nComboSize;
	COptionData(CSearchEngine* q,CProxyManager* p,BOOL& bBeep,int& nComboSize):
	m_bBeep(bBeep),m_nComboSize(nComboSize)
	{
		m_pPM = p;
		m_pSE = q;
	}
};

#endif