
//////////////////////////////////////////////////////////////////
// 文件名：		Buffer.h
// 创建者：		zyk
// 时间：		2002.1.3.
// 内容：		此类用于定义Buffer
// 修改：		zyk
// 修改内容：	none
//////////////////////////////////////////////////////////////////



#ifndef _Buffer
#define _Buffer

#include "FileStruct.h"
#include "FS.h"
#include "FS_Kernel.h"

#define BUFFER_BLOCK_NUM 16

typedef struct tagBuffer_Line
{
  int bValid;
  int bModified;
  File_Struct *fp; 
  int iLength;
  Byte *Content;
  int next;
} Buffer_Line;

typedef struct tagBuffer_Table
{
  FILE_HANDLE fd;
  int iFirst_Block;
  int iBuf_No;
  tagBuffer_Table *next;
} Buffer_Table;

typedef struct tagHistory
{
  int iBuf_No;
  tagHistory *next;
} History;

class Buffer
{
public:

  int Buffer_Read(File_Struct *fp,Byte *pDest,int iOffset,int &iLen);
  int Buffer_Write(File_Struct *fp,Byte *pSource,int iOffset,int &iLen);
  int Buffer_Flush(File_Struct *fp);
  void Show_Buffer(FILE_HANDLE fd);
  Buffer(FS* pVFS_Related,int iBuf_Block_Size);
  ~Buffer();

private:

  int Replace_Block(int &iBuf_No);
  int Alloc_Block(int &iBuf_No);
  int Find_Block(FILE_HANDLE fd,int iPos,int &iBuf_No);
  void Change_History(int iBuf_No);
  void Add_Entry(FILE_HANDLE fd,int iBuf_No);
  void Free_Block(int iBuf_No);

  Buffer_Line *pBuf_Line;
  Buffer_Table *pBuf_Table;
  History *pBuf_History;  
  int iFree_Block;
  int iBuffer_Block_Size;
  FS* pVFS;
};

#endif