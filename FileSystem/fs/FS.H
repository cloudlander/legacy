
//----------------------------------------------------------------

//////////////////////////////////////////////////////////////////
// �ļ�����		FS.h
// �����ߣ�		icelx
// ʱ�䣺		2001.12.16.
// ���ݣ�		��������ʵ�ֻ������ļ�ϵͳ�Ĺ���
//				�ṩ��ʽ�����½��ļ���Ŀ¼��д�ļ��Ľӿں���
// �޸ģ�		pentium, rick
// �޸����ݣ�	��DirItem�ͳ����Ƴ�
//				�����ļ�������չ�����ȳ���
//				������һЩ��Ա�����������˽ṹ
//				����һЩ��Ա����
//				����FAT����             
//////////////////////////////////////////////////////////////////

#ifndef _FS_H
#define _FS_H

#include <ctime>
#include <list>
#include <string>

#include "FS_Kernel.h"
#include "DirItem.h"
#include "FileStruct.h"
#include "ErrorDef.h"

using namespace std;

const int MAX_PATH_LENGTH = 100;		// Max path length for parse

struct tagDirItemRes				// ListDir Directory Item Res
{
	char* strItemName;		     	// DirItem Name
	char* strItemExtName;			// DirItem ExtName
	bool   bIsFile;				    // True for file, false for directory
	Byte*  pAttri;					// Directory or file Attribute
	time_t timeLastWrite;		    // Time for last modification
	int    iItemLen;				// DirItem Length
};

typedef list<struct tagDirItemRes> i_DirContainer;		// DirItem List Iterator
typedef i_DirContainer::iterator i_DirIterator;

struct tagListDirRes				// ListDir command result @Rick
{
	char* strListDirPath;			// ListDir Path
    i_DirContainer DirContainer;	// Directory Item Container List
	int   iTotalBytesAvail;		// Total bytes available in disk
	int    iFileCount;			    // Total file count
	int    iDirCount;				// Total directory count
	int	   iTotalBytesUsed;			// Total bytes used in disk	
};

struct tagParsePatternRes			// ParsePattern result @Rick
{
	bool	bIsNameAster;			// Name only has asterisk "*"
	bool	bIsExtAster;			// Extname only has asterisk "*"
	short	sNamePattPos;			// Pattern (not "*") posisiton in name, 0 - head(e.g. "abc*"), 1 - tail(e.g. "*abc"), 2 - no asterisk (e.g. "abc")
	short	sExtPattPos;			// Pattern (not "*") position in extname, 0 - head(e.g. "abc*"), 1 - tail(e.g. "*abc"), 2 - no asterisk (e.g. "abc")
	string	strNamePatt;			// Pattern (not "*") in name
	string	strExtPatt;				// Pattern (not "*") in extname
};

class FS
{
public: //compile 
//protected:
//  �ļ�ϵͳ���Ĳ��� {
	int iBlockSize;
	int iMaxBlockNum;				//�ܹ�����
	int iFATBlock,iBlockNumSize;    //FAT��ռ�Ŀ���,�����ռ�ֽ���
	int iFile_DirNameLen,iFileExtNameLen;
	int iDirItemSize;
	int iErrorCode;

   // string VolumeName;				// Volume Name -- Need to be implemented! @Rick

	DirItem CurrentPath;            // ��ǰĿ¼
	char Path[MAX_PATH];
	
	int autoSaveTimeout;            //  �Զ�����FAT�ļ��
	                                //  <# pentium #>
	FS_Kernel *pFS_Kernel;
	
	Byte *FAT;						// FAT������ָ��
//  }

//  �ļ�ϵͳ���ĺ��� {	
	int FS_InitialDir(int DirFirstBlock,int PareDirFirstBlock);
	int FS_FindEmptyDirItem(int DirFirstBlock,int& BlockNum,int& ByteOffset);
	int FS_NewDirItem(DirItem& NewDirItem,int DirFirstBlock );
	int FS_RemoveDirItem(char* , int,int);   // ɾ��Ŀ¼��
	int FS_ReadFATTable();
	int FS_WriteFATTable();
	int FS_WriteFAT(int BlockNum,int InputData);
	int FS_ReadFAT(int BlockNum,int& OutputData);
	int FS_ReqNewBlock(int& BlockNum,int PreBlockNum,int* fsFirstBlock=NULL);
	int FS_ReleaseBlock(int BlockNum,int data);    // �ͷſ�
	int FS_FindNextBlock(int CurrentBlock,int& NextBlock);
	int FS_IsDirEmpty(int BlockNum);               //  ���Ŀ¼�Ƿ�Ϊ��
											       //  ��RemoveDirʹ��
	int FS_IsDirValid(char*,DirItem*,int&);             //  ���Ŀ¼�Ƿ����

//  }

//  �ļ�·�������������� { ( �ⲿ���ɼ� )
//  Ŀ¼Ԥ����, �������ڿ��
//  PreProcessPathȷʵ�ӿڲ��á� ��ʱ���¡�
	int PreProcessPath(char* ,int iFirstBlock, int& , int &);    
	int ParsePath(char*,char*,int);            // ��ȡ·��
	int FindCurPath(char* FileName,int op,int isFile,int iBlock,int& iNextBlock,File_Struct*,DirItem* =NULL);    
	// �ڵ�ǰ·���������ļ�
	int Level(char* Path);         // ��·������
	void SplitFileName(char* ,char* , char* );  // ���ļ�����,���Ƚ�
	void strsub(char* );             // sub the last '/'
//  }

public:
	FS(char* strVDFileName,int DiskSize,int BlockSize,int FATBlock);
	~FS();
	
	int FS_Format();
	int FS_SetAutoSaveTimeOut(int);             // �����Զ�������
	int FS_GetAutoSaveTimeOut();             // ��ȡ�Զ�������

//  �ļ�ϵͳ�ӿ�{ 
//  ���·����漰��File_Struct�ṹָ��ģ�
//  ���뱣ָ֤�벻ָ���λ��, ������ʱ�����
	int Seek(File_Struct* fs,int pos);                 // �ƶ��ļ���׼λ��
	int CreatNewFile(File_Struct*,int mode,char* ,int);   // �ⲿ�����ã�( ��OpenFile���� )               // �½�һ�����ļ�
	int ReadFile(File_Struct*, Byte*,int,int&);               // ��ȡ�ļ�����
	int WriteFile(File_Struct*, Byte*,int,int&);              // д�ļ�
	int OpenFile(File_Struct* ,char* ,char mode,int bCreatNew=1);           // ���ļ�
	int CloseFile(File_Struct*);                           // �ر��ļ�
	int ChangeMode(char* FilePath,int mode);          // �ı��ļ�����
	int RemoveFile(char*);                                 // ɾ���ļ�
	int CopyFiles(char* Source,char* Dest);                 // �����ļ�
	int MoveFiles(char* Source,char* Dest);               //  �ƶ��ļ�
	int MakeDir(char*,int bReqNewBlock=1);                                    // ����Ŀ¼
	int RemoveDir(char*);                                  // ɾ��Ŀ¼
	int ChangeDir(char*);                                  // �ı䵱ǰĿ¼
	char* GetCurrentPath();                 // ȡ��ǰ·��ȫ·���ַ���
	int IsFormated();

	int ParsePattern(string strPattern, struct tagParsePatternRes& PatternRes);	// Parse the pattern received
  int ListDir(string strPath, struct tagParsePatternRes PatRes, struct tagListDirRes& ListDirRes);		// List directory
	int GetItemInfoList(char* strPath, struct tagListDirRes& ListDirRes, struct tagParsePatternRes PatRes, bool bHasPattern, bool bIsFile, bool bHasExt);	
	
	int CopyDirItem(Byte* pTempData, struct tagListDirRes& ListDirRes, bool bHasExt);		// Copy Dir Item to List
	bool IsPatternMatch(Byte* pTempData, struct tagParsePatternRes Pat);		// Pattern Match or not

};

#endif

//----------------------------------------------------------------------------