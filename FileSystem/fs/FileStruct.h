
//----------------------------------------------------------------------------

//////////////////////////////////////////////////////////
//   文件和目录结构定义
//   FS类调用，主存中维护.
//   结构中使用数组直接
//   存放DirItem有效数据
//   目录分隔符为 / 
//   创建者：pentium
//   修改者：rick 
//////////////////////////////////////////////////////////

#ifndef _FS_STRUCT_H
#define _FS_STRUCT_H

#include "DirItem.h"

#include <ctime>

//  以下是文件属性，对应DirItem中的Attri的第2个字节
//  为方便使用， 定义宏定义  {

#define  FILE_READ_ONLY          0xFEF0         // 只读
#define  FILE_WRITE_ONLY         0xFEF1         // 只写
#define  FILE_READ_WRITE         0xFEF2         // 读写

//  其他属性添加...              按照这个顺序
/////////////////////////////}

typedef  char OPEN_MODE;                        // 仅取值为 w,a,r
typedef  int  FILE_HANDLE;                      // 文件句柄
                                                // FD命令使用


typedef struct tagFileStruct                    // 连接表形式 
{
	char FileName[File_Name_Len+1];               // 文件（目录）名
	char FileExtName[File_ExtName_Len+1];         // 文件扩展名
	int  File_Length;                           // 文件长度
	int  Cur_File_Length;                       // 当前文件长度
//  可能用到的时间属性
//	time_t Time_Last_Modified;                   最后修改时间
//	time_t Time_Last_Read;                       最后读入时间
	time_t Time_Last_Write;                     // 最后写出时间
	Byte File_Attri[DirItem_AttriLen];	        // 文件属性(包括权限)
	OPEN_MODE mode;
	int First_Block_Num;						// 文件首块号 
	int DirItem_Block_Num;                      // 文件对应目录项的块号
	                                            // 仅为方便处理
	FILE_HANDLE fd;                             // 文件句柄
	int Base_Pos;                               // 文件的基准位置
	void*  buffer;                              // 缓冲区指针(由zyk处理)
	struct tagFileStruct* next;                 // 链接指针

} File_Struct;

#endif

//----------------------------------------------------------------------------
