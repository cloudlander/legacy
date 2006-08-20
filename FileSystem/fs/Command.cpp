
// 文件名： command.cpp
// 创建者： zyk
// 时间： 2001-12-11
// 内容： The interface of the virtual file system
// 修改： none


#include "FS.h"
#include "Buffer.h"
#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include "ErrorDef.h"


#define INIT_SUCCESS 0
#define MAX_CMDLINE_LEN 1024
#define MAX_ARGU_LEN 255
#define INPUT_BUFFER_SIZE 1024
#define MAX_NAME_LENGTH 255
typedef struct tagArgu
{
  char strArgu[MAX_ARGU_LEN];
  struct tagArgu *next;
} Argument;

typedef struct tagFile_Table
{
  File_Struct *fp;
  tagFile_Table *next;
} File_Table;

FS *pVFS;
Buffer *pBuf;
File_Table *pFile_Table;

int DiskSize;
int BlockSize;
int FATBlock;
char DiskFile[MAX_NAME_LENGTH];


// 
// If initiation is successful, the function will return 0,
// Otherwise it will return a non-negative value.

// 函数名： Init
// 功能： Initialize the file system.
// 输入： None
// 输出： Welcome message
// 返回值： 0 represents success. Positive value represents error.          
// 备注： None

int Init(void)
{
  printf("\nSystem starting, please wait.\n");
  printf("\nMounting virtual disk......");
  pVFS=new FS(DiskFile,DiskSize,BlockSize,FATBlock);
	printf ("[ OK ]\n");
  pBuf=new Buffer(pVFS,BlockSize);
  pFile_Table=NULL;
  printf("\n                  Welcome to Z3L Virtual File System   \n");
  printf("\n               It is produced by rick,icelx,zyk,pentium\n");
  printf("\n                        All Rights Reserved.           \n\n\n");

  return 0;
}

int Check_Name(char* strName)
{
  char cInvalidCharacter[]={'?','*','/','\"','<','>','\\','|',':'};
  int i;
  for (i=0;i<8;i++)
    if(strchr(strName,cInvalidCharacter[i]))
      return ERR_NAME_INVALID;
  return 0;  
}

int Find_File_Hanle(FILE_HANDLE fd,File_Struct **fp)
{
  File_Table *pCur_File_Entry,*pPrev_File_Entry;
  pPrev_File_Entry=NULL;
  pCur_File_Entry=pFile_Table;
  while (pCur_File_Entry && pCur_File_Entry->fp->fd!=fd)
    pCur_File_Entry=pCur_File_Entry->next;
  if (!pCur_File_Entry)
    return ERR_HANDLE_INVALID;
  *fp=pCur_File_Entry->fp;
  return ERR_SUCCESS;
}

void DisplayErrMsg(int iErrCode)
{
  switch (iErrCode)
  {
    case ERR_NOT_FORMAT:
      printf("The virtual disk hasn\'t been formatted.\n");
      break;
    case ERR_BAD_COMMAND:
      printf("Unknown Command.\n");
      break;
    case ERR_SYNTAX:
      printf("Command syntax error.\n");
      break;
    case ERR_NAME_INVALID:
      printf("Invalid filename or dirname.\n");
      break;
    case ERR_DIR_DUPLICATE:
      printf("The directory already exists.\n");
      break;
    case ERR_DIR_NOT_FOUND:
      printf("Cannot find the directory.\n");
      break;
    case ERR_DIR_NOT_EMPTY:
      printf("The directory is not empty.\n");
      break;
    case ERR_MODE_INVALID:
      printf("Invalide open mode.\n");
      break;
    case ERR_FILE_NOT_OPENED:
      printf("File is not opened.\n");
      break;
    case ERR_HANDLE_INVALID:
      printf("Invalid file handle.\n");
      break;
    case ERR_FILE_NOT_FOUND:
      printf("File not found.\n");
      break;
    case ERR_OPEN_READ:
      printf("File is opened for reading.\n");
      break;
    case ERR_OPEN_WRITE:
      printf("File is opened for writing.\n");
      break;
    case ERR_NULL_FILE:
      printf("The file doesn't contain content.\n");
      break;
    case ERR_POS_INVALID:
      printf("Invalid Position.\n");
      break;
    case ERR_FILE_ALREADY_OPENED:
      printf("File is already opened.\n");
      break;
    case ERR_ACCESS_DENIED:
      printf("Access Denied.\n");
      break;
    case ERR_ATTRI_NOT_SUPPORT:
      printf("Not supported attribute.\n");
      break;
	case ERR_DISK_FULL:
		printf("Disk is full.\n");
		break;
	case ERR_REMOVE_CURRENT_DIR:
		printf("Attemp to remove current directory.\n");
		break;
	default:
		printf("Error Number : %d",iErrCode);
		break;
  }
}

int Alloc_File_Handle(void)
{
  File_Table *pCur_File_Entry;
  FILE_HANDLE fd=1;
  pCur_File_Entry=pFile_Table;
  while (1)
  {
    while(pCur_File_Entry && pCur_File_Entry->fp->fd!=fd)
      pCur_File_Entry=pCur_File_Entry->next;
    if (!pCur_File_Entry)
      return fd;
    else
      fd++;
  }
}

int CheckOpen(char* strFullName,char mode)
{
  int iErrCode=ERR_SUCCESS;
  char strFileName[MAX_NAME_LENGTH],strPath[MAX_NAME_LENGTH],strTemp[MAX_NAME_LENGTH];
  File_Struct *fp;
  File_Table *pCur_File_Entry;
  if (!strrchr(strFullName,'/'))
  {
    strPath[0]='\0';
    strcpy(strFileName,strFullName);
    iErrCode=Check_Name(strFileName);
  }
  else
  {
    strcpy(strFileName,strrchr(strFullName,'/')+1);
    if (strlen(strFileName)==0)
       return ERR_NAME_INVALID;
    else
      iErrCode=Check_Name(strFileName);

    strcpy(strTemp,strFullName);
    strcpy(strPath,strrev(strchr(strrev(strTemp),'/')));
  }
  fp=new File_Struct;
  fp->fd=Alloc_File_Handle();
  iErrCode=pVFS->OpenFile(fp,strFullName,mode,0);
  switch(iErrCode)
  {
  case ERR_FILE_NOT_FOUND:
    if (mode=='r')
      return ERR_FILE_NOT_FOUND;
    else
      return ERR_SUCCESS;
    break;
  case ERR_SUCCESS:
    pCur_File_Entry=pFile_Table;
    while (pCur_File_Entry && pCur_File_Entry->fp->DirItem_Block_Num!=fp->DirItem_Block_Num)
    {
      strcpy(strTemp,pCur_File_Entry->fp->FileName);
      strcat(strTemp,pCur_File_Entry->fp->FileExtName);
      if (strcmp(strFileName,strTemp)==0)
        break;
      else
        pCur_File_Entry=pCur_File_Entry->next;
    }
    if (pCur_File_Entry)
      return ERR_FILE_ALREADY_OPENED;
    else
      return ERR_SUCCESS;
    break;
  default:
    return iErrCode;
  }
}

void Add_File_Entry(File_Table *New_File_Entry)
{
  File_Table *pCur_File_Entry,*pPrev_File_Entry;
  pPrev_File_Entry=NULL;
  pCur_File_Entry=pFile_Table;
  if (!pFile_Table)
    pFile_Table=New_File_Entry;
  else  
  {
    while (pCur_File_Entry)
    {
      pPrev_File_Entry=pCur_File_Entry;
      pCur_File_Entry=pCur_File_Entry->next;
    }
    pPrev_File_Entry->next=New_File_Entry;
  }
}

void Delete_File_Entry(FILE_HANDLE fd)
{
  File_Table *pCur_File_Entry,*pPrev_File_Entry;
  pPrev_File_Entry=NULL;
  pCur_File_Entry=pFile_Table;
  while (pCur_File_Entry->fp->fd!=fd)
  {
    pPrev_File_Entry=pCur_File_Entry;
    pCur_File_Entry=pCur_File_Entry->next;
  }
  if (pPrev_File_Entry)
    pPrev_File_Entry->next=pCur_File_Entry->next;
  else
    pFile_Table=pCur_File_Entry->next;
  delete pCur_File_Entry->fp;
  delete pCur_File_Entry;
}

// 函数名： InputCommnad
// 功能： 接受命令输入
// 输入： Commands user typed
// 输出： 分解出的各个命令行参数
// 返回值： None
// 备注： None

void InputCommand(Argument **pArgu)
{
  char strCmdLine[MAX_CMDLINE_LEN];
  unsigned int i,j,bInQuote,bInWord;
  Argument *pCurArgu, *pPrevArgu;
  pCurArgu=pPrevArgu=*pArgu=NULL;
  gets(strCmdLine);
  strlwr(strCmdLine);
  if (strlen(strCmdLine)>0)
  {
    i=0;
    bInQuote=0;
    bInWord=0;
    while (i<strlen(strCmdLine) && strCmdLine[i]==' ')
      i++;
    while (i<strlen(strCmdLine))
      switch (strCmdLine[i])
      {
        case ' ':
          if (bInQuote==1)
            pCurArgu->strArgu[j++]=strCmdLine[i++];
          else
          {
            i++;
            if (bInWord==1)
            {
              pCurArgu->strArgu[j]='\0';
              bInWord=0;
            }
          }
          break;
        case '\"':
          i++;
          bInQuote=1-bInQuote;
          if (bInQuote==0)
            pCurArgu->strArgu[j]='\0';
          else
          {
            pPrevArgu=pCurArgu;
            pCurArgu=new Argument;
            pCurArgu->next=NULL;
            j=0;
            if (!*pArgu)
              *pArgu=pCurArgu;
            else
              pPrevArgu->next=pCurArgu;
          }
          break;
        default:
          if (bInWord==0 && bInQuote==0)
          {
            bInWord=1;
            pPrevArgu=pCurArgu;
            pCurArgu=new Argument;
            pCurArgu->next=NULL;
            j=0;
            if (!*pArgu)
              *pArgu=pCurArgu;
            else
              pPrevArgu->next=pCurArgu;
          }
          pCurArgu->strArgu[j++]=strCmdLine[i++];
      }
    if (pCurArgu)
      pCurArgu->strArgu[j]='\0';
  }
}
// 函数名： CommandParser
// 功能： Parse commands user issued
// 输入： Commands user typed
// 输出： The result of the desired operation or the error message.
// 返回值： 0 represents normal exit. Positive value represents abortion because of errors.          
// 备注： None

int Format(Argument *pArgu)
{
  int iErrCode=ERR_SUCCESS;
  File_Table *pTemp;
  char cChoice;
  if (pArgu->next)
    return ERR_SYNTAX;
  while (pFile_Table)
  {
    pBuf->Buffer_Flush(pFile_Table->fp);
    pVFS->CloseFile(pFile_Table->fp);
    pTemp=pFile_Table;
    pFile_Table=pFile_Table->next;
    delete pTemp->fp;
    delete pTemp;
  } 
  printf("ALL DATA WILL BE LOST!\nARE YOU SURE TO GO ON (Y/N)?");
  while(1)
  {
    cChoice=getch();
    if (cChoice=='y' || cChoice=='Y')
    {
      printf("Y\n\nFormatting......");
      iErrCode=pVFS->FS_Format();
      if (iErrCode==ERR_SUCCESS)
        printf("Done\n");              
      break;
    }
    if (cChoice=='n' || cChoice=='N')
    {
      printf("N\n");
      break;
    }
  }
  return iErrCode;
}

int Quit(Argument *pArgu,int &bExit)
{
  if (pArgu->next)
    return ERR_SYNTAX;
  printf("GoodBye!\n");
  bExit=1;
  return ERR_SUCCESS;
}

int MakeDir(Argument *pArgu)
{
  int iErrCode=ERR_SUCCESS; 
  if (!pVFS->IsFormated())
    return ERR_NOT_FORMAT;
  pArgu=pArgu->next;
  if (!pArgu)
    return ERR_SYNTAX;
  while (pArgu && iErrCode==ERR_SUCCESS)
  {
    if (pArgu->strArgu[strlen(pArgu->strArgu)-1]!='/')
    {
      pArgu->strArgu[strlen(pArgu->strArgu)+1]='\0';
      pArgu->strArgu[strlen(pArgu->strArgu)]='/';
    }
    iErrCode=pVFS->MakeDir(pArgu->strArgu);
    pArgu=pArgu->next;
  }
  return iErrCode;
}

int ChangeDir(Argument *pArgu)
{
  int iErrCode=ERR_SUCCESS; 
  if (!pVFS->IsFormated())
    return ERR_NOT_FORMAT;
  pArgu=pArgu->next;
  if (!pArgu)
  {
    printf("%s\n",pVFS->GetCurrentPath());
    return ERR_SUCCESS;
  }
  if (pArgu->next)
    return ERR_SYNTAX;
  if (pArgu->strArgu[strlen(pArgu->strArgu)-1]!='/')
    {
      pArgu->strArgu[strlen(pArgu->strArgu)+1]='\0';
      pArgu->strArgu[strlen(pArgu->strArgu)]='/';
    }
  iErrCode=pVFS->ChangeDir(pArgu->strArgu);
  return iErrCode;
}

int RemoveDir(Argument *pArgu)
{
  int iErrCode=ERR_SUCCESS;
  if (!pVFS->IsFormated())
    return ERR_NOT_FORMAT;
  pArgu=pArgu->next;
  if (!pArgu)
    return ERR_SYNTAX;
  while (pArgu && iErrCode==ERR_SUCCESS)
  {
    if (pArgu->strArgu[strlen(pArgu->strArgu)-1]!='/')
    {
      pArgu->strArgu[strlen(pArgu->strArgu)+1]='\0';
      pArgu->strArgu[strlen(pArgu->strArgu)]='/';
    }
    iErrCode=pVFS->RemoveDir(pArgu->strArgu);
    pArgu=pArgu->next;
  }
  return iErrCode;
}

int OpenFile(Argument *pArgu)
{
  File_Table *pNew_File_Entry;
  char strFullName[MAX_NAME_LENGTH];
  int iErrCode=ERR_SUCCESS;
  if (!pVFS->IsFormated())
    return ERR_NOT_FORMAT;
  pArgu=pArgu->next;
  if (!pArgu)
    return ERR_SYNTAX;
  strcpy(strFullName,pArgu->strArgu);
  if (!pArgu->next)
    return ERR_SYNTAX;
  pArgu=pArgu->next;
  if (strcmp(pArgu->strArgu,"r")!=0 && strcmp(pArgu->strArgu,"w")!=0 && strcmp(pArgu->strArgu,"a")!=0)
    return ERR_MODE_INVALID;
  if (pArgu->next)
    return ERR_SYNTAX;
  iErrCode=CheckOpen(strFullName,pArgu->strArgu[0]);
  if (iErrCode!=ERR_SUCCESS)
    return iErrCode;
  pNew_File_Entry=new File_Table;
  pNew_File_Entry->fp=new File_Struct;
  pNew_File_Entry->fp->fd=Alloc_File_Handle();
  pNew_File_Entry->next=NULL;
  iErrCode=pVFS->OpenFile(pNew_File_Entry->fp,strFullName,pArgu->strArgu[0]);
  if (iErrCode==ERR_SUCCESS)
  {
     Add_File_Entry(pNew_File_Entry);
     printf("File is assigned to handle %d\n",pNew_File_Entry->fp->fd);
  }
  else
  {
    delete pNew_File_Entry->fp;
    delete pNew_File_Entry;
  }
  return iErrCode;
}

int CloseFile(Argument *pArgu)
{
  FILE_HANDLE fd;
  File_Struct *fp;
  File_Table *pTemp;
  int iErrCode=ERR_SUCCESS;
  if (!pVFS->IsFormated())
    return ERR_NOT_FORMAT;  
  pArgu=pArgu->next;
  if (!pArgu)
     return ERR_SYNTAX;
  if (strcmp(pArgu->strArgu,"all")==0)
  {
    if (pArgu->next)
      return ERR_SYNTAX;
    while (pFile_Table)
    {
      iErrCode=pBuf->Buffer_Flush(pFile_Table->fp);
      if (iErrCode!=ERR_SUCCESS)
        return iErrCode;
      iErrCode=pVFS->CloseFile(pFile_Table->fp);
      if (iErrCode!=ERR_SUCCESS)
        return iErrCode;
      pTemp=pFile_Table;
      pFile_Table=pFile_Table->next;
      delete pTemp->fp;
      delete pTemp;
    }
  }
  else
    while (pArgu)
    {
      fd=atoi(pArgu->strArgu);
      if (!fd)
        return ERR_HANDLE_INVALID;
      iErrCode=Find_File_Hanle(fd,&fp);
      if (iErrCode!=ERR_SUCCESS)
        return iErrCode;
      iErrCode=pBuf->Buffer_Flush(fp);
      if (iErrCode!=ERR_SUCCESS)
        return iErrCode;
      iErrCode=pVFS->CloseFile(fp);
      if (iErrCode!=ERR_SUCCESS)
        return iErrCode;
      Delete_File_Entry(fd);
      pArgu=pArgu->next;
    }
  return iErrCode;  
}

int Help(Argument *pArgu)
{
  int iErrCode=ERR_SUCCESS;
  pArgu=pArgu->next;
  if (!pArgu)
  {
    printf("Enter \"HELP command\" or \"MAN command\" for detailed information.\n");
    printf("CAT         Display the content of files.\n");
    printf("CD          Change the current directory.\n");
    printf("CHDIR       Change the current directory.\n");
    printf("CHMOD       Change the mode of files.\n");
    printf("CLOSE       Close opened files.\n");
    printf("COPY        Copy one file to the destination.\n");
    printf("CP          Copy one file to the destination.\n");
    printf("DEL         Delete one file.\n");
    printf("DF          Display the free capacity of the disk.\n");
    printf("DIR         Display files and subdiretories in the directory.\n");
    printf("EXIT        Quit this program.\n");
    printf("FORMAT      Format the disk.\n");
    printf("HELP        Provide help message of commands.\n");
    printf("LS          Display files and subdiretories in the directory.\n");
    printf("MAN         Provide help message of commands.\n");
    printf("MD          Make new directories.\n");
    printf("MKDIR       Make new directories.\n");
    printf("MOVE        Move a file from one directory to another.\n");
    printf("MV          Move a file from one directory to another.\n");
    printf("OPEN        Open a file.\n");
    printf("QUIT        Quit this program.\n");
    printf("RD          Remove directories.\n");
    printf("READ        Read the content of an opened file.\n");
    printf("RM          Delete one file.\n");
    printf("RMDIR       Remove directories.\n");
    printf("SEEK        Change the current position of an opened file.\n");
    printf("SHOWBUF     Show the buffer of the file.\n");
    printf("TYPE        Display the content of files.\n");
    printf("WRITE       Write text to a file.\n");
  }
  else
  {
    if (pArgu->next)
      return ERR_SYNTAX;
    strlwr(pArgu->strArgu);
    if (strcmp(pArgu->strArgu,"cat")==0 || strcmp(pArgu->strArgu,"type")==0)
    {
      printf("Display the content of files.\n\n");
      printf("%s [path]<filename>\n",strupr(pArgu->strArgu));
    }
    else
    if (strcmp(pArgu->strArgu,"cd")==0 || strcmp(pArgu->strArgu,"chdir")==0)
    {
      printf("Change the current directory.\n\n");
      printf("%s [path]\n",strupr(pArgu->strArgu));
    }
    else
    if (strcmp(pArgu->strArgu,"chmod")==0)
    {
      printf("Change the mode of files.\n\n");
      printf("CHMOD [path]<filename> <attribute>\n");
      printf("  attribute   r read-only\n");
      printf("              w write-only\n");
      printf("              rw read-write\n");
    }
    else
    if (strcmp(pArgu->strArgu,"close")==0)
    {
      printf("Close opened files.\n\n");
      printf("CLOSE <file handle>/ALL\n");
    }
    else
    if (strcmp(pArgu->strArgu,"copy")==0 || strcmp(pArgu->strArgu,"cp")==0)
    {
      printf("Copy one file to the destination.\n\n");
      printf("%s <source> <destination>\n\n",strupr(pArgu->strArgu));
      printf("  source        Specified file to be copied.\n");
      printf("  destination   Specified directory or filename for the new file.\n");
    }
    else
    if (strcmp(pArgu->strArgu,"del")==0 || strcmp(pArgu->strArgu,"rm")==0)
    {
      printf("Delete at least one file.\n\n");
      printf("%s <filenames>\n\n",strupr(pArgu->strArgu));
    }
    else
    if (strcmp(pArgu->strArgu,"df")==0)
    {
      printf("Display the free capacity of the disk.\n\n");
      printf("DF\n");
    }
    else
    if (strcmp(pArgu->strArgu,"dir")==0 || strcmp(pArgu->strArgu,"ls")==0)
    {
      printf("Display files and subdiretories in the directory.\n\n");
      printf("%s [/P] [/L] [path][filename]\n\n",strupr(pArgu->strArgu));
      printf("  -L   Display detailed information of the file.\n");
      printf("  -P   Pause after displaying a screen.\n");
    }
    else
    if (strcmp(pArgu->strArgu,"exit")==0 || strcmp(pArgu->strArgu,"quit")==0)
    {
      printf("Quit this program.\n\n");
      printf("%s\n",strupr(pArgu->strArgu));
    }
    else
    if (strcmp(pArgu->strArgu,"format")==0)
    {
      printf("Format the disk.\n\n");
      printf("FORMAT\n");
    }
    else
    if (strcmp(pArgu->strArgu,"man")==0 || strcmp(pArgu->strArgu,"help")==0)
    {
      printf("Provide help message of commands.\n\n");
      printf("%s [command]\n",strupr(pArgu->strArgu));
    }
    else
    if (strcmp(pArgu->strArgu,"md")==0 || strcmp(pArgu->strArgu,"mkdir")==0)
    {
      printf("Make new directories.\n\n");
      printf("%s <path>\n",strupr(pArgu->strArgu));
    }
    else
    if (strcmp(pArgu->strArgu,"mv")==0 || strcmp(pArgu->strArgu,"move")==0)
    {
      printf("Move files from one directory to another and rename them.\n\n");
      printf("%s <source> <destination>\n\n",strupr(pArgu->strArgu));
      printf("  source        Specified files or directories to be moved.\n");
      printf("  destination   Specified directory for files to be moved to. If only one file\n");
      printf("                to be moved and renamed, a filename can be included.\n");
    }
    else
    if (strcmp(pArgu->strArgu,"open")==0)
    {
      printf("Open a file.\n\n");
      printf("Open [path]<filename> <mode>\n");
      printf("  mode  \'a\' for appending.\n");
      printf("        \'r\' for reading.\n");
      printf("        \'w\' for writing.\n");
    }
    else
    if (strcmp(pArgu->strArgu,"rd")==0 || strcmp(pArgu->strArgu,"rmdir")==0)
    {
      printf("Remove directories.\n\n");
      printf("%s <path>\n",strupr(pArgu->strArgu));
    }
     else
    if (strcmp(pArgu->strArgu,"read")==0)
    {
      printf("Read the content of an opened file.\n\n");
      printf("READ <file handle> <length>\n");
    }
    else
    if (strcmp(pArgu->strArgu,"write")==0)
    {
      printf("Write text to an opened file.\n\n");
      printf("WRITE <file handle>\n");
    }
    else
    if (strcmp(pArgu->strArgu,"seek")==0)
    {
      printf("Change the current position of an opened file.\n\n");
      printf("SEEK <file handle> <position>\n");
    }
    else
    if (strcmp(pArgu->strArgu,"showbuf")==0)
    {
      printf("Show the buffer of the file.\n\n");
      printf("SHOWBUF <file handle>\n");
    }
    else
      printf("Not supported command.\n");
 }
  return iErrCode;
}

int WriteToFile(Argument *pArgu)
{
  int iErrCode=ERR_SUCCESS,bExit=0,i,j,iLen=0,bEOL=0;
  FILE_HANDLE fd;
  File_Struct *fp;
  char *strContent,strLine[81],c;
  unsigned int iSize;
  if (!pVFS->IsFormated())
    return ERR_NOT_FORMAT;  
  pArgu=pArgu->next;
  if (!pArgu)
     return ERR_SYNTAX;
  fd=atoi(pArgu->strArgu);
  if (!fd)
    return ERR_HANDLE_INVALID;
  iErrCode=Find_File_Hanle(fd,&fp);
  if (iErrCode!=ERR_SUCCESS)
    return iErrCode;
  if (fp->mode=='r')
    return ERR_OPEN_READ;
  pArgu=pArgu->next;
  strContent=(char *)malloc(INPUT_BUFFER_SIZE);
  strContent[0]='\0';
  iSize=INPUT_BUFFER_SIZE;
  if (pArgu)
    while (pArgu)
    {
       if (strlen(strContent)+strlen(pArgu->strArgu)+1>=iSize)
         strContent=(char *)realloc(strContent,iSize+INPUT_BUFFER_SIZE);
       strcat(strContent,pArgu->strArgu);
       strcat(strContent,"\n");
       pArgu=pArgu->next;
    }
  else
  {
    printf("Please input text (Ctrl-W to save,Ctrl-Q to discard):\n");
    i=0;
    while (!bExit)
    {
      if (bEOL)
        c='\r';
      else
        c=getch();
      switch (c)
      {
      case 8:
        if (i>0)
        {
          strLine[--i]='\0';
          putch('\r');
          for (j=1;j<=79;j++)
            putch(' ');
          printf("\r%s",strLine);
        }
        break;
      case 13:
        putch('\n');
        bEOL=0;
        strLine[i++]='\n';
        strLine[i]='\0';
        if (strlen(strContent)+i>=iSize)
          strContent=(char *)realloc(strContent,iSize+INPUT_BUFFER_SIZE);
        strcat(strContent,strLine);
        i=0;
        strLine[i]='\0';
        break;
      case 17:
        bExit=1;
        strContent[0]='\0';
        break;
      case 23:
        bExit=1;
        strLine[i]='\0';
        if (strlen(strContent)+i+1>=iSize)
          strContent=(char *)realloc(strContent,iSize+INPUT_BUFFER_SIZE);
        strcat(strContent,strLine);
        strcat(strContent,"\n");
        break;
      default:
        if (c>=32 && c<127)
          if (i>=79)
          {
            ungetch(c);
            bEOL=1;
          }
          else
          {
            putch(c);
            strLine[i++]=c;
          }
      }
    }
  }
  if (strlen(strContent)==0)
    iLen=0;
  else
    iErrCode=pBuf->Buffer_Write(fp,(Byte *)strContent,strlen(strContent),iLen);
  if (iErrCode==ERR_SUCCESS)
    printf("\n\n%d bytes has been write to the file.\n",iLen);
  return iErrCode;
}

int ReadFromFile(Argument *pArgu)
{
  int iErrCode=ERR_SUCCESS,i,iOffset,iLen;
  FILE_HANDLE fd;
  File_Struct *fp;
  char *strContent;
  if (!pVFS->IsFormated())
    return ERR_NOT_FORMAT;  
  pArgu=pArgu->next;
  if (!pArgu)
     return ERR_SYNTAX;
  fd=atoi(pArgu->strArgu);
  if (!fd)
    return ERR_HANDLE_INVALID;
  iErrCode=Find_File_Hanle(fd,&fp);
  if (iErrCode!=ERR_SUCCESS)
    return iErrCode;
  if (fp->mode!='r')
    return ERR_OPEN_WRITE;
  pArgu=pArgu->next;
  if (!pArgu)
    return ERR_SYNTAX;
  iOffset=atoi(pArgu->strArgu);
  if (iOffset==0)
    return ERR_SYNTAX;
  if (fp->Base_Pos+iOffset>fp->File_Length)
    iOffset=fp->File_Length-fp->Base_Pos;
  strContent=new char[iOffset+1];
  iErrCode=pBuf->Buffer_Read(fp,(Byte *)strContent,iOffset,iLen);
  if (iErrCode!=ERR_SUCCESS)
    return iErrCode;
  for (i=0;i<iLen;i++)
    if (strContent[i]=='\0')
      strContent[i]=' ';
  strContent[i]='\0';
  printf("%s",strContent);
  return iErrCode;
}

int SeekFile(Argument *pArgu)
{
  int iErrCode=ERR_SUCCESS,iPos;
  FILE_HANDLE fd;
  File_Struct *fp;
  if (!pVFS->IsFormated())
    return ERR_NOT_FORMAT;  
  pArgu=pArgu->next;
  if (!pArgu)
     return ERR_SYNTAX;
  fd=atoi(pArgu->strArgu);
  if (!fd)
    return ERR_HANDLE_INVALID;
  iErrCode=Find_File_Hanle(fd,&fp);
  if (iErrCode!=ERR_SUCCESS)
    return iErrCode;
  pArgu=pArgu->next;
  if (!pArgu)
    return ERR_SYNTAX;
  iPos=atoi(pArgu->strArgu);
  if (!iPos)
    return ERR_POS_INVALID;
  if (pArgu->next)
    return ERR_SYNTAX;
  iErrCode=pVFS->Seek(fp,iPos-1);
  return iErrCode;
}

int ShowBuffer(Argument *pArgu)
{
  int iErrCode=ERR_SUCCESS;
  File_Struct *fp;
  FILE_HANDLE fd;
  if (!pVFS->IsFormated())
    return ERR_NOT_FORMAT;  
  pArgu=pArgu->next;
  if (!pArgu)
     return ERR_SYNTAX;
  fd=atoi(pArgu->strArgu);
  if (!fd)
    return ERR_HANDLE_INVALID;
  iErrCode=Find_File_Hanle(fd,&fp);
  if (iErrCode!=ERR_SUCCESS)
    return iErrCode;
  pBuf->Show_Buffer(fd);
  return ERR_SUCCESS;
}

int DiskFree(Argument *pArgu)
{
  int iErrCode=ERR_SUCCESS;
  return iErrCode;
}

int ListDir(Argument *pArgu)
{
  int iErrCode=ERR_SUCCESS,bLarge=0,bPause=0;
  char strPath[MAX_NAME_LENGTH],strFileName[MAX_NAME_LENGTH];
  string Path,FileName;
	struct tagParsePatternRes PatRes;
	struct tagListDirRes ListDirRes;  
  i_DirIterator iter;
  if (!pVFS->IsFormated())
    return ERR_NOT_FORMAT;  
  pArgu=pArgu->next;
  while (pArgu)
  {
    if (pArgu->strArgu[0]=='-')
    {
      if (strcmp(pArgu->strArgu+1,"p")==0)
        bPause=1;
      else
      if (strcmp(pArgu->strArgu+1,"l")==0)
        bLarge=1;
      else
        return ERR_SYNTAX;
    }
    else
      break;
    pArgu=pArgu->next;
  }
  if (!pArgu)
  {
    strcpy(strPath,pVFS->GetCurrentPath());
    strcpy(strFileName,"*.*");
    //Path=strPath;
    //FileName=strFileName;
    pVFS->ParsePattern(strFileName,PatRes);
    pVFS->ListDir(strPath,PatRes,ListDirRes);

    ListDirRes.strListDirPath = new char[MAX_PATH_LENGTH];
    ListDirRes.strListDirPath = strPath;
    printf("Path: %s\n\n",ListDirRes.strListDirPath);
  	for (iter=ListDirRes.DirContainer.begin();iter!=ListDirRes.DirContainer.end();iter++)
    {

      //printf("%s    ",static_cast<tagDirItemRes>(*iter).timeLastWrite);
	  time_t tTime = static_cast<tagDirItemRes>(*iter).timeLastWrite;
//	  printf("Time and date:\t\t\t%s", ctime(&tTime));
	  char* ptime=ctime(&tTime);
	  ptime[strlen(ptime)-1]='\0';
	  printf("%s", ptime);
      if (static_cast<tagDirItemRes>(*iter).bIsFile) 
        printf(" %12d",static_cast<tagDirItemRes>(*iter).iItemLen);
      else
        printf("<DIR>        ");
      if (strlen(static_cast<tagDirItemRes>(*iter).strItemExtName)!=0)
        printf(" %s.%s\n",static_cast<tagDirItemRes>(*iter).strItemName,static_cast<tagDirItemRes>(*iter).strItemExtName);
      else
        printf(" %s\n",static_cast<tagDirItemRes>(*iter).strItemName);       
    }
    printf("%12d files %12d bytes\n",ListDirRes.iFileCount,ListDirRes.iTotalBytesUsed);
    printf("%12d dirs  %12d bytes available\n",ListDirRes.iDirCount,ListDirRes.iTotalBytesAvail);
  }
  else
    ;
  return iErrCode;
}

int CopyFiles(Argument *pArgu)
{
  int iErrCode=ERR_SUCCESS;
  char strSource[MAX_NAME_LENGTH],strDest[MAX_NAME_LENGTH],strTemp[MAX_NAME_LENGTH];
  char strSourceName[MAX_NAME_LENGTH],strDestName[MAX_NAME_LENGTH];
  char cChoice;
  if (!pVFS->IsFormated())
    return ERR_NOT_FORMAT;  
  pArgu=pArgu->next;
  if (!pArgu)
     return ERR_SYNTAX;
  strcpy(strSource,pArgu->strArgu);
  pArgu=pArgu->next;
  if (!pArgu)
    return ERR_SYNTAX;
  strcpy(strDest,pArgu->strArgu);
  if (pArgu->next)
    return ERR_SYNTAX;
  if (!strrchr(strSource,'/'))
  {
    strcpy(strSourceName,strSource);
    iErrCode=Check_Name(strSourceName);
  }
  else
  {
    strcpy(strSourceName,strrchr(strSource,'/')+1);
    if (strlen(strSourceName)==0)
       return ERR_NAME_INVALID;
  }
  if (iErrCode!=ERR_SUCCESS)
    return iErrCode;
  if (!strrchr(strDest,'/'))
  {
    strcpy(strDestName,strDest);
    iErrCode=Check_Name(strDestName);
  }
  else
  {
    strcpy(strDestName,strrchr(strDest,'/')+1);
    if (strlen(strDestName)==0)
      strcat(strDest,strSourceName);
  }
  if (iErrCode!=ERR_SUCCESS)
    return iErrCode;
  iErrCode=CheckOpen(strSource,'r');
  if (iErrCode!=ERR_SUCCESS)
    return iErrCode;
  iErrCode=CheckOpen(strSource,'w');
  if (iErrCode!=ERR_SUCCESS && iErrCode!=ERR_FILE_NOT_FOUND)
    return iErrCode;
  iErrCode=pVFS->CopyFiles(strSource,strDest);
  if (iErrCode==EXC_FILE_EXIST)
  {
    printf("%s exists,overwrite it(Y/N)?",strDestName);
    while(1)
    {
      cChoice=getch();
      if (cChoice=='y' || cChoice=='Y')
      {
        printf("Y\n");
        iErrCode=pVFS->RemoveFile(strDest);
        if (iErrCode!=ERR_SUCCESS)
          return iErrCode;
        iErrCode=pVFS->CopyFiles(strSource,strDest);
        break;
      }
      if (cChoice=='n' || cChoice=='N')
      {
        printf("N\n");
        break;
      }
    }
  }
  return iErrCode;
}

int RemoveFile(Argument *pArgu)
{
  int iErrCode=ERR_SUCCESS;
  if (!pVFS->IsFormated())
    return ERR_NOT_FORMAT;  
  pArgu=pArgu->next;
  if (!pArgu)
     return ERR_SYNTAX;
  while (pArgu)
  {
    iErrCode=CheckOpen(pArgu->strArgu,'w');
    if (iErrCode!=ERR_SUCCESS)
      return iErrCode;
    iErrCode=pVFS->RemoveFile(pArgu->strArgu);
    if (iErrCode!=ERR_SUCCESS)
      return iErrCode;
    pArgu=pArgu->next;
  }
  return iErrCode;
}

int MoveFiles(Argument *pArgu)
{
  int iErrCode=ERR_SUCCESS;
  char strSource[MAX_NAME_LENGTH],strDest[MAX_NAME_LENGTH],strTemp[MAX_NAME_LENGTH];
  char strSourceName[MAX_NAME_LENGTH],strDestName[MAX_NAME_LENGTH];
  char cChoice;
  if (!pVFS->IsFormated())
    return ERR_NOT_FORMAT;  
  pArgu=pArgu->next;
  if (!pArgu)
     return ERR_SYNTAX;
  strcpy(strSource,pArgu->strArgu);
  pArgu=pArgu->next;
  if (!pArgu)
    return ERR_SYNTAX;
  strcpy(strDest,pArgu->strArgu);
  if (pArgu->next)
    return ERR_SYNTAX;
  if (!strrchr(strSource,'/'))
  {
    strcpy(strSourceName,strSource);
    iErrCode=Check_Name(strSourceName);
  }
  else
  {
    if (strlen(strSource)==1)
      return ERR_NAME_INVALID;
    strcpy(strSourceName,strrchr(strSource,'/')+1);
    if (strlen(strSourceName)==0)
    {
      strSourceName[strlen(strSourceName)-1]='\0';
      strcpy(strSourceName,strrchr(strSource,'/')+1);
    }
  }
  if (iErrCode!=ERR_SUCCESS)
    return iErrCode;
  if (!strrchr(strDest,'/'))
  {
    strcpy(strDestName,strDest);
    iErrCode=Check_Name(strDestName);
  }
  else
  {
    strcpy(strDestName,strrchr(strDest,'/')+1);
    if (strlen(strDestName)==0)
      strcat(strDest,strSourceName);
  }
  if (iErrCode!=ERR_SUCCESS)
    return iErrCode;
  iErrCode=CheckOpen(strSource,'r');
  if (iErrCode!=ERR_SUCCESS && iErrCode!=ERR_FILE_NOT_FOUND)
    return iErrCode;
  iErrCode=CheckOpen(strDest,'w');
  if (iErrCode!=ERR_SUCCESS && iErrCode!=ERR_FILE_NOT_FOUND)
    return iErrCode;
  iErrCode=pVFS->MoveFiles(strSource,strDest);
  if (iErrCode==EXC_FILE_EXIST)
  {
    printf("%s exists,overwrite it(Y/N)?",strDestName);
    while(1)
    {
      cChoice=getch();
      if (cChoice=='y' || cChoice=='Y')
      {
        printf("Y\n");
        iErrCode=pVFS->RemoveFile(strDest);
        if (iErrCode!=ERR_SUCCESS)
          return iErrCode;
        iErrCode=pVFS->MoveFiles(strSource,strDest);
        break;
      }
      if (cChoice=='n' || cChoice=='N')
      {
        printf("N\n");
        break;
      }
    }
  }
  return iErrCode;
}

int TypeFile(Argument *pArgu)
{
  File_Struct *fp;
  char *strContent;
  int iErrCode=ERR_SUCCESS,iLen,i;
  if (!pVFS->IsFormated())
    return ERR_NOT_FORMAT;  
  pArgu=pArgu->next;
  if (!pArgu)
    return ERR_SYNTAX;
  while (pArgu && iErrCode==ERR_SUCCESS)
  {
    iErrCode=CheckOpen(pArgu->strArgu,'r');
    if (iErrCode!=ERR_SUCCESS)
      return iErrCode;
    fp=new File_Struct;
    fp->fd=Alloc_File_Handle();
    iErrCode=pVFS->OpenFile(fp,pArgu->strArgu,'r');
    if (iErrCode!=ERR_SUCCESS)
      return iErrCode;
    strContent=new char[fp->File_Length];
    iErrCode=pVFS->ReadFile(fp,(Byte *)strContent,fp->File_Length,iLen);
    if (iErrCode!=ERR_SUCCESS && iErrCode!=EXC_END_OF_FILE)
      return iErrCode;
    for (i=0;i<fp->File_Length;i++)
      if (!strContent[i])
        putch(' ');
    else
      putch(strContent[i]);
    iErrCode=pVFS->CloseFile(fp);
    if (iErrCode!=ERR_SUCCESS)
      return iErrCode;
    pArgu=pArgu->next;
  }
  return iErrCode;

}

int ChangeMode(Argument *pArgu)
{
  int iErrCode=ERR_SUCCESS;
  int iMode;
  char strFileName[MAX_NAME_LENGTH];
  if (!pVFS->IsFormated())
    return ERR_NOT_FORMAT;  
  pArgu=pArgu->next;
  if (!pArgu)
    return ERR_SYNTAX;
  strcpy(strFileName,pArgu->strArgu);
  pArgu=pArgu->next;
  if (!pArgu)
    return ERR_SYNTAX;
  if (strcmp(pArgu->strArgu,"r")==0)
    iMode=FILE_READ_ONLY;
  else
  if (strcmp(pArgu->strArgu,"w")==0)
    iMode=FILE_WRITE_ONLY;
  else
  if (strcmp(pArgu->strArgu,"rw")==0)
    iMode=FILE_READ_WRITE;
  else
    return ERR_ATTRI_NOT_SUPPORT;
  iErrCode=CheckOpen(strFileName,'r');
  if (iErrCode==ERR_ACCESS_DENIED)
    iErrCode=CheckOpen(strFileName,'w');
  if (iErrCode!=ERR_SUCCESS)
    return iErrCode;
  iErrCode=pVFS->ChangeMode(strFileName,iMode);
  return iErrCode;
}

int CommandParser(void)
{
  Argument *pArgu,*pCurArgu,*pTemp;
  int bExit,iErrCode;
  bExit=0;
  pArgu=pCurArgu=pTemp=NULL;
  while (bExit==0)
  {
    printf("%s>",pVFS->GetCurrentPath());
    InputCommand(&pArgu);
    if (pArgu)
    {
      if (strcmp(pArgu->strArgu,"format")==0)
        iErrCode=Format(pArgu);
      else
  		if (strcmp(pArgu->strArgu,"quit")==0 || strcmp(pArgu->strArgu,"exit")==0)
        iErrCode=Quit(pArgu,bExit);
      else
      if (strcmp(pArgu->strArgu,"md")==0 || strcmp(pArgu->strArgu,"mkdir")==0)
        iErrCode=MakeDir(pArgu);
      else
      if (strcmp(pArgu->strArgu,"rd")==0 || strcmp(pArgu->strArgu,"rmdir")==0)
        iErrCode=RemoveDir(pArgu);
      else
      if (strcmp(pArgu->strArgu,"open")==0)
        iErrCode=OpenFile(pArgu);
      else
      if (strcmp(pArgu->strArgu,"close")==0)
        iErrCode=CloseFile(pArgu);
      else
      if (strcmp(pArgu->strArgu,"man")==0 || strcmp(pArgu->strArgu,"help")==0)
        iErrCode=Help(pArgu);
      else
      if (strcmp(pArgu->strArgu,"write")==0)
        iErrCode=WriteToFile(pArgu);
      else
      if (strcmp(pArgu->strArgu,"read")==0)
        iErrCode=ReadFromFile(pArgu);
      else
      if (strcmp(pArgu->strArgu,"showbuf")==0)
        iErrCode=ShowBuffer(pArgu);
      else
      if (strcmp(pArgu->strArgu,"seek")==0)
        iErrCode=SeekFile(pArgu);
      else
      if (strcmp(pArgu->strArgu,"cd")==0 || strcmp(pArgu->strArgu,"chdir")==0)
        iErrCode=ChangeDir(pArgu);
      else
      if (strcmp(pArgu->strArgu,"df")==0)
        iErrCode=DiskFree(pArgu);
      else
      if (strcmp(pArgu->strArgu,"ls")==0 || strcmp(pArgu->strArgu,"dir")==0)
        iErrCode=ListDir(pArgu);
      else
      if (strcmp(pArgu->strArgu,"cp")==0 || strcmp(pArgu->strArgu,"copy")==0)
        iErrCode=CopyFiles(pArgu);
      else
      if (strcmp(pArgu->strArgu,"rm")==0 || strcmp(pArgu->strArgu,"del")==0)
        iErrCode=RemoveFile(pArgu);
      else
      if (strcmp(pArgu->strArgu,"cat")==0 || strcmp(pArgu->strArgu,"type")==0)
        iErrCode=TypeFile(pArgu);
      else
      if (strcmp(pArgu->strArgu,"mv")==0 || strcmp(pArgu->strArgu,"move")==0)
        iErrCode=MoveFiles(pArgu);
      else
      if (strcmp(pArgu->strArgu,"chmod")==0)
        iErrCode=ChangeMode(pArgu);
      else
        iErrCode=ERR_BAD_COMMAND;
    }
    if (iErrCode!=ERR_SUCCESS)
      DisplayErrMsg(iErrCode);
    if (pArgu)
      printf("\n");
    pCurArgu=pArgu;
    while (pCurArgu)
    {
      pTemp=pCurArgu;
      pCurArgu=pCurArgu->next;
      delete pTemp;
    }
  }
  return 0;
}


// 函数名： Shutdown
// 功能： Do some cleaning work before the system is completely shut down.
// 输入： None
// 输出： Related messages.
// 返回值： 0 represents success. Positive value represents error.          
// 备注： None

int Shutdown(void)
{
  File_Table *pTemp;
  printf("\nSystem is going to shut down,Please wait\n");
  printf("\nClosing opened files......");
  while (pFile_Table)
  {
    pBuf->Buffer_Flush(pFile_Table->fp);
    pVFS->CloseFile(pFile_Table->fp);
    pTemp=pFile_Table;
    pFile_Table=pFile_Table->next;
    delete pTemp->fp;
    delete pTemp;
  }
  printf("Done\n");
  delete pBuf;
  printf("Unmount virtual disk......");
  delete pVFS; 
	printf("Done\n");
  printf("\nYou can turn off power safely.\n\n");
  return 0;
}

// Main Function of the virtual file system.

int main(int argc, char* argv[])
{
	// Initialize the system.
	if( argc < 2 ){
		printf("No System Parameters provided\n");
		return 1;
	}
	
	strcpy(DiskFile,argv[1]);
	DiskSize=atoi(argv[2]);
	BlockSize=atoi(argv[3]);
	FATBlock=atoi(argv[4]);
	
  if (Init()!=INIT_SUCCESS) return 1;

	// Parse commands user inputed.
	CommandParser();

	// Shutdown the system.
	Shutdown();
	
	return 0;
}



