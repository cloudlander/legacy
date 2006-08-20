
//----------------------------------------------------------------------------

//////////////////////////////////////////////////////////////////
// 文件名：		DirItem.h
// 创建者：		icelx
// 时间：		2001.12.16.
// 内容：		此类用于定义基本的目录项目信息
// 修改：		rick
// 修改内容：	修改常量数据的定义
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
