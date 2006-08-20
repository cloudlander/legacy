
//----------------------------------------------------------------

//////////////////////////////////////////////////////////////////
// 文件名：		FS.h
// 创建者：		icelx
// 时间：		2001.12.16.
// 内容：		此类用于实现基本的文件系统的功能
//				提供格式化，新建文件，目录，写文件的接口函数
// 修改：		pentium, rick
// 修改内容：	将DirItem和常数移出
//				增加文件名和扩展名长度常量
//				添加了一些成员变量及调整了结构
//				添加一些成员函数
//				添加FAT数组             
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
//  文件系统核心参数 {
	int iBlockSize;
	int iMaxBlockNum;				//总共块数
	int iFATBlock,iBlockNumSize;    //FAT所占的块数,块号所占字节数
	int iFile_DirNameLen,iFileExtNameLen;
	int iDirItemSize;
	int iErrorCode;

   // string VolumeName;				// Volume Name -- Need to be implemented! @Rick

	DirItem CurrentPath;            // 当前目录
	char Path[MAX_PATH];
	
	int autoSaveTimeout;            //  自动保存FAT的间隔
	                                //  <# pentium #>
	FS_Kernel *pFS_Kernel;
	
	Byte *FAT;						// FAT表数组指针
//  }

//  文件系统核心函数 {	
	int FS_InitialDir(int DirFirstBlock,int PareDirFirstBlock);
	int FS_FindEmptyDirItem(int DirFirstBlock,int& BlockNum,int& ByteOffset);
	int FS_NewDirItem(DirItem& NewDirItem,int DirFirstBlock );
	int FS_RemoveDirItem(char* , int,int);   // 删除目录项
	int FS_ReadFATTable();
	int FS_WriteFATTable();
	int FS_WriteFAT(int BlockNum,int InputData);
	int FS_ReadFAT(int BlockNum,int& OutputData);
	int FS_ReqNewBlock(int& BlockNum,int PreBlockNum,int* fsFirstBlock=NULL);
	int FS_ReleaseBlock(int BlockNum,int data);    // 释放块
	int FS_FindNextBlock(int CurrentBlock,int& NextBlock);
	int FS_IsDirEmpty(int BlockNum);               //  检查目录是否为空
											       //  供RemoveDir使用
	int FS_IsDirValid(char*,DirItem*,int&);             //  检查目录是否存在

//  }

//  文件路径处理辅助函数 { ( 外部不可见 )
//  目录预处理, 返回所在块号
//  PreProcessPath确实接口不好。 暂时留下。
	int PreProcessPath(char* ,int iFirstBlock, int& , int &);    
	int ParsePath(char*,char*,int);            // 提取路径
	int FindCurPath(char* FileName,int op,int isFile,int iBlock,int& iNextBlock,File_Struct*,DirItem* =NULL);    
	// 在当前路径下搜索文件
	int Level(char* Path);         // 求路径层数
	void SplitFileName(char* ,char* , char* );  // 将文件名拆开,供比较
	void strsub(char* );             // sub the last '/'
//  }

public:
	FS(char* strVDFileName,int DiskSize,int BlockSize,int FATBlock);
	~FS();
	
	int FS_Format();
	int FS_SetAutoSaveTimeOut(int);             // 设置自动保存间隔
	int FS_GetAutoSaveTimeOut();             // 读取自动保存间隔

//  文件系统接口{ 
//  以下凡是涉及到File_Struct结构指针的，
//  必须保证指针不指向空位置, 函数暂时不检查
	int Seek(File_Struct* fs,int pos);                 // 移动文件基准位置
	int CreatNewFile(File_Struct*,int mode,char* ,int);   // 外部不调用，( 由OpenFile调用 )               // 新建一个空文件
	int ReadFile(File_Struct*, Byte*,int,int&);               // 读取文件内容
	int WriteFile(File_Struct*, Byte*,int,int&);              // 写文件
	int OpenFile(File_Struct* ,char* ,char mode,int bCreatNew=1);           // 打开文件
	int CloseFile(File_Struct*);                           // 关闭文件
	int ChangeMode(char* FilePath,int mode);          // 改变文件属性
	int RemoveFile(char*);                                 // 删除文件
	int CopyFiles(char* Source,char* Dest);                 // 复制文件
	int MoveFiles(char* Source,char* Dest);               //  移动文件
	int MakeDir(char*,int bReqNewBlock=1);                                    // 创建目录
	int RemoveDir(char*);                                  // 删除目录
	int ChangeDir(char*);                                  // 改变当前目录
	char* GetCurrentPath();                 // 取当前路径全路径字符串
	int IsFormated();

	int ParsePattern(string strPattern, struct tagParsePatternRes& PatternRes);	// Parse the pattern received
  int ListDir(string strPath, struct tagParsePatternRes PatRes, struct tagListDirRes& ListDirRes);		// List directory
	int GetItemInfoList(char* strPath, struct tagListDirRes& ListDirRes, struct tagParsePatternRes PatRes, bool bHasPattern, bool bIsFile, bool bHasExt);	
	
	int CopyDirItem(Byte* pTempData, struct tagListDirRes& ListDirRes, bool bHasExt);		// Copy Dir Item to List
	bool IsPatternMatch(Byte* pTempData, struct tagParsePatternRes Pat);		// Pattern Match or not

};

#endif

//----------------------------------------------------------------------------