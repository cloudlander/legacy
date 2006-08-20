
//////////////////////////////////////////////////////////////////
// 文件名：		Buffer.cpp
// 创建者：		zyk
// 时间：		2002.1.3.
// 内容：		Buffer地实现
// 修改：		zyk
// 修改内容：	none
//////////////////////////////////////////////////////////////////


#include "Buffer.h"
#include <stdio.h>
#include <conio.h>

Buffer::Buffer(FS* pVFS_Related,int iBuf_Block_Size)
{
  int i;
  iBuffer_Block_Size=iBuf_Block_Size;
  pBuf_Table=NULL;
  pBuf_History=NULL;
  pBuf_Line=new Buffer_Line[BUFFER_BLOCK_NUM];
  for (i=0;i<BUFFER_BLOCK_NUM;i++)
  {
    pBuf_Line[i].bModified=0;
    pBuf_Line[i].bValid=0;
    pBuf_Line[i].fp=new File_Struct;
    pBuf_Line[i].iLength=0;
    pBuf_Line[i].next=i+1;
    pBuf_Line[i].Content=new Byte[iBuffer_Block_Size];
  }
  pVFS=pVFS_Related;
  iFree_Block=0;
}

Buffer::~Buffer()
{
  int i;
  Buffer_Table *pCur_Entry,*pTemp_Entry;
  History *pCur_Item,*pTemp_Item;
  for (i=0;i<BUFFER_BLOCK_NUM;i++)
  {
    delete[] pBuf_Line[i].Content;
    delete pBuf_Line[i].fp;
  }
  delete[] pBuf_Line;
  pCur_Entry=pBuf_Table;
  while (pCur_Entry)
  {
    pTemp_Entry=pCur_Entry;
    pCur_Entry=pCur_Entry->next;
    delete pTemp_Entry;
  }
  pCur_Item=pBuf_History;
  while (pCur_Item)
  {
    pTemp_Item=pCur_Item;
    pCur_Item=pCur_Item->next;
    delete pTemp_Item;
  }
}

void Buffer::Change_History(int iBuf_No)
{
  History *pPrev_Item,*pCur_Item;
  pCur_Item=pBuf_History;
  pPrev_Item=NULL;
  while (pCur_Item && pCur_Item->iBuf_No!=iBuf_No)
  {
    pPrev_Item=pCur_Item;
    pCur_Item=pCur_Item->next;
  }
  if (!pCur_Item)
  {
    pPrev_Item=new History;
    pPrev_Item->iBuf_No=iBuf_No;
    pPrev_Item->next=pBuf_History;
    pBuf_History=pPrev_Item;
  }
  else
    if (pPrev_Item)
    {
      pPrev_Item->next=pCur_Item->next;
      pCur_Item->next=pBuf_History;
      pBuf_History=pCur_Item;
    }
}

int Buffer::Replace_Block(int &iBuf_No)
{
  int iLen,iErr_Code;
  History *pBuf_Replace;
  Buffer_Table *pCur_Entry;
  int iCur_Buf_No,iPrev_Buf_No;
  pBuf_Replace=pBuf_History;
  while(pBuf_Replace->next)
    pBuf_Replace=pBuf_Replace->next;
  iBuf_No=pBuf_Replace->iBuf_No;
  pBuf_Line[iBuf_No].bValid=0;
  pCur_Entry=pBuf_Table;
  while (pCur_Entry->fd!=pBuf_Line[iBuf_No].fp->fd)
    pCur_Entry=pCur_Entry->next;
  if (pBuf_Line[iBuf_No].bModified==1)
  {
    if (pBuf_Line[iBuf_No].fp->First_Block_Num==0xFFFF)
      pBuf_Line[iBuf_No].fp->First_Block_Num=pCur_Entry->iFirst_Block;
    iErr_Code=pVFS->WriteFile(pBuf_Line[iBuf_No].fp,pBuf_Line[iBuf_No].Content,pBuf_Line[iBuf_No].iLength,iLen);
    if (iErr_Code!=ERR_SUCCESS)
      return iErr_Code;
    pBuf_Line[iBuf_No].iLength=0;
    if (pCur_Entry->iFirst_Block==0xFFFF)
      pCur_Entry->iFirst_Block=pBuf_Line[iBuf_No].fp->First_Block_Num;
  }
  iPrev_Buf_No=BUFFER_BLOCK_NUM;
  iCur_Buf_No=pCur_Entry->iBuf_No;
  while (iCur_Buf_No!=iBuf_No)
  {  
    iPrev_Buf_No=iCur_Buf_No;
    iCur_Buf_No=pBuf_Line[iCur_Buf_No].next;
  }
  if (iPrev_Buf_No==BUFFER_BLOCK_NUM)
    pCur_Entry->iBuf_No=pBuf_Line[iCur_Buf_No].next;
  else
    pBuf_Line[iPrev_Buf_No].next=pBuf_Line[iCur_Buf_No].next;
  return iErr_Code;
}

int Buffer::Alloc_Block(int &iBuf_No)
{
  if (iFree_Block==BUFFER_BLOCK_NUM) 
     return Replace_Block(iBuf_No);
   else
   {
     iBuf_No=iFree_Block;
     iFree_Block=pBuf_Line[iFree_Block].next;
     return ERR_SUCCESS;
   }     
}

int Buffer::Find_Block(FILE_HANDLE fd,int iPos,int &iBuf_No)
{
  Buffer_Table *pCur_Entry;
  int bFound=0;
  pCur_Entry=pBuf_Table;
  while (!bFound && pCur_Entry)
  {
     if (pCur_Entry->fd!=fd)
       pCur_Entry=pCur_Entry->next;
     else
     {
       iBuf_No=pCur_Entry->iBuf_No;
       while (!bFound && iBuf_No!=BUFFER_BLOCK_NUM)
         if (pBuf_Line[iBuf_No].fp->Base_Pos<=iPos && iPos<pBuf_Line[iBuf_No].fp->Base_Pos+iBuffer_Block_Size)
           bFound=1;
         else
           iBuf_No=pBuf_Line[iBuf_No].next;
      if (iBuf_No==BUFFER_BLOCK_NUM)
        break;
     }
  }
  return bFound;
}

void Buffer::Add_Entry(FILE_HANDLE fd,int iBuf_No)
{
  Buffer_Table *pCur_Entry,*pPrev_Entry;
  int iPrev_Buf_No=BUFFER_BLOCK_NUM;
  pPrev_Entry=NULL;
  pCur_Entry=pBuf_Table;
  while (pCur_Entry && pCur_Entry->fd!=fd)
  {
    pPrev_Entry=pCur_Entry;
    pCur_Entry=pCur_Entry->next;
  }
  if (!pCur_Entry)
    if (!pPrev_Entry)
    {
      pBuf_Table=new Buffer_Table;
      pBuf_Table->iBuf_No=iBuf_No;
      pBuf_Table->next=NULL;
      pBuf_Table->fd=fd;
      pBuf_Table->iFirst_Block=0xFFFF;
    }
    else
    {
      pCur_Entry=new Buffer_Table;
      pCur_Entry->fd=fd;
      pCur_Entry->iBuf_No=iBuf_No;
      pCur_Entry->next=NULL;
      pPrev_Entry->next=pCur_Entry;
      pCur_Entry->iFirst_Block=0xFFFF;
    }
  else
  {
    iPrev_Buf_No=pCur_Entry->iBuf_No;
    while (pBuf_Line[iPrev_Buf_No].next!=BUFFER_BLOCK_NUM)
      iPrev_Buf_No=pBuf_Line[iPrev_Buf_No].next;
    pBuf_Line[iPrev_Buf_No].next=iBuf_No;
  }
}

void Buffer::Free_Block(int iBuf_No)
{
  int iPrev_Buf_No;
  iPrev_Buf_No=iFree_Block;
  if (iPrev_Buf_No!=BUFFER_BLOCK_NUM)
  {
    while (pBuf_Line[iPrev_Buf_No].next!=BUFFER_BLOCK_NUM)
      iPrev_Buf_No=pBuf_Line[iPrev_Buf_No].next;
    pBuf_Line[iPrev_Buf_No].next=iBuf_No;
  }
  else
    iFree_Block=iBuf_No;
  pBuf_Line[iBuf_No].next=BUFFER_BLOCK_NUM;
}

int Buffer::Buffer_Read(File_Struct *fp,Byte *pDest,int iOffset,int &iLen)
{
  int iCur_Pos,iErr_Code=ERR_SUCCESS;
  Byte *pCur_Dest;
  FILE_HANDLE fd;
  int iBuf_No=BUFFER_BLOCK_NUM;
  iLen=0;
  fd=fp->fd;
  pCur_Dest=pDest;
  iCur_Pos=fp->Base_Pos;
  while (iLen<iOffset && iErr_Code==ERR_SUCCESS)
  {
    if (Find_Block(fp->fd,iCur_Pos,iBuf_No))
    {
      Change_History(iBuf_No);
      while (iLen<iOffset && iCur_Pos<pBuf_Line[iBuf_No].fp->Base_Pos+pBuf_Line[iBuf_No].iLength)
      {
        iLen++;
        *pCur_Dest++=pBuf_Line[iBuf_No].Content[iCur_Pos++-pBuf_Line[iBuf_No].fp->Base_Pos];
      }
      fp->Base_Pos=iCur_Pos;
    }
    else
    {
      iErr_Code=Alloc_Block(iBuf_No);
      if (iErr_Code==ERR_SUCCESS)
      {
        pBuf_Line[iBuf_No].bModified=0;
        pBuf_Line[iBuf_No].bValid=1;
        memcpy(pBuf_Line[iBuf_No].fp,fp,sizeof(File_Struct));
        pBuf_Line[iBuf_No].next=BUFFER_BLOCK_NUM;
        iErr_Code=pVFS->ReadFile(pBuf_Line[iBuf_No].fp,pBuf_Line[iBuf_No].Content,iBuffer_Block_Size,pBuf_Line[iBuf_No].iLength);
        if (iErr_Code==EXC_END_OF_FILE)
          iErr_Code=ERR_SUCCESS;
      }
      if (iErr_Code!=ERR_SUCCESS)
        Free_Block(iBuf_No);
      else
      {
        Add_Entry(fd,iBuf_No);
        memcpy(pBuf_Line[iBuf_No].fp,fp,sizeof(File_Struct));
        if (pBuf_Line[iBuf_No].iLength>iBuffer_Block_Size)
          pBuf_Line[iBuf_No].iLength=iBuffer_Block_Size;
      }
    }
  }
  return iErr_Code;
}

int Buffer::Buffer_Write(File_Struct *fp,Byte *pSource,int iOffset,int &iLen)
{
  int iCur_Pos,iErr_Code=ERR_SUCCESS;
  Byte *pCur_Source;
  FILE_HANDLE fd;
  int iBuf_No=BUFFER_BLOCK_NUM;
  iLen=0;
  fd=fp->fd;
  pCur_Source=pSource;
  iCur_Pos=fp->Base_Pos;
  while (iLen<iOffset && iErr_Code==ERR_SUCCESS)
  {
    if (Find_Block(fp->fd,iCur_Pos,iBuf_No))
    {
      Change_History(iBuf_No);
      while (iLen<iOffset && iCur_Pos<pBuf_Line[iBuf_No].fp->Base_Pos+iBuffer_Block_Size)
      {
        iLen++;
        pBuf_Line[iBuf_No].Content[iCur_Pos++-pBuf_Line[iBuf_No].fp->Base_Pos]=*pCur_Source++;
        if (pBuf_Line[iBuf_No].iLength<iCur_Pos-pBuf_Line[iBuf_No].fp->Base_Pos)
          pBuf_Line[iBuf_No].iLength=iCur_Pos-pBuf_Line[iBuf_No].fp->Base_Pos;
      }
      pBuf_Line[iBuf_No].bModified=1;
      fp->Base_Pos=iCur_Pos;
    }
    else
    {
      iErr_Code=Alloc_Block(iBuf_No);
      if (iErr_Code==ERR_SUCCESS)
      {
        pBuf_Line[iBuf_No].bModified=0;
        pBuf_Line[iBuf_No].bValid=1;
        memcpy(pBuf_Line[iBuf_No].fp,fp,sizeof(File_Struct));
        pBuf_Line[iBuf_No].next=BUFFER_BLOCK_NUM;
      }
      if (iErr_Code!=ERR_SUCCESS)
        Free_Block(iBuf_No);
      else
        Add_Entry(fd,iBuf_No);
    }
  }
  return iErr_Code;
}

void Buffer::Show_Buffer(FILE_HANDLE fd)
{
  Buffer_Table *pCur_Entry;
  int iBuf_No,i;
  printf("The buffer of file %d is as follows.\n",fd);
  pCur_Entry=pBuf_Table;
  while (pCur_Entry && pCur_Entry->fd!=fd)
    pCur_Entry=pCur_Entry->next;
  if (pCur_Entry)
  {
    iBuf_No=pCur_Entry->iBuf_No;
    while (iBuf_No!=BUFFER_BLOCK_NUM)
    {
      printf("\nBuffer block %d :\n",iBuf_No);
      for (i=0;i<pBuf_Line[iBuf_No].iLength;i++)
        putch(pBuf_Line[iBuf_No].Content[i]);
      iBuf_No=pBuf_Line[iBuf_No].next;
      putch('\n');
    }
  }
  else
    printf("No buffer.\n");
}

int Buffer::Buffer_Flush(File_Struct *fp)
{
  Buffer_Table *pCur_Entry,*pPrev_Entry;
  int iBuf_No,iLen,iErr_Code=ERR_SUCCESS,iTemp;
  pPrev_Entry=NULL;
  pCur_Entry=pBuf_Table;
  while (pCur_Entry && pCur_Entry->fd!=fp->fd)
  {
    pPrev_Entry=pCur_Entry;
    pCur_Entry=pCur_Entry->next;
  }
  if (pCur_Entry)
  {
    iBuf_No=pCur_Entry->iBuf_No;
    while (iBuf_No!=BUFFER_BLOCK_NUM && iErr_Code==ERR_SUCCESS)
    {
      pBuf_Line[iBuf_No].bValid=0;
      if (pBuf_Line[iBuf_No].bModified==1)
      {
        if (pBuf_Line[iBuf_No].fp->First_Block_Num==0xFFFF)
          pBuf_Line[iBuf_No].fp->First_Block_Num=pCur_Entry->iFirst_Block;
        iErr_Code=pVFS->WriteFile(pBuf_Line[iBuf_No].fp,pBuf_Line[iBuf_No].Content,pBuf_Line[iBuf_No].iLength,iLen);
      if (iErr_Code!=ERR_SUCCESS)
        return iErr_Code;
      memcpy(fp,pBuf_Line[iBuf_No].fp,sizeof(File_Struct));
      if (pCur_Entry->iFirst_Block==0xFFFF)
        pCur_Entry->iFirst_Block=fp->First_Block_Num;
      }
      iTemp=iBuf_No;
      iBuf_No=pBuf_Line[iBuf_No].next;
      Free_Block(iTemp);
    }
    if (iErr_Code!=ERR_SUCCESS)
      return iErr_Code;
    if (pPrev_Entry)
      pPrev_Entry->next=pCur_Entry->next;
    else
      pBuf_Table=pCur_Entry->next;
  }
  return iErr_Code;
}