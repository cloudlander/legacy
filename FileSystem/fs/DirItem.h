
//----------------------------------------------------------------------------

//////////////////////////////////////////////////////////////////
// �ļ�����		DirItem.h
// �����ߣ�		icelx
// ʱ�䣺		2001.12.16.
// ���ݣ�		�������ڶ��������Ŀ¼��Ŀ��Ϣ
// �޸ģ�		rick
// �޸����ݣ�	�޸ĳ������ݵĶ���
//////////////////////////////////////////////////////////////////

#ifndef _DirItem
#define _DirItem

#include "FS_Kernel.h"

#define DirItem_BlockNum iBlockNumSize
#define MAX_PATH         256

const int DirItem_AttriLen = 3;
const int DirItem_TimeDateLen = 4;
const int DirItem_FileLen = 4;
const int File_Name_Len = 16;
const int File_ExtName_Len = 3;

struct DirItem
{
	char* strDFName;
	char* strExtName;
	Byte* Attri;
	time_t Time_Date;
	int iFirstBlockNum,iFileLength;
};

#endif

//----------------------------------------------------------------------------
