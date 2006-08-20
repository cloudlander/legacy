
//----------------------------------------------------------------------------

//////////////////////////////////////////////////////////
//   �ļ���Ŀ¼�ṹ����
//   FS����ã�������ά��.
//   �ṹ��ʹ������ֱ��
//   ���DirItem��Ч����
//   Ŀ¼�ָ���Ϊ / 
//   �����ߣ�pentium
//   �޸��ߣ�rick 
//////////////////////////////////////////////////////////

#ifndef _FS_STRUCT_H
#define _FS_STRUCT_H

#include "DirItem.h"

#include <ctime>

//  �������ļ����ԣ���ӦDirItem�е�Attri�ĵ�2���ֽ�
//  Ϊ����ʹ�ã� ����궨��  {

#define  FILE_READ_ONLY          0xFEF0         // ֻ��
#define  FILE_WRITE_ONLY         0xFEF1         // ֻд
#define  FILE_READ_WRITE         0xFEF2         // ��д

//  �����������...              �������˳��
/////////////////////////////}

typedef  char OPEN_MODE;                        // ��ȡֵΪ w,a,r
typedef  int  FILE_HANDLE;                      // �ļ����
                                                // FD����ʹ��


typedef struct tagFileStruct                    // ���ӱ���ʽ 
{
	char FileName[File_Name_Len+1];               // �ļ���Ŀ¼����
	char FileExtName[File_ExtName_Len+1];         // �ļ���չ��
	int  File_Length;                           // �ļ�����
	int  Cur_File_Length;                       // ��ǰ�ļ�����
//  �����õ���ʱ������
//	time_t Time_Last_Modified;                   ����޸�ʱ��
//	time_t Time_Last_Read;                       ������ʱ��
	time_t Time_Last_Write;                     // ���д��ʱ��
	Byte File_Attri[DirItem_AttriLen];	        // �ļ�����(����Ȩ��)
	OPEN_MODE mode;
	int First_Block_Num;						// �ļ��׿�� 
	int DirItem_Block_Num;                      // �ļ���ӦĿ¼��Ŀ��
	                                            // ��Ϊ���㴦��
	FILE_HANDLE fd;                             // �ļ����
	int Base_Pos;                               // �ļ��Ļ�׼λ��
	void*  buffer;                              // ������ָ��(��zyk����)
	struct tagFileStruct* next;                 // ����ָ��

} File_Struct;

#endif

//----------------------------------------------------------------------------
