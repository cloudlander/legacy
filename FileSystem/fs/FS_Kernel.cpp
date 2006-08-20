#include "FS_Kernel.h"

#include <math.h>

/////////////////////////////////
// 功能：写块
// 输入：写入块的内容pData，块号iBlockNum
// 返回值： 0--成功，1--写失败
// 备注：
////////////////////////////////
int FS_Kernel::Write_Block( Byte* pData,int iBlockNum,int iBlockSize )
{
	Byte* pTemp=pData;
	DiskFile.seekp(iBlockNum*iBlockSize,ios::beg);
	if (DiskFile.write(pData,iBlockSize)==NULL)
		return 1;
	return 0;
}

/////////////////////////////////
// 功能：读块
// 输入：块号iBlockNum
// 输出：读出块的内容pData
// 返回值： 0--成功，1--读失败
// 备注：
////////////////////////////////
int FS_Kernel::Read_Block( Byte* pData,int iBlockNum,int iBlockSize )
{
	Byte* pTemp=pData;
	DiskFile.seekp(iBlockNum*iBlockSize,ios::beg);
    if (DiskFile.read(pData,iBlockSize)==NULL)
		return 1;
	return 0;
}

/////////////////////////////////
// 功能：对虚拟磁盘文件标准格式化
// 返回值： 0--成功，1--失败
// 备注：首先对整个“磁盘”冲零，然后初始化FAT
// 对于初始化根目录，由高层的FS类的Format来做
////////////////////////////////
int FS_Kernel::Format_Disk(int VFSDiskSize,int iFATBlock)
{
	int BlockSize;
	BlockSize= sqrt(2*VFSDiskSize/iFATBlock);
	Byte Temp[2]={0,0};
	DiskFile.seekp(0,ios::beg);
	for (int i=0; i<VFSDiskSize;i++)
	{
		if (DiskFile.write(Temp,1)==NULL)
		{
			return 1;
		}
		//*/
	}
	// 写FAT
	Temp[0]=255;
    DiskFile.seekp(4,ios::beg);
	for (i=4;i<2*iFATBlock;i++)  //每个块号项占2Byte 
		                         //保留前两块
	{
		if (DiskFile.write(Temp,1)==NULL)
			return 1;
	}
    // 写format标志位
	DiskFile.seekp(0,ios::beg);
	Temp[0]=0xF7;
	if (DiskFile.write(Temp,1)==NULL)
		return 1;
	Temp[0]=0x80;
	DiskFile.write(Temp,1);
	memcpy(Temp,&BlockSize,2);
	DiskFile.write(Temp,2);
	bFormated=1;
	return 0;
}
//////////////////////////////////////////////////////////
// 函数名：		IsFormated
// 功能：		判断当前虚拟磁盘文件是否格式化
// 返回值：		0 - 未格式化，1 - 已格式化
// 备注：
//////////////////////////////////////////////////////////
int FS_Kernel::IsFormated(int VFSDiskSize)
{
	Byte temp;
	long len,fd;//len是文件长度，fd是file handle
	fd=DiskFile.fd();
	len=_filelength(fd);
	if (len<VFSDiskSize)
		return 0;
	DiskFile.read(&temp,1);
	if (temp==0xF7){
		DiskFile.read(&temp,1);
		if( 0x00 == temp)  // 尚未创建根目录
		{	
			temp = temp | 0x80;   // 置位
			DiskFile.seekp(1);
			DiskFile.write(&temp,1);
			return 2;
		}
		return 1;		   // 可以正常运行
	}
	else
		return 0;          //尚未格式化
	
}
