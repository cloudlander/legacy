#include "FS_Kernel.h"

#include <math.h>

/////////////////////////////////
// ���ܣ�д��
// ���룺д��������pData�����iBlockNum
// ����ֵ�� 0--�ɹ���1--дʧ��
// ��ע��
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
// ���ܣ�����
// ���룺���iBlockNum
// ����������������pData
// ����ֵ�� 0--�ɹ���1--��ʧ��
// ��ע��
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
// ���ܣ�����������ļ���׼��ʽ��
// ����ֵ�� 0--�ɹ���1--ʧ��
// ��ע�����ȶ����������̡����㣬Ȼ���ʼ��FAT
// ���ڳ�ʼ����Ŀ¼���ɸ߲��FS���Format����
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
	// дFAT
	Temp[0]=255;
    DiskFile.seekp(4,ios::beg);
	for (i=4;i<2*iFATBlock;i++)  //ÿ�������ռ2Byte 
		                         //����ǰ����
	{
		if (DiskFile.write(Temp,1)==NULL)
			return 1;
	}
    // дformat��־λ
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
// ��������		IsFormated
// ���ܣ�		�жϵ�ǰ��������ļ��Ƿ��ʽ��
// ����ֵ��		0 - δ��ʽ����1 - �Ѹ�ʽ��
// ��ע��
//////////////////////////////////////////////////////////
int FS_Kernel::IsFormated(int VFSDiskSize)
{
	Byte temp;
	long len,fd;//len���ļ����ȣ�fd��file handle
	fd=DiskFile.fd();
	len=_filelength(fd);
	if (len<VFSDiskSize)
		return 0;
	DiskFile.read(&temp,1);
	if (temp==0xF7){
		DiskFile.read(&temp,1);
		if( 0x00 == temp)  // ��δ������Ŀ¼
		{	
			temp = temp | 0x80;   // ��λ
			DiskFile.seekp(1);
			DiskFile.write(&temp,1);
			return 2;
		}
		return 1;		   // ������������
	}
	else
		return 0;          //��δ��ʽ��
	
}
