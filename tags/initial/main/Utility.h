#ifndef _UTILITY_H
#define _UTILITY_H

void ReportErrMsg();

void ReportErrMsg(TCHAR* msg);

void ReportErrMsg(int arg);

BOOL GetActiveRasInterface(TCHAR* buf,int size);

#endif