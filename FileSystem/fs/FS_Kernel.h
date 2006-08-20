/////////////////////////////////
// �ļ�����FS_Kernal
// �����ߣ�icelx
// ʱ�䣺2001.12.16
// ���ݣ����ļ�����ʵ�ֵײ�Դ��̵�io����
// ��һ����Ϊ��λ���ж�д
// ����ָ����������������FS_Kernal��Ķ���ֻ����һ��
// �κζ���������ļ��Ķ�д��Ҫͨ�����������ʵ��
// �޸ģ� (�����޸�ʱ�䣬�޸��ߣ��޸�����)
////////////////////////////////
#ifndef _FS_KERNEL_H
#define _FS_KERNEL_H

#include <fstream.h>
#include <string.h>
#include <io.h>

#define Byte unsigned char     //�ֽ��ͱ���������һ���ֽڵ�ֵ

class FS_Kernel
{
protected:

	char strDiskName[20];
	fstream DiskFile;	// DiskFile���ײ���FAT������FAT����ǰ�����ֽ�
	                    // ���ڴ��ϵͳ��Ϣ����һ��Byte��format�ı�־
	                   
	int iErrorCode;    //iErrorCodeֵ: 0    ����
	                   //              1    ��������������
	                   //              2    д������������
public:
  int bFormated;		//�Ƿ��ʽ��
	FS_Kernel(char* strDName)//strDName����������ļ�����·��
	{
		iErrorCode=0;
		strcpy(strDiskName,strDName);
		DiskFile.open(strDiskName,ios::binary | ios::in | ios::out);  
	}
	~FS_Kernel()
	{
		DiskFile.close();
	}
	//  �ײ�I/O�����ӿ�,��/д�������
	int Write_Block( Byte* pData,int iBlockNum,int iBlockSize );   //iBlockNum��ʾ�˿�����������ļ��ڵĿ��
	int Read_Block( Byte* pData,int iBlockNum,int iBlockSize );    
	int Format_Disk(int VFSDiskSize,int iFATBlock);    //��ʽ����������ļ�
	int IsFormated(int);
};

#endif