/////////////////////////////////
// 文件名：FS_Kernal
// 创建者：icelx
// 时间：2001.12.16
// 内容：此文件用于实现底层对磁盘的io操作
// 以一个块为单位进行读写
// 必须指出，在整个程序中FS_Kernal类的对象只能有一个
// 任何对虚拟磁盘文件的读写都要通过这个对象来实现
// 修改： (包括修改时间，修改者，修改内容)
////////////////////////////////
#ifndef _FS_KERNEL_H
#define _FS_KERNEL_H

#include <fstream.h>
#include <string.h>
#include <io.h>

#define Byte unsigned char     //字节型变量，代表一个字节的值

class FS_Kernel
{
protected:

	char strDiskName[20];
	fstream DiskFile;	// DiskFile的首部是FAT区，而FAT区的前几个字节
	                    // 用于存放系统信息，第一个Byte，format的标志
	                   
	int iErrorCode;    //iErrorCode值: 0    正常
	                   //              1    读操作发生错误
	                   //              2    写操作发生错误
public:
  int bFormated;		//是否格式化
	FS_Kernel(char* strDName)//strDName是虚拟磁盘文件所在路径
	{
		iErrorCode=0;
		strcpy(strDiskName,strDName);
		DiskFile.open(strDiskName,ios::binary | ios::in | ios::out);  
	}
	~FS_Kernel()
	{
		DiskFile.close();
	}
	//  底层I/O操作接口,读/写块的内容
	int Write_Block( Byte* pData,int iBlockNum,int iBlockSize );   //iBlockNum表示此块在虚拟磁盘文件内的块号
	int Read_Block( Byte* pData,int iBlockNum,int iBlockSize );    
	int Format_Disk(int VFSDiskSize,int iFATBlock);    //格式化虚拟磁盘文件
	int IsFormated(int);
};

#endif