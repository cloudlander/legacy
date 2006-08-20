
//----------------------------------------------------------------------------

//////////////////////////////////////////////////////////
// Name:		FS.cpp
// Author:		icelx, rick, pentium
// Date:		2001.12.19.
// Function:	Implementation of tasks defined in FS.h
// Notes:
//////////////////////////////////////////////////////////
#include <stdio.h>
#include <stdlib.h>
#include "FS.h"

// Constructor with Virtual Disk Filename
// 修改: pentium
// 增加参数: DiskSize, BlockSize, FATBlcok
// 它们由前端程序提供，主函数调用

FS::FS(char* VDFileName,int DiskSize,int BlockSize,int FATBlock)
{
//	iBlockSize = 1024;
	iBlockSize = BlockSize;
//	iMaxBlockNum= 4*1024;
	iMaxBlockNum = DiskSize / BlockSize;
//	iFATBlock=8;
	iFATBlock = FATBlock;
	iFile_DirNameLen=File_Name_Len;
	iFileExtNameLen=File_ExtName_Len;
	iBlockNumSize=2;
	iDirItemSize=iFile_DirNameLen+iFileExtNameLen+DirItem_AttriLen
		 +DirItem_TimeDateLen+DirItem_FileLen+DirItem_BlockNum;
	pFS_Kernel=new FS_Kernel(VDFileName);


    // initialize the FAT
	FAT=new Byte[iMaxBlockNum*iBlockNumSize];

	// 在读取FAT之前，要检查VDF有没有格式化 @icelx
	int nRet=pFS_Kernel->IsFormated(DiskSize);
	if (nRet == 1)                      //  完整磁盘
		FS_ReadFATTable(); 
	else if( nRet == 2 )				//  尚未创建根目录
	{		
		FS_ReadFATTable(); 
		if (FS_InitialDir(iFATBlock
			,iFATBlock)==1)              //初始化根目录
		{
			iErrorCode=1;
			cout<<"initial root dir failed"<<endl;
		}	
	}else							    // 格式化磁盘
	{	
		iErrorCode=ERR_NOT_FORMAT;
//	    cout<<"not formated"<<endl;
	    FS_Format();  // compile @icelx;
	}	
	
	CurrentPath.Attri=new Byte [3];
	CurrentPath.strDFName=new char[17];

	ChangeDir("/");             // 当前目录是'/'

	FS_SetAutoSaveTimeOut(10);  // 设置自动保存间隔
}

int FS::IsFormated()
{
  return pFS_Kernel->bFormated;
}

// Destructor
FS::~FS()
{
	delete[] CurrentPath.Attri;
	delete[] CurrentPath.strDFName;

	if (FS_WriteFATTable())
	{
		//对FAT表写回出错处理 to do !!  @icelx
		cout<<"fatal error: writing FAT error"<<endl;
	}
	delete pFS_Kernel;
	delete FAT;
}

//////////////////////////////////////////////////////////
// 功能：	格式化当前虚拟磁盘，包括初始化根目录
// 输出：
// 返回值：	错误代码 0 - 正常；1 - 格式化失败
// 备注：
//////////////////////////////////////////////////////////

int FS::FS_Format()
{
	if ((*pFS_Kernel).Format_Disk(iBlockSize*iMaxBlockNum,iFATBlock)==1)   //格式化虚拟磁盘文件
	{
		iErrorCode=1;
		//cout<<"format_disk failed"<<endl;
		return 1;
	}
	if (FS_ReadFATTable())             
	{
		iErrorCode=1;
		return 1;
		
	}	
	if (FS_InitialDir(iFATBlock
		             ,iFATBlock)==1)              //初始化根目录
	{
		iErrorCode=1;
		//cout<<"initialdir failed"<<endl;
		return 1;
	}
	
	return 0;
}

//////////////////////////////////////////////////////////
// 函数名：		FS_InitialDir
// 功能：		对一个目录文件进行初始化
//				包括对目录名，创建时间日期，
//				两个系统目录(.和..)的添加
// 输入：		DirFirstBlock - 目录文件首块号
//				PareDirFirstBlock - 父目录首块号。
// 返回值：		0 - 正常，1 - 失败
// 备注：
// 修改：       pentium
//              为方便查找，文件名左对齐，不足处充0x00
//              支持空格，但空格不能是第一个 
//////////////////////////////////////////////////////////

int FS::FS_InitialDir(int DirFirstBlock,int PareDirFirstBlock)
{
    DirItem tempDir;
	char *temp,*tempp;
	tempDir.strDFName=new char[iFile_DirNameLen];
	temp=tempDir.strDFName;
	*temp='.';
	temp++;
	tempp=temp;
	for (int i=1;i<iFile_DirNameLen;i++,temp++)
		*temp=0x00;
//	temp--;
	tempDir.strExtName=new char[iFileExtNameLen];
	temp=tempDir.strExtName;
	for (i=0;i<iFileExtNameLen;i++,temp++)
		*temp=0x00;
	tempDir.Attri=new Byte[DirItem_AttriLen];
	Byte* tempByte=tempDir.Attri;
	for (i=0;i<DirItem_AttriLen;i++,tempByte++)
        *tempByte=0;
	*(tempDir.Attri)=1;     //设置属性位，表明这个目录项是子目录
	
	
	//设置其他属性添加在这里


	time(&(tempDir.Time_Date));
	tempDir.iFirstBlockNum=DirFirstBlock;
	tempDir.iFileLength=0;
	if (FS_NewDirItem(tempDir,DirFirstBlock))
		return 1;
	*tempp='.';
	tempDir.iFirstBlockNum=PareDirFirstBlock;
	if (FS_NewDirItem(tempDir,DirFirstBlock))
		return 1;
	delete tempDir.strDFName;
	delete tempDir.strExtName;
	delete tempDir.Attri;
	//写fat表,(在内存中的FAT)
	if (FS_WriteFAT(DirFirstBlock,0xFFFF))
	{
        //cout<<"Write fat failed"<<endl;
		return 1;
	}
    return 0;
}

//////////////////////////////////////////////////////////
// 函数名：		FS_NewDirItem
// 功能：		新建一个目录项，如果当前块已满，要申请新块
// 输入：		DirFirstBlock - 目录文件首块号
//				DirItem - 添加目录项的目录结构
// 返回值：		0 - 正常，1 - 失败, 2 - 磁盘满,针对申请新块的情况
// 备注：
//////////////////////////////////////////////////////////

int FS::FS_NewDirItem(DirItem& NewDirItem,int DirFirstBlock )
{
	Byte* pCurrentBlockData=new Byte[iBlockSize];
	Byte *temp=pCurrentBlockData;
	int iCurrentBlock=0,iOffset=0;
	int iTemp=FS_FindEmptyDirItem(DirFirstBlock,iCurrentBlock,iOffset);
	if (iTemp>1)
    {
		delete pCurrentBlockData;
		return 1;
	}
	if (pFS_Kernel->Read_Block(pCurrentBlockData,iCurrentBlock,iBlockSize))
	{
		delete pCurrentBlockData;
		return 1;
	}
	temp+=iOffset;
    memcpy(temp,NewDirItem.strDFName,iFile_DirNameLen);
	temp+=iFile_DirNameLen;
    memcpy(temp,NewDirItem.strExtName,iFileExtNameLen);
    temp+=iFileExtNameLen;
    memcpy(temp,NewDirItem.Attri,DirItem_AttriLen);
    temp+=DirItem_AttriLen;
    memcpy(temp,&NewDirItem.Time_Date,DirItem_TimeDateLen);
	temp+=DirItem_TimeDateLen;
    *temp=NewDirItem.iFirstBlockNum % 256;
	
    temp++;
    *temp=NewDirItem.iFirstBlockNum / 256;
	
    temp--;
    temp+=DirItem_BlockNum;
    memcpy(temp,&NewDirItem.iFileLength,DirItem_FileLen);
	
    if (pFS_Kernel->Write_Block(pCurrentBlockData,iCurrentBlock,iBlockSize))
	{
	    delete pCurrentBlockData;
	    return 1;
	}
    delete pCurrentBlockData;
	if (iTemp==1)   //FS_FindEmptyDirItem申请了新块
		return 2;		//  算是错误咯？ 但FS_FindEmptyDirItem不会返回1 
						//   <# pentium #>
	return 0;
}

//////////////////////////////////////////////////////////
// 函数名：		FS_RemoveDirItem    
// 创建者：     pentium
// 功能：		删除一个目录项，如果当前块已满，要申请新块
// 输入：		DirFirstBlock - 目录文件首块号
//              bfile - 是否是文件标识 1: 文件 0: 目录
//				DirItem - 目录项的目录结构
// 返回值：		0 - 正常，1 - 失败
// 备注：
//////////////////////////////////////////////////////////
int FS::FS_RemoveDirItem(char* FileName,int bfile,int DirFirstBlock )
{
	int temp;
	char Main[16];       // 文件主名或目录名
	char Ext[3];         // 文件扩展名
	Byte* pCurrentBlockData=new Byte[iBlockSize];
	Byte* tempdata=new Byte[iBlockSize];
	int iCurrentBlock=DirFirstBlock;
	int tempBlock;
	int i,iNewBlock,iTemp,ttemp,iOffset=0;
	int count=0;
	bool complete=false;
	if( bfile)
		SplitFileName(FileName,Main,Ext); // 拆分文件名
	else
		strcpy(Main,FileName);            // 保存目录名
	
	do{
		//搜索当前块的内容
		if (pFS_Kernel->Read_Block(pCurrentBlockData,iCurrentBlock,iBlockSize))
		{
			delete pCurrentBlockData;
			return 2;
		}
		memcpy(tempdata,pCurrentBlockData,iBlockSize);
		for (i=0;i<(int)iBlockSize/iDirItemSize;i++)
		{
			if ((((int)pCurrentBlockData[iOffset+(iFile_DirNameLen+iFileExtNameLen)]) % 4) == 0){
				count++;     // 空目录项记数
			}
			if (bfile && (((int)pCurrentBlockData[iOffset+(iFile_DirNameLen+iFileExtNameLen)]) % 4) == 2)      
			{	//文件的属性标志位均为1
				*(pCurrentBlockData+iOffset+19)='\0';
				if (0 == strcmp((char*)(pCurrentBlockData+iOffset+16),Ext)){  // 扩展名符合
					*(pCurrentBlockData+iOffset+16)='\0';
					if ( 0 == strcmp((char*)(pCurrentBlockData+iOffset),Main)){  // 文件名符合				
						//准备删除
					temp=0;
					memcpy(tempdata+iOffset+(iFile_DirNameLen+iFileExtNameLen),&temp,2);
					pFS_Kernel->Write_Block(tempdata,iCurrentBlock,iBlockSize);
					delete pCurrentBlockData;	
					delete tempdata;					
					count++;
					complete=true;
					}
				}
			}
			else if (!bfile && (((int)pCurrentBlockData[iOffset+(iFile_DirNameLen+iFileExtNameLen)]) % 2) == 1)
			{
				*(pCurrentBlockData+iOffset+16)='\0';
				if ( 0 == strcmp((char*)(pCurrentBlockData+iOffset),Main)){  // 目录名符合				
					//准备删除
					temp=0;
					memcpy(tempdata+iOffset+(iFile_DirNameLen+iFileExtNameLen),&temp,2);
					pFS_Kernel->Write_Block(tempdata,iCurrentBlock,iBlockSize);
					delete pCurrentBlockData;	
					delete tempdata;					
					count++;
					complete=true;
				}
			}	
			iOffset+=iDirItemSize;
		}
		ttemp=FS_FindNextBlock(iCurrentBlock,iNewBlock); //搜索余下的块
		
		if( count == (int)iBlockSize/iDirItemSize){  // 该块可以释放
			FS_WriteFAT(tempBlock,iNewBlock);
			FS_ReleaseBlock(iCurrentBlock,0);
		}
		
		iOffset=0;
		tempBlock=iCurrentBlock;   // 保留父块号
		iCurrentBlock=iNewBlock;
		
	}
	while (!complete);

}

//////////////////////////////////////////////////////////
// 函数功能：	检查目录是否为空
// 创建者:      pentium
// 输入：		BlockNum 为目录首块号
// 返回值：		0 -  空，1 -  不空   错误查ErrDef.h
////////////////////////////////////////////////////////// 
int FS::FS_IsDirEmpty(int BlockNum)
{
	int count=0;
	int i;
	Byte* tempdata=new Byte[iBlockSize];
	Byte* tttemp=tempdata;
	Byte* ttemp;			// for search
	do{
		pFS_Kernel->Read_Block(tempdata,BlockNum,iBlockSize);
		for(i=0;i<iBlockSize/iDirItemSize ;i++)
		{
			ttemp=tempdata+File_ExtName_Len+File_Name_Len;
			if ( 0 != ((int)*ttemp % 4) )  // 是文件或目录
				count++;
			tempdata+=iDirItemSize;
		}
		if(EXC_BLOCK_END == FS_FindNextBlock(BlockNum,BlockNum))
			break;
	}while(true);
	delete[] tttemp;
	if(count != 2)     // 目录非空
		return 1;
	else
		return 0;
}

/////////////////////////////////////////////////////////
// 函数功能:    设置自动保存FAT间隔
// 创建者:      pentium
/////////////////////////////////////////////////////////
int FS::FS_SetAutoSaveTimeOut(int TimeOuttoSet)
{
	if( TimeOuttoSet >= 250 )   // 上限是250
		return 1;
	autoSaveTimeout=TimeOuttoSet;
	return 0;
}


/////////////////////////////////////////////////////////
// 函数功能:    读取自动保存FAT间隔
// 创建者:      pentium
/////////////////////////////////////////////////////////
int FS::FS_GetAutoSaveTimeOut()
{
	return autoSaveTimeout;
}

//////////////////////////////////////////////////////////
// 函数功能：	改名当前目录
// 创建者:      pentium
// 输入：		DirPath 要进入的目录 ,必须以'/'结束,函数不检查!!!
// 返回值：		0 - 正常，错误查ErrDef.h
// 备注:        此函数仅记录当前目录首块号，
//              以方便其他接口函数使用
////////////////////////////////////////////////////////// 
char* FS::GetCurrentPath()
{
	return Path;
}

void FS::strsub(char* str)
{
	int len=strlen(str);
	for(int i=len-2;i>=0;i--){
		if( str[i] == '/' ){
			str[i+1]='\0';
			break;
		}
	}
}
//////////////////////////////////////////////////////////
// 函数功能：	改名当前目录
// 创建者:      pentium
// 输入：		DirPath 要进入的目录 ,必须以'/'结束,函数不检查!!!
// 返回值：		0 - 正常，错误查ErrDef.h
// 备注:        此函数仅记录当前目录首块号，
//              以方便其他接口函数使用
////////////////////////////////////////////////////////// 
int FS::ChangeDir(char* DirPath)
{
	char CurPath[File_Name_Len+File_ExtName_Len];  // 当前解析出的路径
	int nextBlock;
	int startBlock;
	int level;
	int i=0;
	int tempblock;
	char PathTemp[MAX_PATH];
	DirItem tempItem;
	tempItem.strDFName=new char[MAX_PATH];
	tempItem.Attri=new Byte[3];

	level=Level(DirPath);
	ParsePath(DirPath,CurPath,0);
	if(*CurPath == '\0')        // 是绝对路径
	{
		startBlock=iFATBlock;
		strcpy(PathTemp,"/");
		i=1;                    // 查下一层路径
	}
	else{                        // 是相对路径
		startBlock=CurrentPath.iFirstBlockNum;     //设置查找起始块号
		strcpy(PathTemp,Path);
	}
	
	for(;i<level-1;i++)
	{
		ParsePath(DirPath,CurPath,i);       // 开始搜索目录
		tempblock=startBlock;       // 记下目录首块号
		do{
			if (0 == FindCurPath(CurPath,2,0,startBlock,nextBlock,NULL,&tempItem)){
				if( 0 == strcmp(CurPath,"..") ){
					strsub(PathTemp);
				}
				else if( 0 != strcmp(CurPath,".")){
					strcat(PathTemp,tempItem.strDFName);
					strcat(PathTemp,"/");
				}
				break;
			}
			if ( EXC_BLOCK_END == FS_FindNextBlock(startBlock,startBlock))
				return ERR_DIR_NOT_FOUND;
		}while(true);
		startBlock=nextBlock;         // 继续下一层搜索
	}
	tempblock=startBlock;             // 保存目录项首块号
	ParsePath(DirPath,CurPath,level-1);       // 搜索目录
	if(*CurPath != '\0' || level != 1)        // 不是根目录
	{
		do{
			if (0 == FindCurPath(CurPath,2,0,startBlock,nextBlock,NULL,&CurrentPath)){
				if( 0 == strcmp(CurPath,"..") ){
					strsub(PathTemp);
				}
				else if ( 0 != strcmp(CurPath,".")){
					strcat(PathTemp,CurrentPath.strDFName);
					strcat(PathTemp,"/");
				}
				break;
			}
			if ( EXC_BLOCK_END == FS_FindNextBlock(startBlock,startBlock))
				return ERR_DIR_NOT_FOUND;           // 目录不存在	
		}while(true);
		CurrentPath.iFirstBlockNum=nextBlock;           // 记录当前目录首块号
		strcpy(Path,PathTemp);
	}
	else{
		CurrentPath.iFirstBlockNum=iFATBlock;
		strcpy(Path,DirPath);
	}
	return 0;
}

//////////////////////////////////////////////////////////
// 函数功能：	创建目录
// 创建者:      pentium
// 输入：		DirPath 要创建的目录 ,必须以'/'结束,函数不检查!!!
// 返回值：		0 - 正常，错误查ErrDef.h
////////////////////////////////////////////////////////// 
int FS::MakeDir(char* DirPath,int bReqNewBlock)
{
	char CurPath[File_Name_Len+File_ExtName_Len];  // 当前解析出的路径
	int nextBlock;
	int startBlock;
	int level;
	int i=0;
	int tempblock;
	bool bDirCreated=false;
	DirItem tempDir;
	level=Level(DirPath);
	ParsePath(DirPath,CurPath,0);
	if(*CurPath == '\0')        // 是绝对路径
	{
		startBlock=iFATBlock;
		i=1;                    // 查下一层路径
	}
	else                        // 是相对路径
		startBlock=CurrentPath.iFirstBlockNum;     //设置查找起始块号
	
	for(;i<level;i++)
	{
		ParsePath(DirPath,CurPath,i);       // 开始搜索目录
		tempblock=startBlock;       // 记下目录首块号
		do{
			if (0 == FindCurPath(CurPath,-1,0,startBlock,nextBlock,NULL))
				break;
			if ( EXC_BLOCK_END == FS_FindNextBlock(startBlock,startBlock))
			{
				// 准备创建目录
				char *temp;
				Byte* tempByte;
				tempDir.strDFName=new char[iFile_DirNameLen];
				temp=tempDir.strDFName;
				for (int i=0;i<iFile_DirNameLen;i++,temp++)
					*temp=0x00;
				strcpy(tempDir.strDFName,CurPath);
				tempDir.strExtName=new char[iFileExtNameLen];
				temp=tempDir.strExtName;
				for (i=0;i<iFileExtNameLen;i++,temp++)
					*temp=0x00;
				tempDir.Attri=new Byte[DirItem_AttriLen];
				tempByte=tempDir.Attri;
				for (i=0;i<DirItem_AttriLen;i++,tempByte++)
					*tempByte=0x00;
				*(tempDir.Attri)=1;     //设置属性位，表明这个目录项是子目录
				
				
				//设置其他属性添加在这里
				
				
				time(&(tempDir.Time_Date));
				if( bReqNewBlock && ERR_DISK_FULL == FS_ReqNewBlock(tempDir.iFirstBlockNum,0xFFFF))
				{
					delete[] tempDir.strDFName;
					delete[] tempDir.strExtName;
					delete[] tempDir.Attri;
					return ERR_DISK_FULL;
				}
				tempDir.iFileLength=0;
				if (FS_NewDirItem(tempDir,tempblock))
					return 1;		
				if( bReqNewBlock)
					FS_InitialDir(tempDir.iFirstBlockNum,tempblock);
				startBlock=tempblock;
				bDirCreated=true;
			}
		}while(true);
		
		startBlock=nextBlock;         // 继续下一层搜索
	}
	if(!bDirCreated) 
		return ERR_DIR_DUPLICATE;
	return 0;
}

//////////////////////////////////////////////////////////
// 函数功能：	删除目录
// 创建者:      pentium
// 输入：		DirPath 要删除的目录 ,必须以'/'结束,函数不检查!!!
// 返回值：		0 - 正常，错误查ErrDef.h
////////////////////////////////////////////////////////// 
int FS::RemoveDir(char* DirPath)
{
	char CurPath[File_Name_Len+File_ExtName_Len];  // 当前解析出的路径
	int nextBlock;
	int startBlock;
	int curBlock;
	int level;
	int* stack;
	int is=0;
	int i=0;
	int tempblock;
	bool bDirCreated=false;

	level=Level(DirPath);
	ParsePath(DirPath,CurPath,0);
	if(*CurPath == '\0')        // 是绝对路径
	{
		curBlock=startBlock=iFATBlock;
		i=1;                    // 查下一层路径
	}
	else                        // 是相对路径
		curBlock=startBlock=CurrentPath.iFirstBlockNum;     //设置查找起始块号
	
	for(;i<level-1;i++)
	{
		ParsePath(DirPath,CurPath,i);       // 开始搜索目录
		tempblock=startBlock;       // 记下目录首块号
		do{
			if (0 == FindCurPath(CurPath,-1,0,startBlock,nextBlock,NULL))
				break;
			if ( EXC_BLOCK_END == FS_FindNextBlock(startBlock,startBlock))
				return ERR_DIR_NOT_FOUND;
		}while(true);
		
		startBlock=nextBlock;         // 继续下一层搜索
	}
	tempblock=startBlock;             // 保存目录项首块号
	ParsePath(DirPath,CurPath,level-1);       // 搜索待删除目录
	if(*CurPath != '\0' || level != 1)        // 不是根目录
	{
		do{
			if (0 == FindCurPath(CurPath,-1,0,startBlock,nextBlock,NULL))
				break;
			if ( EXC_BLOCK_END == FS_FindNextBlock(startBlock,startBlock))
				return ERR_DIR_NOT_FOUND;           // 没有要删除目录	
		}while(true);
		if ( nextBlock == curBlock)
			return ERR_REMOVE_CURRENT_DIR;
	}
	else
		nextBlock=startBlock;
	

	if(FS_IsDirEmpty(nextBlock))
		return ERR_DIR_NOT_EMPTY;

	FS_RemoveDirItem(CurPath,0,tempblock);

	stack=new int [ iFATBlock ];
	do{
		stack[is++]=nextBlock;         // 收集待释放块号
	}while(EXC_BLOCK_END != FS_FindNextBlock(nextBlock,nextBlock));
	
	for(is--;is>=0;is--)
		FS_ReleaseBlock(stack[is],0x0000);		// 释放块
	return 0;
}

/////////////////////////////////////////////////////////
// 函数：	CreatNewFile
// 功能：	创建空文件
// 输入：	文件结构指针(fs) 
//          
//          目录项所在块号
// 返回:    正确 0 ; 错误 查ErrorDef.h
//////////////////////////////////////////////////////////
int FS::CreatNewFile(File_Struct* fs,int mode,char* filename,int DirFirstBlockNum)
{
	SplitFileName(filename,fs->FileName,fs->FileExtName);
	fs->Base_Pos=0;
	fs->Cur_File_Length=0;
	fs->File_Length=0;
	fs->DirItem_Block_Num=DirFirstBlockNum;
	fs->mode=mode;
	fs->First_Block_Num=0xFFFF;         // 空文件不分配块
	// to do :  setup fs's time, fs's attribute....

    DirItem tempDir;	// 新建目录项
	char *temp;
	tempDir.strDFName=new char[iFile_DirNameLen+1];
	temp=tempDir.strDFName;
	for (int i=0;i<=iFile_DirNameLen;i++,temp++)
		*(temp)=0x00;
	memcpy(tempDir.strDFName,fs->FileName,16);
	tempDir.strExtName=new char[iFileExtNameLen+1];
	temp=tempDir.strExtName;
	for (i=0;i<=iFileExtNameLen;i++,temp++)
		*temp=0x00;
	memcpy(tempDir.strExtName,fs->FileExtName,3);
	tempDir.Attri=new Byte[DirItem_AttriLen];
	Byte* tempByte=tempDir.Attri;
	for (i=0;i<DirItem_AttriLen;i++,tempByte++)
        *tempByte=0;
	*(tempDir.Attri)=2;     //设置属性位，表明这个目录项是文件
	
	
	//设置其他属性添加在这里
	
	
	time(&(tempDir.Time_Date));
	tempDir.iFirstBlockNum=0xFFFF;  // 空文件不分配块
	tempDir.iFileLength=0;
	if (FS_NewDirItem(tempDir,DirFirstBlockNum))
		return 1;
	delete[] tempDir.strDFName;
	delete[] tempDir.strExtName;
    return 0;
}

/////////////////////////////////////////////////////////
// 函数：	RemoveFile         
// 功能：	打开文件，指定打开模式
// 输入：	文件路径 ( FilePath )  (相对，绝对都可以)
// 返回:    正确 0 ; 错误 查ErrorDef.h
//////////////////////////////////////////////////////////
int FS::RemoveFile(char* FilePath)
{
	char CurPath[File_Name_Len+File_ExtName_Len];  // 当前解析出的路径
	int nextBlock;
	int startBlock;
	int level;
	int i=0;
	int tempblock;
	File_Struct fs;                 // just a dummy
	int* stack;
	int is=0;
  int Attri=0;

	char* filename=new char[21];    // 拼出文件名
	filename[0]='\0';
	level=Level(FilePath);
	ParsePath(FilePath,CurPath,0);
	if(*CurPath == '\0')        // 是绝对路径
	{
		startBlock=iFATBlock;
		i=1;                    // 查下一层路径
	}
	else                        // 是相对路径
		startBlock=CurrentPath.iFirstBlockNum;     //设置查找起始块号
	
	for(;i<level;i++)
	{
		ParsePath(FilePath,CurPath,i);       // 开始搜索目录
		do{
			if (0 == FindCurPath(CurPath,-1,0,startBlock,nextBlock,NULL))
				break;
			if ( EXC_BLOCK_END == FS_FindNextBlock(startBlock,startBlock))
				return ERR_FILE_NOT_FOUND;
		}while(true);
		
		startBlock=nextBlock;         // 继续下一层搜索
	}
	tempblock=startBlock;             // 保存目录项首块号
	ParsePath(FilePath,CurPath,level);       // 搜索文件
	do{
		if (0 == FindCurPath(CurPath,0,1,startBlock,nextBlock,&fs))
			break;
		if ( EXC_BLOCK_END == FS_FindNextBlock(startBlock,startBlock))
				return ERR_FILE_NOT_FOUND;           // 没有要删除文件	
	}while(true);

  memcpy(&Attri,fs.File_Attri+1,1);
  if( 1 == Attri )   // file read only
    return ERR_ACCESS_DENIED;

	strcpy(filename,fs.FileName);
	if(fs.FileExtName!=NULL){
		strcat(filename,".");
		strcat(filename,fs.FileExtName);
	}
	
	FS_RemoveDirItem(filename,1,startBlock);  // 清除目录项
	if(fs.File_Length==0){
		delete filename;
		return 0;
	}
	stack=new int [ iFATBlock ];
	do{
		stack[is++]=nextBlock;         // 收集待释放块号
	}while(EXC_BLOCK_END != FS_FindNextBlock(nextBlock,nextBlock));
	
	for(is--;is>=0;is--)
		FS_ReleaseBlock(stack[is],0x0000);		// 释放块
	
	delete filename;
	delete[] stack;
	return 0;
}


/////////////////////////////////////////////////////////
// 函数：	OpenFile
// 功能：	打开文件，指定打开模式
// 输入：	文件路径 ( FilePath )  (相对，绝对都可以)
//          打开模式 (mode)
//          如果文件不存在是否创建 (bCreatNew)
//          1:  创建    0:  不创建
//          文件结构指针 (fs)
// 返回:    正确 0 ; 错误 查ErrorDef.h
//////////////////////////////////////////////////////////
int FS::OpenFile(File_Struct* fs,char* FilePath,char mode,int bCreatNew)
{
	char CurPath[File_Name_Len+File_ExtName_Len];  // 当前解析出的路径
	int nextBlock;
	int startBlock;
	int level;
	int i=0;
	int tempblock;
	int Attri=0;
  
	for(i=0;i<=File_Name_Len;i++)	// 文件名清零
		(fs->FileName)[i]='\0';
	for(i=0;i<=File_ExtName_Len;i++)
		(fs->FileExtName)[i]='\0';
	
	level=Level(FilePath);
	ParsePath(FilePath,CurPath,0);
	i=0;
	if(*CurPath == '\0')        // 是绝对路径
	{
		startBlock=iFATBlock;
		i=1;                    // 查下一层路径
	}
	else                        // 是相对路径
		startBlock=CurrentPath.iFirstBlockNum;     //设置查找起始块号
	
	for(;i<level;i++)
	{
		ParsePath(FilePath,CurPath,i);       // 开始搜索目录
		do{
			if (0 == FindCurPath(CurPath,-1,0,startBlock,nextBlock,NULL))
				break;
			if ( EXC_BLOCK_END == FS_FindNextBlock(startBlock,startBlock))
				return ERR_FILE_NOT_FOUND;
		}while(true);

		startBlock=nextBlock;         // 继续下一层搜索
	}
	tempblock=startBlock;             // 保存目录项首块号
	ParsePath(FilePath,CurPath,level);       // 搜索文件
	do{
		if (0 == FindCurPath(CurPath,0,1,startBlock,nextBlock,fs))
			break;
		if ( EXC_BLOCK_END == FS_FindNextBlock(startBlock,startBlock)){
			if(bCreatNew){
				if( CreatNewFile(fs,mode,CurPath,tempblock))  // 如果没有则新建
					return 1;	
			}
			else
				return ERR_FILE_NOT_FOUND;
		}
	}while(true);

  memcpy(&Attri,fs->File_Attri+1,1);   // 检查权限
  if( 'P' != mode){
	  switch( Attri )
	  {
	  case 1:              // file read only
      if( 'w' == mode || 'a' == mode )
      return ERR_ACCESS_DENIED;
      break;
	  case 2:             // file write only
      if( 'r' == mode )
	    return ERR_ACCESS_DENIED;
	  }
  }
	
	fs->Cur_File_Length=fs->File_Length; // 设置当前文件长度
	fs->DirItem_Block_Num=tempblock;  // 设置文件所在目录项首块号
	fs->mode=mode;
	fs->First_Block_Num=nextBlock;     // 设置文件首块号
	if(mode=='a'){                     // 设置文件起始位置
		fs->mode=fs->mode;
		fs->Base_Pos=fs->File_Length;
	}
	if(mode=='w' || mode == 'r') {
		fs->mode=mode;
		fs->Base_Pos=0;
	}
	return 0;
}

//////////////////////////////////////////////////////////
// 函数：	ReadFile
// 功能：	读取文件内容到data
// 输入：	文件路径 ( FilePath )  data 数据指针 
//          数据块长度( offset )
// 输出：   len:  实际读出字节数
// 返回:    正确 0 ; 错误 查ErrorDef.h
// 备注:    文件实际读出字节数还有一些错误  <# pentium #>
//////////////////////////////////////////////////////////

int FS::ReadFile(File_Struct* fs, Byte* data, int offset,int& len)
{
	int nextBlock;
	bool bEnd=false;
	int i=0;
	int temp;
	int itemp;
	int pos=fs->Base_Pos;
	len=0;
	if ( offset < 0 )             // 偏移量非负
		return ERR_PARA_INVALID;

	if( 0 == fs->Cur_File_Length){
		len=0;
		return ERR_NULL_FILE;     // 企图读取空文件
	}

	nextBlock=fs->First_Block_Num;
	Byte* ptempdata=new Byte[iBlockSize];
	Byte* pptemp=ptempdata;

	itemp=pos / iBlockSize;		// 搜索到指定位置
	for(i=0;i<itemp;i++)
		FS_FindNextBlock(nextBlock,nextBlock);
	
	temp=(iBlockSize - pos % iBlockSize) >= offset ? offset : iBlockSize - (pos%iBlockSize);
	pFS_Kernel->Read_Block(ptempdata,nextBlock,iBlockSize);	//读出
	ptempdata+=(pos % iBlockSize);
	memcpy(data,ptempdata,temp);
	data+=temp;
	offset-=temp;		// 减少偏移量
	len+=temp;
	if(offset<=0){
		delete[] pptemp;
		fs->Base_Pos=len;
		if(EXC_BLOCK_END == FS_FindNextBlock(nextBlock,nextBlock)){
			len=fs->File_Length % iBlockSize;
			return EXC_END_OF_FILE;
		}
		return 0;
	}
	if(EXC_BLOCK_END == FS_FindNextBlock(nextBlock,nextBlock))
		return EXC_END_OF_FILE;
	while(!bEnd && offset > 0)
	{
		pFS_Kernel->Read_Block(ptempdata,nextBlock,iBlockSize);
		temp= ( iBlockSize > offset) ? offset : iBlockSize;
		memcpy(data,ptempdata,temp);
		len+=temp;
		offset-=iBlockSize;
		if( EXC_BLOCK_END == FS_FindNextBlock(nextBlock,nextBlock))  // 读完最后一块
			bEnd=true;
		else
			data+=temp;
	}
	fs->Base_Pos=pos+len;
	delete[] pptemp;
	if(bEnd)
		return EXC_END_OF_FILE;
  return 0;
/*
	FileFirstBlock=PreProcessPath(FilePath,8,FileFirstBlock,FileLength);
	else						 // 是相对路径
		FileFirstBlock=PreProcessPath(FilePath,CurrentPath.iFirstBlockNum,FileFirstBlock,FileLength);
	if ( ERR_FILE_NOT_FOUND == FileFirstBlock)
	{
		return ERR_FILE_NOT_FOUND;
	}
	else
	{
		data=new Byte[FileLength];         // 将数据读到data
		while(!bEnd)
		{
			pFS_Kernel->Read_Block(data,FileFirstBlock,iBlockSize);
			data+=iBlockSize;
			FS_FindNextBlock(FileFirstBlock,FileFirstBlock);
		}
	}
*/
}

//////////////////////////////////////////////////////////
// 函数功能：   写文件
// 输入：	文件路径 ( FilePath )  data 数据指针 
//          数据块大小(offset) 
// 输出：   len:  实际写入字节数
// 返回:    正确 0 ; 错误 查ErrorDef.h
/////////////////////////////////////////////////////////
int FS::WriteFile(File_Struct* fs,Byte* data,int offset,int& len)
{	
	int nextBlock;
	bool bEnd=false;
	int i=0;
	int itemp;
	int temp;
	int pos=fs->Base_Pos;
	Byte* ptempdata=new Byte[iBlockSize];
	Byte* pptempdata=ptempdata;
	nextBlock=fs->First_Block_Num;
	len=0;
	if(nextBlock==0xFFFF)  // 是空文件
		if(ERR_DISK_FULL == FS_ReqNewBlock(nextBlock,nextBlock,&(fs->First_Block_Num))){
			delete[] ptempdata;
			return ERR_DISK_FULL;
		}
	
	if ( offset < 0 ){             // 偏移量非负
		delete[] ptempdata;
		return ERR_PARA_INVALID;
	}	
	nextBlock=fs->First_Block_Num;
	itemp=pos / iBlockSize;		// 搜索到指定位置
			
	for(i=0;i<itemp;i++)
		if(EXC_BLOCK_END == FS_FindNextBlock(nextBlock,nextBlock))
			if(ERR_DISK_FULL == FS_ReqNewBlock(nextBlock,nextBlock)){
				delete[] ptempdata;
				len+=iBlockSize;
				return ERR_DISK_FULL;
			}
	temp=(iBlockSize - pos % iBlockSize) >= offset ? offset : iBlockSize - (pos%iBlockSize);
	pFS_Kernel->Read_Block(ptempdata,nextBlock,iBlockSize);	//读出
	pptempdata+=(pos % iBlockSize);
	memcpy(pptempdata,data,temp);
	pFS_Kernel->Write_Block(ptempdata,nextBlock,iBlockSize); //写入
	data+=temp;
	offset-=temp;		// 减少偏移量
	len+=temp;
	if(offset<=0) {
		delete[] ptempdata;
		fs->Cur_File_Length=pos+len;               // 更新文件长度
		fs->Base_Pos=fs->Cur_File_Length;
		if( fs->Cur_File_Length > fs->File_Length)
			fs->File_Length=fs->Cur_File_Length;
		return 0;
	}
	if(EXC_BLOCK_END == FS_FindNextBlock(nextBlock,nextBlock))
		if( ERR_DISK_FULL == FS_ReqNewBlock(nextBlock,nextBlock)){ // 申请新块				
			delete[] ptempdata;
			fs->Cur_File_Length=pos+len;               // 更新文件长度

			if( fs->Cur_File_Length > fs->File_Length)
				fs->File_Length=fs->Cur_File_Length;

			fs->Base_Pos=fs->Cur_File_Length;
			return ERR_DISK_FULL;
		}
	pptempdata=ptempdata;
	while(offset > 0){
		pFS_Kernel->Read_Block(ptempdata,nextBlock,iBlockSize);
		temp= ( iBlockSize > offset) ? offset : iBlockSize;
		memcpy(pptempdata,data,temp);
		pFS_Kernel->Write_Block(ptempdata,nextBlock,iBlockSize);
		len+=temp;
		offset-=iBlockSize;
		if( EXC_BLOCK_END == FS_FindNextBlock(nextBlock,nextBlock))  // 读完最后一块
			if(offset>0)  // 还没有写完
				if(ERR_DISK_FULL == FS_ReqNewBlock(nextBlock,nextBlock)){ //申请新块
					delete[] ptempdata;
					fs->Cur_File_Length=pos+len;               // 更新文件长度
					fs->Base_Pos=fs->Cur_File_Length;
					if( fs->Cur_File_Length > fs->File_Length)
						fs->File_Length=fs->Cur_File_Length;
					return ERR_DISK_FULL;   // 磁盘已满
				}
		data+=iBlockSize;
	}
	
	fs->Cur_File_Length=pos+len;               // 更新文件长度
	if( fs->Cur_File_Length > fs->File_Length)
		fs->File_Length=fs->Cur_File_Length;
	fs->Base_Pos=fs->Cur_File_Length;
	delete[] ptempdata;    
			
	// to do :  update time ....
	return 0;

}

//////////////////////////////////////////////////////////
// 函数：	CloseFile
// 功能：	更新并关闭文件
// 输入：	文件结构指针: fs 
// 备注:    调用者必须确保fs指向的是正确的已经打开的文件的结构指针!
// 返回:    正确 0 ; 错误 查ErrorDef.h
//////////////////////////////////////////////////////////
int FS::CloseFile(File_Struct* fs)
{
	int itemp;
	int nextBlock;
	int foo;    //foo 

	char* filename=new char[21];    // 拼出文件名
	filename[0]='\0';
	strcpy(filename,fs->FileName);
	if(fs->FileExtName!=NULL){
		strcat(filename,".");
		strcat(filename,fs->FileExtName);
	}

	nextBlock=fs->DirItem_Block_Num;
//	FindCurPath(filename,1,1,fs->DirItem_Block_Num,foo,fs);  
	// 更新属性
//*	
	do{
		if (0 == FindCurPath(filename,1,1,nextBlock,foo,fs))
			break;
		if ( EXC_BLOCK_END == FS_FindNextBlock(nextBlock,nextBlock))
			return 1;	
	}while(true);
//*/
	int is=0;  //栈指针
	int* stack=new int[iMaxBlockNum];   // 使用栈释放块
	if(fs->Cur_File_Length>=fs->File_Length){	// 不需要释放块
		delete[] filename;
		return 0;
	}
	else
		itemp=fs->Cur_File_Length / iBlockSize;		// 搜索到指定位置

	nextBlock=fs->First_Block_Num;
	for(int i=0;i<itemp;i++)
		FS_FindNextBlock(nextBlock,nextBlock);

	do{
		stack[is++]=nextBlock;         // 收集待释放块号
	}while(EXC_BLOCK_END != FS_FindNextBlock(nextBlock,nextBlock));
		
	for(is--;is>0;is--)
		FS_ReleaseBlock(stack[is],0x0000);		// 释放块
	FS_ReleaseBlock(stack[0],0xFFFF);

	delete[] stack;
	delete[] filename;
	return 0;
}

//////////////////////////////////////////////////////////
// 函数功能:  拷贝文件
// 输入：	  源文件路径:    source
//            目标文件路径:  Dest
// 返回:      正确 0 ; 错误 查ErrorDef.h
//////////////////////////////////////////////////////////
int FS::CopyFiles(char* Source,char* Dest)
{
	File_Struct sfs;             // just a dummy <# pentium #>
	File_Struct dfs;
	int len;
	int length=0;
	int res=OpenFile(&sfs,Source,'r',0);
	
  if(ERR_FILE_NOT_FOUND == res)
		return ERR_FILE_NOT_FOUND;
  if(ERR_ACCESS_DENIED == res )
    return ERR_ACCESS_DENIED;
  
  if( 0 == OpenFile(&dfs,Dest,'w',0))
    return EXC_FILE_EXIST;
  
  res=OpenFile(&dfs,Dest,'w');
  if( ERR_ACCESS_DENIED == res )
    return ERR_ACCESS_DENIED;
	
  Byte* buffer=new Byte [sfs.File_Length];
  ReadFile(&sfs,buffer,sfs.File_Length,len);
  res=WriteFile(&dfs,buffer,sfs.File_Length,len);
	
  if(res==ERR_DISK_FULL)
	{
		delete[] buffer;
		CloseFile(&dfs);
		return ERR_DISK_FULL;
	}
	delete[] buffer;
	CloseFile(&dfs);
	return 0;
/*	
	while(true)
	{
		res = ReadFile(&sfs,buffer,iBlockSize,len);
		if( ERR_NULL_FILE == res)
		{
			CloseFile(&sfs);
			CloseFile(&dfs);
			return 0;
		}
		else if( EXC_END_OF_FILE == res)
		{
			if( ERR_DISK_FULL == WriteFile(&dfs,buffer,len,len)){
				dfs.Cur_File_Length=length;
				CloseFile(&dfs);
				return ERR_DISK_FULL;
			}
			length+=len;
			CloseFile(&dfs);
			return 0;			
		}
		else
		{
			if( ERR_DISK_FULL == WriteFile(&dfs,buffer,len,len)){
				dfs.Cur_File_Length=length;
				CloseFile(&dfs);
				return ERR_DISK_FULL;
			}
			length+=len;
		}
	}
*/	
}


///////////////////////////////////////////////////////////
//
///////////////////////////////////////////////////////////
int FS::FS_IsDirValid(char * Source,DirItem* dir,int& tempblock)
{
	int i;
	int level;
	int startBlock,nextBlock;
	char CurPath[File_Name_Len+File_ExtName_Len];  // 当前解析出的路径
	i=0;
	level=Level(Source);
	ParsePath(Source,CurPath,0);
	if(*CurPath == '\0')        // 是绝对路径
	{
		startBlock=iFATBlock;
		i=1;                    // 查下一层路径
	}
	else                        // 是相对路径
		startBlock=CurrentPath.iFirstBlockNum;     //设置查找起始块号
	
	for(;i<level;i++)
	{
		ParsePath(Source,CurPath,i);       // 开始搜索目录
		tempblock=startBlock;       // 记下目录首块号
		do{
			if (0 == FindCurPath(CurPath,-1,0,startBlock,nextBlock,NULL))
				break;
			if ( EXC_BLOCK_END == FS_FindNextBlock(startBlock,startBlock))
				return ERR_DIR_NOT_FOUND;
		}while(true);
		
		startBlock=nextBlock;         // 继续下一层搜索
	}
	tempblock=startBlock;             // 保存目录项首块号
	ParsePath(Source,CurPath,level);       // 搜索目录

	do{
		if (0 == FindCurPath(CurPath,2,0,startBlock,nextBlock,NULL,dir))
			break;
		if ( EXC_BLOCK_END == FS_FindNextBlock(startBlock,startBlock)){
			return ERR_DIR_NOT_FOUND;           // 没有找到目录	
		}
	}while(true);
	return 0;
}

//////////////////////////////////////////////////////////
// 函数功能:  同Windows move 命令
// 输入：	  源路径: Source ( 不以'/'结尾 )
//            目标路径:  Dest ( 不以'/'结尾 )
// 返回:      正确 0 ; 错误 查ErrorDef.h
//////////////////////////////////////////////////////////
int FS::MoveFiles(char* Source,char* Dest)
{
	int i;
	File_Struct sfs,dfs;
	int nRet1,nRet2,nRet;
	DirItem sdir;
	DirItem ddir;
	int level;
	int sstartBlock=0,dstartBlock=0;
	int startBlock,nextBlock;
	char temp[256];
  int res;

	nRet1=OpenFile(&sfs,Source,'r',0);
	if( 0 == nRet1 ){   //  source file found
		nRet2=OpenFile(&dfs,Dest,'w',0);
		if( 0 == nRet2 ){  //  dest file found
				CloseFile(&dfs);				   // just for clean 
				CloseFile(&sfs);
				return EXC_FILE_EXIST;             // file has exist
		}
		else{   // dest file not found
			CloseFile(&sfs);                       // just for clean
			res=OpenFile(&sfs,Source,'w');
			if( ERR_ACCESS_DENIED == res )
				return res;
			else
				CloseFile(&sfs);
			res=CopyFiles(Source,Dest);                 // first, copy to dest
			if( 0 != res)
				return res;
			res=RemoveFile(Source);                    // then,  delete source
			if( 0 != res)
				return res;
			return 0;        
		}// ok, move complete :)
	}
	else{					//  source file not found
		// maybe source is a dir
		sdir.Attri=new Byte[3];
		sdir.strDFName=new char[17];
		nRet=FS_IsDirValid(Source,&sdir,sstartBlock);
		if( ERR_DIR_NOT_FOUND == nRet ){   // source dir not valid
			delete[] sdir.Attri;
			delete[] sdir.strDFName;
			return ERR_DIR_NOT_FOUND;
		}
		else if( 0 == nRet ){    
			// source dir found 
			ddir.Attri=new Byte[3];
			ddir.strDFName=new char[17];
			nRet1 = FS_IsDirValid(Dest,&ddir,dstartBlock);
			if( ERR_DIR_NOT_FOUND == nRet1 ){  // dest dir not valid
				if( dstartBlock == sstartBlock ){  // rename dir
					level=Level(Dest);
					for(i=0;i<=File_Name_Len;i++)	// 目录名清零
						(ddir.strDFName)[i]='\0';
					ParsePath(Dest,ddir.strDFName,level);
					memcpy(ddir.Attri,sdir.Attri,3);
					ddir.iFirstBlockNum=sdir.iFirstBlockNum;
					startBlock=sstartBlock;
					do{
						if (0 == FindCurPath(sdir.strDFName,3,0,startBlock,nextBlock,NULL,&ddir))
							break;
						if ( EXC_BLOCK_END == FS_FindNextBlock(startBlock,startBlock))
							return ERR_DIR_NOT_FOUND;           // 没有找到目录	
					}while(true);
					delete[] sdir.Attri;
					delete[] sdir.strDFName;
					delete[] ddir.Attri;
					delete[] ddir.strDFName;
					return 0;
				}
			}
			else if( 0 == nRet1 ){  // dest dir  found
				if( sstartBlock == dstartBlock && 0 == strcmp(sdir.strDFName,ddir.strDFName)){  // 目录相同
					delete[] sdir.Attri;
					delete[] sdir.strDFName;
					delete[] ddir.Attri;
					delete[] ddir.strDFName;
					return ERR_DIR_DUPLICATE;
				}
				else{       // move source dir to dest dir
					strcpy(temp,Dest);
					strcat(temp,"/");
					strcat(temp,sdir.strDFName);
					strcat(temp,"/");
					i=strlen(temp);
					MakeDir(temp,0);         // make an new dir under dest dir
					temp[i-1]='\0';
					FS_IsDirValid(temp,&ddir,dstartBlock);  // find new dir's .. 's block
					ddir.iFirstBlockNum=sdir.iFirstBlockNum;  // update FirstBlock
					startBlock=dstartBlock;
					do{
						if (0 == FindCurPath(sdir.strDFName,3,0,startBlock,nextBlock,NULL,&ddir))
							break;
						if ( EXC_BLOCK_END == FS_FindNextBlock(startBlock,startBlock))
							return ERR_DIR_NOT_FOUND;           // 没有找到目录	
					}while(true);
					FS_RemoveDirItem(sdir.strDFName,0,sstartBlock);  // clean older diritem
					delete[] sdir.Attri;
					delete[] sdir.strDFName;
					delete[] ddir.Attri;
					delete[] ddir.strDFName;
					return 0;
				}
			}
		}
	}
	return ERR_DIR_NOT_FOUND;
}
	
//////////////////////////////////////////////////////////
// 函数功能:  改变文件属性
// 输入：	  文件路径: FilePath
//            属性: mode 
// 返回:      正确 0 ; 错误 查ErrorDef.h
//////////////////////////////////////////////////////////
int FS::ChangeMode(char* FilePath,int mode)
{
	File_Struct fs;         // just a dummy
	if( ERR_FILE_NOT_FOUND == OpenFile(&fs,FilePath,'P',0))  // open as privilege
		return ERR_FILE_NOT_FOUND;
	switch(mode)
	{
	case FILE_READ_ONLY:
		 (fs.File_Attri)[1]=0x01;
		 break;
	case FILE_WRITE_ONLY:
		 (fs.File_Attri)[1]=0x02;
		 break;
	case FILE_READ_WRITE:
		 (fs.File_Attri)[1]=0x03;
		 break;
	default:
   	 	CloseFile(&fs);
    	return ERR_ATTRI_NOT_SUPPORT;
	}
	CloseFile(&fs);
	return 0;
}

//////////////////////////////////////////////////////////
// 函数：	Seek
// 功能：	移动文件基准点
// 输入：	文件结构指针: fs
//          新位置 pos 
// 返回:    正确 0 ; 错误 查ErrorDef.h
//////////////////////////////////////////////////////////
int FS::Seek(File_Struct* fs,int pos)
{
	if( pos < 0 )
		return ERR_POS_INVALID;
	if( fs->mode == 'a' && pos < fs->Base_Pos)
		return ERR_POS_INVALID;
	if( fs->mode == 'r' && pos > fs->File_Length)
		return ERR_POS_INVALID;
	
	fs->Base_Pos=pos;
	return 0;
}

//////////////////////////////////////////////////////////
// 函数功能：	计算路径中的'/'数量
//				供OpenFile等命令使用
// 输入：		Path: 路径字符串
// 输出：		'/'个数
//////////////////////////////////////////////////////////

int FS::Level(char* Path)
{
	int count=0;
	char* temp=Path;
	for(;*temp != '\0';temp++)
		if(*temp == '/')
			count++;
	return count;	
}

//////////////////////////////////////////////////////////
// 函数功能：   拆分文件名
// 输入：       Source : 待拆分文件名
//              Main:    文件主名
//              Ext:     文件扩展名
// 输出：       无 （永远正确） 文件名有效性验证由调用者负责
///////////////////////////////////////////////////////////
void FS::SplitFileName(char* Source,char* Main,char* Ext)
{
	char* temp=Source;
	int i=0,j=0;
	for(;*temp!='\0'  && *temp != '.';temp++)
		*(Main++)=*temp;
	*Main='\0';
	if(*temp == '\0'){
		*Ext='\0';
	}
	else
	{
		temp++;
		for(;*temp != '\0';temp++)
			*(Ext++)=*temp;
		*Ext='\0';
	}
}
//////////////////////////////////////////////////////////
// 函数功能：	解析路径
// 输入：		level指解析到第几个'/',
//				SourcePath 要解析的路径
// 输出：		DestPath表示第level个'/'后面的子目录/文件名
// 返回值：		0 - 正常，-1 - 文件结束
// 备注：			当SourcePath为"/"时，DestPath可能为空串
////////////////////////////////////////////////////////// 

int FS::ParsePath(char* SourcePath,char* DestPath,int level)
{
	int j;
	char* temp=SourcePath;
	char* ttemp=DestPath;
    for(j=0;*temp != '\0' && j<level;temp++)
	{
		if( '/'==*temp)
		{
			j++;
		}
	}
	if(*temp == '\0')       //路径查寻结束
		return -1;
	for(;*temp != '\0' && *temp != '/';ttemp++,temp++)
		*(ttemp)=*(temp);
	*ttemp='\0';
	return 0;
}

//////////////////////////////////////////////////////////
// 函数:	FindCurPath
// 创建者:  pentium
// 功能：	查找当前路径下文件（目录） (只查一块)
// 输入：	FileName: 待查找文件（目录）名，由ParsePath提供
//          op;      指定对文件结构指针的操作
//                   －1： 什么也不作   0:  取到fs  1: 存到虚拟磁盘    
//                   2:    取到DirItem    3:  存到虚拟磁盘
//			isFile:  标识待查找是文件还是目录 1：文件，0：目录
//			iBlock:  从第几个块查起
//			fs:     如果是文件，则返回文件结构,不是文件则忽略(NULL)
// 输出：	iNextBlock: 待查文件（目录）的首块号
// 返回:	正确 0, 错误： ERR_FILE_NOT_FOUND
// 备注：	此函数可以替代PreProcessPath
//////////////////////////////////////////////////////////

int FS::FindCurPath(char* FileName,int op,int isFile,int iBlock,int& iNextBlock,File_Struct* fs,DirItem* dir)
{
	int i;
	char Main[16];       // 文件主名或目录名
	char Ext[3];         // 文件扩展名
	
	if(isFile)
		SplitFileName(FileName,Main,Ext); // 拆分文件名
	else
		strcpy(Main,FileName);

	bool bFound=false;             //尚未找到
	Byte* tempdata=new Byte[iBlockSize];
	Byte* temp;                     // copy of tempdata
	Byte* tttemp;                   // for delete
	Byte* ttempdata=tempdata;		// for delete
	Byte* ttemp=tempdata;			// for search
	pFS_Kernel->Read_Block(tempdata,iBlock,iBlockSize);
	temp=new Byte [iBlockSize];
	memcpy(temp,tempdata,iBlockSize);
	tttemp=temp;
	for(i=0;i<iBlockSize/iDirItemSize && !bFound;i++)
	{
			
		ttemp=tempdata+File_ExtName_Len+File_Name_Len;
		if ( 1 == ((int)*ttemp % 2) && !isFile)  // 是目录
		{
			*(tempdata+16)='\0';
			if ( 0 == strcmp((char*)tempdata,Main) )  // 符合
			{
				bFound=true;
				if( op == 2){  // 将目录项数据取到DirItem
					memcpy(dir->strDFName,temp,16);	  // 输出目录名
					(dir->strDFName)[16]='\0';              
//					memcpy(dir->,temp+16,3);	// 输出文件扩展名
//					(fs->FileExtName)[3]='\0';
					memcpy(dir->Attri,temp+19,3);  // 输出目录属性域
//					memcpy(&(fs->Time_Last_Write),temp+22,4);  // 输出文件修改日期
					dir->iFirstBlockNum=0;
					memcpy(&(dir->iFirstBlockNum),temp+26,2);  // 输出目录首块号
//					memcpy(&(fs->File_Length),temp+28,4);       //输出文件长度
				}
				else if ( op == 3 ){ // 将目录项数据存到磁盘
					memcpy(temp,dir->strDFName,16);	  // 输出目录名
//					(dir->strDFName)[16]='\0';              
//					memcpy(dir->,temp+16,3);	// 输出文件扩展名
//					(fs->FileExtName)[3]='\0';
					memcpy(temp+19,dir->Attri,3);  // 输出目录属性域
//					memcpy(&(fs->Time_Last_Write),temp+22,4);  // 输出文件修改日期
//					dir->iFirstBlockNum=0;
					memcpy(temp+26,&(dir->iFirstBlockNum),2);  // 输出目录首块号
//					memcpy(&(fs->File_Length),temp+28,4);       //输出文件长度
					pFS_Kernel->Write_Block(tttemp,iBlock,iBlockSize);  // 存回
				}
			}
		}
		else if ( 2 == ((int)*ttemp % 4 ) && isFile)  //是文件
		{
			*(tempdata+19)='\0';
			if (0 == strcmp((char*)(tempdata+16),Ext)){  // 扩展名符合
				*(tempdata+16)='\0';
				if ( 0 == strcmp((char*)tempdata,Main)){  // 文件名符合				
					bFound=true;
					if(op==0){    //   将目录项数据取到文件结构
						memcpy(fs->FileName,temp,16);	  // 输出文件主名
						(fs->FileName)[16]='\0';              
						memcpy(fs->FileExtName,temp+16,3);	// 输出文件扩展名
						(fs->FileExtName)[3]='\0';
						memcpy(&(fs->File_Attri),temp+19,3);  // 输出文件属性域
						memcpy(&(fs->Time_Last_Write),temp+22,4);  // 输出文件修改日期
						fs->First_Block_Num=0;
						memcpy(&(fs->First_Block_Num),temp+26,2);  // 输出文件首块号
						memcpy(&(fs->File_Length),temp+28,4);       //输出文件长度

					}
					else if(op==1){         //   将文件结构中的数据存到虚拟磁盘
						
						memcpy(temp,fs->FileName,16);	  // 输出文件主名
						memcpy(temp+16,fs->FileExtName,3);	// 输出文件扩展名
						memcpy(temp+19,fs->File_Attri,3);  // 输出文件属性域
						memcpy(temp+22,&(fs->Time_Last_Write),4);  // 输出文件修改日期
						memcpy(temp+26,&(fs->First_Block_Num),2);  // 输出文件首块号
						memcpy(temp+28,&(fs->Cur_File_Length),4);       //输出文件长度
						pFS_Kernel->Write_Block(tttemp,iBlock,iBlockSize);  // 存回
					}
				}
			}
		}
		if(bFound)   // 如果找到
		{
			tempdata+=26;
			iNextBlock=0;
			memcpy(&iNextBlock,tempdata,2);      //输出待查文件(目录)的首块号
			delete[] ttempdata;
			delete[] tttemp;
			return 0;
		}
		else
			tempdata+=iDirItemSize;
			temp+=iDirItemSize;
	}

	delete[] ttempdata;
	delete[] tttemp;
	return ERR_FILE_NOT_FOUND;
}

//////////////////////////////////////////////////////////
// 函数名：	PreProcessPath
// 功能：	搜索FAT表，找到正确路径对应文件（目录）
//			的首块号。
// 输入：	FilePath: 待查找路径
//			iFirstBlock：从第几个块查起（相对路径和绝对路径不同）
// 输出：	FileFirstBlock: 存放查找到的文件（目录）首块号
//			FileLen:  存放查找到的文件的长度						
// 返回值：	正确 0， 错误： ERR_FILE_NOT_FOUND
//                  其他错误待定
// 备注：
//////////////////////////////////////////////////////////

int FS::PreProcessPath(char* FilePath, int iFirstBlock, int& FileFirstBlock,int& FileLen)
{
	char CurPath[File_Name_Len+File_ExtName_Len]; 
	int level=0;
	int i;
	bool bFound=false;
	bool bFile=false;
	bool bEnd=false;
	Byte* tempdata=new Byte[iBlockSize];
	Byte* ttemp=tempdata;
	do
	{
		pFS_Kernel->Read_Block(tempdata,iFirstBlock,iBlockSize);
		if ( EXC_BLOCK_END == FS_FindNextBlock(iFirstBlock,iFirstBlock))
		{
			bEnd=true;
		}
		if(	-1 == ParsePath(FilePath,CurPath,level))
		{
			bFile=true;
		}
		for(i=0;i<iBlockSize/iDirItemSize && !bFound;i++)
		{

			ttemp=tempdata+File_ExtName_Len+File_Name_Len;
			if ( 1 == ((int)*ttemp % 2) && !bFile)  // 是目录
			{
				*(tempdata+16)='\0';
				if ( strcmp((char*)tempdata,CurPath) )  // 符合
				{
					bFound=true;
					level++;
					
				}
			}
			else if ( 2 == ((int)*ttemp % 4 ) && bFile)  //是文件
			{
				*(tempdata+19)='\0';
				if ( strcmp((char*)tempdata,CurPath) )  // 符合
				{
					bFound=true;
					ttemp+=7;
					FileFirstBlock=0;
					memcpy(&FileFirstBlock,ttemp,2);
					FileLen=0;
					memcpy(&FileLen,ttemp+2,4);
					return 0;             // 返回
				}
			}
			tempdata+=iDirItemSize;
		}
	}
	while( !bEnd);     // 继续搜索其他块
	return ERR_FILE_NOT_FOUND;
}

//////////////////////////////////////////////////////////
// 函数名：		FS_ReadFATTable
// 功能：		将FAT表读入到内存中FS对象的FAT成员变量里
// 输入：
// 返回值：		0 - 正常，1 - 失败
// 备注：		
//////////////////////////////////////////////////////////

int FS::FS_ReadFATTable()
{
	Byte *pDataTemp=new Byte[iBlockSize];
	Byte *pTemp=FAT;
    for (int i=0;i<iFATBlock;i++)
	{
		pFS_Kernel->Read_Block(pDataTemp,i,iBlockSize);
		memcpy(pTemp,pDataTemp,iBlockSize);
		pTemp+=iBlockSize;
	}
	delete pDataTemp;
	return 0;
}

//////////////////////////////////////////////////////////
// 函数名：		FS_WriteFATTable
// 功能：		将在内存中的FAT成员变量里的FAT表写回到磁盘上
// 输入：
// 返回值：		0 - 正常，1 - 失败
// 备注：
//////////////////////////////////////////////////////////

int FS::FS_WriteFATTable()
{
	Byte *pDataTemp=new Byte[iBlockSize];
	Byte *pTemp=FAT;
    for (int i=0;i<iFATBlock;i++)
	{
		memcpy(pDataTemp,pTemp,iBlockSize);
		if (pFS_Kernel->Write_Block(pDataTemp,i,iBlockSize))
		{
			delete pDataTemp;
			return 1;
		}
		pTemp+=iBlockSize;
	}
	delete pDataTemp;
	return 0;
}

//////////////////////////////////////////////////////////
// 功能：	写相应块号的FAT内容
// 输入：	块号BlockNum,要写入FAT内容InputData
// 输出：
// 返回值： 0--成功，1--读失败
// 备注：
//////////////////////////////////////////////////////////

int FS::FS_WriteFAT(int BlockNum,int InputData)
{
	static int to=0;	
	Byte *Temp=FAT;
	Temp+=BlockNum*iBlockNumSize;
	memcpy(Temp,&InputData,iBlockNumSize);
	to++;

	if( to >= autoSaveTimeout){  // auto save processing
		FS_WriteFATTable();
		to=0;
	}

	return 0;
}

//////////////////////////////////////////////////////////
// 功能：	读相应块号的FAT内容
// 输入：	块号BlockNum,
// 输出：	FAT内容OutputData
// 返回值： 0--成功，1--读失败
// 备注：
//////////////////////////////////////////////////////////

int FS::FS_ReadFAT(int BlockNum,int& OutputData)
{
	Byte *Temp=FAT;
	OutputData=0;
	Temp+=BlockNum*iBlockNumSize;
 	memcpy(&OutputData,Temp,iBlockNumSize);
	return 0;
}

//////////////////////////////////////////////////////////
// 函数名：	FS_ReleaseBlock
// 功能：	释放空块
// 创建者： pentium
// 输出：	BlockNum - 空块号
// 返回值：	0 - 正常，1 - 失败，磁盘已满，找不到空块, 
//			2 - 失败，读FAT表时出错
// 备注：	暂时以最先分配最小块号作为空块的分配策略以后
//			可以考虑，设置一个FAT表的指针，从指针所在的
//			位置开始向下搜索.
// 修改：   pentium  申请新块时同时将块清零 (方便WriteFile调用)
//////////////////////////////////////////////////////////
int FS::FS_ReleaseBlock(int BlockNum,int data)
{
	if(FS_WriteFAT(BlockNum,data) )   // 释放块
	{
		return 1;
	}
	return 0;
}


//////////////////////////////////////////////////////////
// 函数名：	FS_ReqNewBlock
// 功能：	申请空块，并返回空块的块号
// 输出：	BlockNum - 空块号
// 返回值：	0 - 正常，1 - 失败，磁盘已满，找不到空块, 
//			2 - 失败，读FAT表时出错
// 备注：	暂时以最先分配最小块号作为空块的分配策略以后
//			可以考虑，设置一个FAT表的指针，从指针所在的
//			位置开始向下搜索.
// 修改：   pentium  申请新块时同时将块清零 (方便WriteFile调用)
//          对第一次申请块另外处理
//          所以增加一个参数fsFirstBlock（缺省为0）
//////////////////////////////////////////////////////////

int FS::FS_ReqNewBlock(int& BlockNum,int PreBlockNum,int* fsFirstBlock)
{
//	Byte *Temp=FAT;	// not used
	int iTemp=0;
	int k;
	Byte* data;
	for (int i=iFATBlock;i<iMaxBlockNum;i++)
	{
		if (FS_ReadFAT(i,iTemp))
			return 2;
		if (iTemp==0)
		{
			BlockNum=i;
			data=new Byte [iBlockSize];  // 将新块清零  <# pentium #>
			for(k=0;k<iBlockSize;k++)
				data[k]=0x00;
			pFS_Kernel->Write_Block(data,BlockNum,iBlockSize);
			delete[] data;
			// 写FAT表     PreBlockNum可能是0xFFFF 分别处理 <# pentium #>
			if(PreBlockNum != 0xFFFF){
				FS_WriteFAT(PreBlockNum,BlockNum);
				FS_WriteFAT(BlockNum,0xFFFF);
			}
			else{
				if(fsFirstBlock != NULL)
					*fsFirstBlock=BlockNum;
				FS_WriteFAT(BlockNum,0xFFFF);
			}
			return 0;
		}
		
	}
	return ERR_DISK_FULL;	// 磁盘用尽  <# pentium #>
}

//////////////////////////////////////////////////////////
// 函数名：		FS_FindNextBlock   
// 功能：		找出当前块的下一块的首地址
// 输入：		CurrentBlock - 当前块块号，
// 输出：		NextBlock - 下一块块号
// 返回值：		0 - 正常，
//				EXC_BLOCK_END - 此块是文件/目录的最后一块
//				2 - FAT数据出错
// 备注：
//////////////////////////////////////////////////////////

int FS::FS_FindNextBlock(int CurrentBlock,int& NextBlock)
{
	Byte *pTemp=FAT+CurrentBlock*iBlockNumSize;
	int temp=0;
	NextBlock=CurrentBlock;		// 便于处理 <# pentium #>
	memcpy(&temp,pTemp,iBlockNumSize);
    if (temp==0xFFFF)
		return EXC_BLOCK_END;
	else
		if (NextBlock>=iMaxBlockNum)
			return 2;
	NextBlock=temp;
	return 0;
}

//////////////////////////////////////////////////////////
// 函数名：		FS_FindEmptyDirItem   
// 功能：		在给定目录中查找第一个空的目录项
// 输入：		DirFirstBlock - 目录的首块号
// 输出：		BlockNum - 空目录项所在块号，
//				ByteOffset - 空目录项在块中偏移
// 返回值：		0 - 正常，1 - 没找到，并申请新块, 2 - 失败
// 备注：
//////////////////////////////////////////////////////////

int FS::FS_FindEmptyDirItem(int DirFirstBlock,int& BlockNum,int& ByteOffset)
{
	Byte* pCurrentBlockData=new Byte[iBlockSize];
	int iCurrentBlock=DirFirstBlock;
	int i,iNewBlock,iTemp,ttemp,iOffset=0;
	do{
		//搜索当前块的内容
		if (pFS_Kernel->Read_Block(pCurrentBlockData,iCurrentBlock,iBlockSize))
		{
			delete pCurrentBlockData;
			return 2;
		}
		for (i=0;i<(int)iBlockSize/iDirItemSize;i++)
		{
			if ((((int)pCurrentBlockData[iOffset+(iFile_DirNameLen+iFileExtNameLen)]) % 4) == 0)      
			{	//目录和文件的属性标志位均为0
			    //找到了空块
				//std::cout<<iOffset<<endl;
				BlockNum=iCurrentBlock;
				ByteOffset=iOffset;
				delete pCurrentBlockData;
				return 0;
			}
			iOffset+=iDirItemSize;
		}
		ttemp=FS_FindNextBlock(iCurrentBlock,iNewBlock); //搜索余下的块
		//std::cout<<ttemp<<endl;
		iOffset=0;
        switch (ttemp)
		{
		case 2:
			//std::cout<<"some thing wrong le "<<endl;
			delete pCurrentBlockData;
			return 2;
		case EXC_BLOCK_END: 
			iTemp=FS_ReqNewBlock(iNewBlock,iCurrentBlock);
			//std::cout<<"find new block"<<endl;
			if (iTemp==0)
			{
				break;
			}
			else {
				delete pCurrentBlockData;
				return 2;
			}
			break;
		case 0: 
			//std::cout<<"ok"<<endl;
			;
		}
		iCurrentBlock=iNewBlock;
	}
	while (ttemp==0);
	delete pCurrentBlockData;	
	return 2;
}

//////////////////////////////////////////////////////////
// 函数名：	ListDir
// 功能：	列出目录中的各项内容
// 输入：   strPath -- ListDir目录, strPattern -- 匹配条件.
// 输出：	ListDirRes -- 记录，记录内容为list命令返回的内容.
//			其中DirContainer为list对象，一项对应一个目录项.
// 返回值： 0 - 正常，
//			1 - 空目录，
//			异常情况EXC_LISTDIR_FAIL见ErrorDef.h
// 备注：	一定要以/结束strPath,不管最后是否为文件；
//			通配符一律先用ParsePattern()处理后再将PatRes作为参数传入
//////////////////////////////////////////////////////////

int FS::ListDir(string strPath, struct tagParsePatternRes PatRes, struct tagListDirRes& ListDirRes)
{

	bool bHasPattern;		// Has pattern to match
	bool bIsFile;			// Item to list is a file
	int iBlockIter;			// Block number iterator
	int iStartBlock;
	int iNextBlock;
	int iPathLevel;
	char* CurPath;
	char* tempPath;
	Byte* tempBlockData;	
	//int iItemLen;			// Item Length

	if ((PatRes.sNamePattPos == -1) || ((PatRes.bIsExtAster) && (PatRes.bIsNameAster)))		// Has no pattern
		bHasPattern = false;
	else
		bHasPattern = true;
	
	tempBlockData = new Byte[iBlockSize];
	CurPath = new char[MAX_PATH_LENGTH];
	tempPath = new char[strPath.length()];

	int i = 0;
	ListDirRes.iFileCount = ListDirRes.iDirCount = ListDirRes.iTotalBytesUsed = 0;

	if (-1 == ParsePath(const_cast<char *>(strPath.c_str()), tempPath, Level(const_cast<char *>(strPath.c_str()))-1))
		return EXC_LISTDIR_FAIL;

	if (bHasPattern)		// Has pattern in user input, i.e. must be directory in strPath
	{
		GetItemInfoList(const_cast<char *>(strPath.c_str()), ListDirRes, PatRes, bHasPattern, false, false);

		ListDirRes.iTotalBytesAvail = 0;
		ListDirRes.iTotalBytesAvail = (iMaxBlockNum - iFATBlock) * iBlockSize - ListDirRes.iTotalBytesUsed;
	}
	else					// Maybe a directory of a file to list, has to judge first.
		if (NULL != strchr(tempPath,'.'))	// Must be a file HAS ExtName
		{
			bIsFile = true;
			if (0 != GetItemInfoList(const_cast<char *>(strPath.c_str()), ListDirRes, PatRes, false, bIsFile, true))
				return EXC_LISTDIR_FAIL;

			ListDirRes.iTotalBytesAvail = 0;
			ListDirRes.iTotalBytesAvail = (iMaxBlockNum - iFATBlock) * iBlockSize - ListDirRes.iTotalBytesUsed;
		}
		else							// Maybe a dir or a file
		{
			iPathLevel = Level(const_cast<char *>(strPath.c_str()));

			ParsePath(const_cast<char *>(strPath.c_str()), CurPath, 0);

			if ('\0' == *CurPath)			// Is absolute path
			{
				iStartBlock = iFATBlock;
				i = 1;						// Check for next level of path
			}
			else	                        // Is relative path
				iStartBlock = CurrentPath.iFirstBlockNum;         //Set the start block number for search

			for(; i <(iPathLevel-1); i++)		// Find the directory next to the end of strPath
			{
				ParsePath(const_cast<char *>(strPath.c_str()), CurPath, i);       // Begin search the directory
		
				do
				{
					if (0 == FindCurPath(CurPath, -1, 0, iStartBlock, iNextBlock, NULL))
					{
						ParsePath(const_cast<char *>(strPath.c_str()), tempPath, i+1); 
						break;
					}
						
					if ( EXC_BLOCK_END == FS_FindNextBlock(iStartBlock, iStartBlock))
						return ERR_FILE_NOT_FOUND;
				}
				while(true);

				iStartBlock = iNextBlock;
			}

			iBlockIter = iStartBlock;

			if (strPath == "/")		// Is root directory
			{
			do
			{
			pFS_Kernel->Read_Block(tempBlockData, iBlockIter, iBlockSize);

			for (int k = 0; k < (iBlockSize/iDirItemSize); k++)
			{
				//char* temp;

				//temp = new char[File_Name_Len];
				//memcpy(temp, tempBlockData, File_Name_Len + File_ExtName_Len);

				if ((((int)tempBlockData[File_Name_Len+File_ExtName_Len]) % 4) == 0)      // Directory Item is empty
				{
					tempBlockData += iDirItemSize;
					continue;
				}
					

				if ((bHasPattern) && (!IsPatternMatch(tempBlockData, PatRes)))		// If has pattern, and NOT matches, continue to next DirItem
				{
					tempBlockData += iDirItemSize;
					continue;
				}
				else	// Has pattern which matches with pattern, or there's no pattern to match, then just copy DirItem into ListInfo list
				{
					if (0 != CopyDirItem(tempBlockData, ListDirRes, false))
						return EXC_GETITEMINFOLIST;
					tempBlockData += iDirItemSize;
				}
				//delete[] temp;		
			} // end for
			if (EXC_BLOCK_END == FS_FindNextBlock(iBlockIter, iBlockIter))
				break;
		}while(true);	// end do
		ListDirRes.iTotalBytesAvail = (iMaxBlockNum - iFATBlock) * iBlockSize - ListDirRes.iTotalBytesUsed;
	}
    else      // Is NOT root directory
    {
      do
			{
				pFS_Kernel->Read_Block(tempBlockData, iBlockIter, iBlockSize);

				//int iMatch;				// Match time must <= 2
				
				//iMatch = 0;				// Default is NOT match

				for (int k = 0; k < (iBlockSize/iDirItemSize); k++)
				{
					
					char* strName;			// Temp storage for test with pattern
					char* strExt;

					strName = new char[File_Name_Len];
					strExt = new char[File_ExtName_Len];

					//memcpy(strName, tempBlockData, File_Name_Len + File_ExtName_Len);
					if ((((int)tempBlockData[File_Name_Len+File_ExtName_Len]) % 4) == 0)
					{
						tempBlockData += iDirItemSize;
						continue;
					}
					else
					{
						memcpy(strName, tempBlockData, File_Name_Len);
						strName[File_Name_Len] = '\0';
					}
		
					if (0 != strcmp(tempPath, strName)) 		// Name not match
					{
						tempBlockData += iDirItemSize;
						continue;
					}
					else	// Found the same name, next decide it's file or directory 
					{
						Byte* temp;
						Byte* pOffset;
						
						temp = new Byte;
						pOffset = new Byte[iBlockSize];
						temp = tempBlockData + File_Name_Len + File_ExtName_Len;
						memcpy(pOffset, temp, DirItem_AttriLen);
					
						if (1 == ((int) * pOffset % 4))			// Is directory
						{
							//iMatch++;
							bIsFile = false;
							
							if (0 != GetItemInfoList(const_cast<char *>(strPath.c_str()), ListDirRes, PatRes, false, bIsFile, false))
								return EXC_GETITEMINFOLIST;

							ListDirRes.iTotalBytesAvail = (iMaxBlockNum - iFATBlock) * iBlockSize - ListDirRes.iTotalBytesUsed;
							return 0;
							//if (iMatch == 2)		// @Rick New -- already got 2, quit
							//	break;
						}
						else
							if (2 == ((int) * pOffset % 4 ))		// Is file, has NO ExtName
							{
								//iMatch++;
								if (0 != CopyDirItem(tempBlockData, ListDirRes, false))
									return EXC_GETITEMINFOLIST;
								ListDirRes.iTotalBytesAvail = (iMaxBlockNum - iFATBlock) * iBlockSize - ListDirRes.iTotalBytesUsed;
								return 0;
								//if (iMatch == 2)	// @Rick new -- already got 2, quit
								//	break;
							}
						tempBlockData += iDirItemSize;
						//delete[] pOffset;
					}	// end else
					//delete[] strName;
					//delete[] strExt;
					
				}// end for
				if (EXC_BLOCK_END == FS_FindNextBlock(iBlockIter, iBlockIter))
					break;
			}while(true);	// end do
		}
  }
		
		//delete[] CurPath;
		//delete[] tempBlockData;		// Destructor for pointers
		//delete[] pOffset;
		//delete[] tempPath;
	
		return 0;
}

//*/

//////////////////////////////////////////////////////////
// 函数名：	ParsePattern
// 功能：	处理命令中的通配符
// 输入：   strPatttern -- 命令中的通配符
// 输出：	PatternRes -- 记录此通配符代表信息的记录
//          bIsNameAster, bIsExtAster -- Name和Extname是否仅为"*"
//			sNamePattPos, sExtPattPos -- Name和Extname中非"*"的字符位置
//	               0 -- "abc*"  1 -- "*abc"  2 -- "abc"
//				   sNamePattPos = -1 表示通配符为空
//          strNamePatt, strExtPatt -- Name和Extname中的非"*"的字符
// 返回值： 0 - 正常，异常情况EXC_PARSE_PATTERN见ErrorDef.h
// 备注：	
//////////////////////////////////////////////////////////

int FS::ParsePattern(string strPattern, struct tagParsePatternRes& PatternRes)
{
	PatternRes.bIsExtAster = PatternRes.bIsNameAster = false;
	PatternRes.sExtPattPos = PatternRes.sNamePattPos = 0;
	PatternRes.strExtPatt = PatternRes.strNamePatt = "\0";

	if (strPattern.length() == 0)		// Null pattern
	{
		PatternRes.sNamePattPos = -1;	// Mark for null pattern

		return 0;
	}

	int iDotPos = strPattern.find(".");		// Get position of "."
	int iAsterPos = strPattern.find("*");	// Get position of "*"
             
	if (((iDotPos == -1) || (iAsterPos == -1)) && (strPattern.length() == 1))	// Pattern is just one "." or "*"
	{
		PatternRes.bIsNameAster = PatternRes.bIsExtAster = true;
		PatternRes.sNamePattPos = PatternRes.sExtPattPos = 0;
		PatternRes.strNamePatt = PatternRes.strExtPatt = "\0";

		return 0;
	}
		
	string strNamePattern = strPattern.substr(0, iDotPos);					// Get name pattern
	string strExtPattern = strPattern.substr(iDotPos + 1, strPattern.length());	// Get extname pattern

	if (-1 == iAsterPos)
	{
		PatternRes.bIsNameAster = PatternRes.bIsExtAster = false;
		PatternRes.sNamePattPos = PatternRes.sExtPattPos = 2;
		PatternRes.strNamePatt = strNamePattern;
		PatternRes.strExtPatt = strExtPattern;
			
		return 0;
	}

	if ((-1 != strNamePattern.find("*")) && (-1 != strExtPattern.find("*")) && (strExtPattern.length() >= 2) && (strNamePattern.length() >= 2))	// abc*.abc*
	{
		bool ParseOK = false;

		PatternRes.bIsNameAster = PatternRes.bIsExtAster = false;

		if (strNamePattern.find("*") == (strNamePattern.length() - 1))
		{
			PatternRes.sNamePattPos = 0;
			PatternRes.strNamePatt = strNamePattern.substr(0, strNamePattern.find("*"));

			ParseOK = true;
		}
		else
		{
			PatternRes.sNamePattPos = 1;
			PatternRes.strNamePatt = strNamePattern.substr(strNamePattern.find("*")+1, strNamePattern.length());

			ParseOK = true;
		}
		if (strExtPattern.find("*") == (strExtPattern.length() - 1))
		{
			PatternRes.sExtPattPos = 0;
			PatternRes.strExtPatt = strExtPattern.substr(0, strExtPattern.find("*"));

			ParseOK = true;
		}
		else
		{
			PatternRes.sExtPattPos = 1;
			PatternRes.strExtPatt = strExtPattern.substr(strExtPattern.find("*")+1, strExtPattern.length());

			ParseOK = true;
		}

		if (ParseOK)
			return 0;
		else 
			return EXC_PARSE_PATTERN;
	}

	if ((strNamePattern.find("*") == 0) && (strExtPattern.find("*") == 0) && (strNamePattern.length() == 1) && (strExtPattern.length() == 1))	// Pattern is "*.*"
	{
		PatternRes.bIsNameAster = PatternRes.bIsExtAster = true;
		PatternRes.sNamePattPos = PatternRes.sExtPattPos = 0;
		PatternRes.strNamePatt = PatternRes.strExtPatt = "\0";

		return 0;
	}

	if ((strNamePattern.find("*") == 0) && (strNamePattern.length() == 1))
		if (strExtPattern.length() == 0)					// Pattern is "*."
		{
			PatternRes.bIsNameAster = PatternRes.bIsExtAster = true;
			PatternRes.sNamePattPos = PatternRes.sExtPattPos = 0;
			PatternRes.strNamePatt = PatternRes.strExtPatt = "\0";

			return 0;
		}
		else
			if ((strExtPattern.find("*") != -1))			// Pattern is "*.abc*" or "*.abc*"
			{
				PatternRes.bIsNameAster = true;
				PatternRes.bIsExtAster = false;
				PatternRes.sNamePattPos = 0;
				PatternRes.strNamePatt = "\0";
				PatternRes.sExtPattPos = (((strExtPattern.find("*") + 1) == strExtPattern.length())? 0:1);	// Get the position of asterisk in extname
				if (PatternRes.sExtPattPos == 0)
					PatternRes.strExtPatt = strExtPattern.substr(0, strExtPattern.find("*"));
				else
					PatternRes.strExtPatt = strExtPattern.substr(strExtPattern.find("*") + 1, strExtPattern.length());
				
				return 0;
			}
			else					// Pattern is "*.abc"
			{
				PatternRes.bIsNameAster = true;
				PatternRes.bIsExtAster = false;
				PatternRes.sNamePattPos = 0;
				PatternRes.strNamePatt = "\0";
				PatternRes.sExtPattPos = 2;
				PatternRes.strExtPatt = strExtPattern;

				return 0;
			}

	if (strExtPattern.find("*") == 0)
		if (strNamePattern.length() == 0)					// Pattern is ".*"
		{
			PatternRes.bIsNameAster = PatternRes.bIsExtAster = true;
			PatternRes.sNamePattPos = PatternRes.sExtPattPos = 0;
			PatternRes.strNamePatt = PatternRes.strExtPatt = "\0";

			return 0;
		}
		else
			if (strNamePattern.find("*") != -1)				// Pattern is "*abc.*" or "abc*.*"
			{
				PatternRes.bIsExtAster = true;
				PatternRes.bIsNameAster = false;
				PatternRes.sExtPattPos = 0;
				PatternRes.strExtPatt = "\0";
				PatternRes.sNamePattPos = (((strNamePattern.find("*") + 1) == strNamePattern.length())? 0:1);	// Get the position of asterisk in name
				if (PatternRes.sNamePattPos == 0)
					PatternRes.strNamePatt = strNamePattern.substr(0, strNamePattern.find("*"));
				else
					PatternRes.strNamePatt = strNamePattern.substr(strNamePattern.find("*") + 1, strNamePattern.length());
				
				return 0;
			}
			else											// Pattern is "abc.*"
			{
				PatternRes.bIsExtAster = true;
				PatternRes.bIsNameAster = false;
				PatternRes.sExtPattPos = 0;
				PatternRes.strExtPatt = "\0";
				PatternRes.sNamePattPos = 2;
				PatternRes.strNamePatt = strNamePattern;

				return 0;
			}
		
	return EXC_PARSE_PATTERN;		// Exception -- can't parse patterns
}

//////////////////////////////////////////////////////////
// 函数名：	IsPatternMatch
// 功能：	判断通配符匹配与否
// 输入：   pTempData -- 有待判断的目录项数据所在的缓冲区
// 输出：	无
// 返回值： true  - 匹配成功
//			false - 匹配不成功
// 备注：	
//////////////////////////////////////////////////////////

bool FS::IsPatternMatch(Byte* pTempData, struct tagParsePatternRes Pat)
{
	int bMatch;				// Is pattern match
	char* pstrName;			// Temp storage for test with pattern
	char* pstrExt;
	string strName;
	string strExt;

	bMatch = 1;				// Default is NOT match

	pstrName = new char[File_Name_Len];
	pstrExt = new char[File_ExtName_Len];

	memcpy(pstrName, pTempData, File_Name_Len);
	pstrName[File_Name_Len] = '\0';
	memcpy(pstrExt, pTempData+File_Name_Len, File_ExtName_Len);
	pstrExt[File_ExtName_Len] = '\0';

	strName = pstrName;
	strExt = pstrExt;

	if ((Pat.bIsNameAster) && !(Pat.bIsExtAster))	// Pattern is "*.NOT*"
	{
		switch (Pat.sExtPattPos)
		{
		case 0:
			if ((strExt.length() <= Pat.strExtPatt.length()) || ("." == strExt) || (".." == strExt))
					{
						bMatch = -1;
						break;
					}
			bMatch = strcmp(strExt.substr(0, Pat.strExtPatt.length()).c_str(), Pat.strExtPatt.c_str());
			break;
		case 1:
			if ((strExt.length() <= Pat.strExtPatt.length()) || ("." == strExt) || (".." == strExt))
					{
						bMatch = -1;
						break;
					}
			bMatch = strcmp(strExt.substr(strExt.length() - Pat.strExtPatt.length(), strExt.length()-1).c_str(), Pat.strExtPatt.c_str());
			break;
		case 2:
			bMatch = strcmp(strExt.c_str(), Pat.strExtPatt.c_str());
			break;
		}
	}
	else	
		if (!(Pat.bIsNameAster) && (Pat.bIsExtAster))	// Pattern is "NOT*.*"
		{
			switch (Pat.sNamePattPos)
			{
				case 0:
					if ((strName.length() <= Pat.strNamePatt.length()) || ("." == strName) || (".." == strName))
					{
						bMatch = -1;
						break;
					}
					bMatch = strcmp(strName.substr(0, Pat.strNamePatt.length()).c_str(), Pat.strNamePatt.c_str());
					break;
				case 1:
					if ((strName.length() <= Pat.strNamePatt.length()) || ("." == strName) || (".." == strName))
					{
						bMatch = -1;
						break;
					}
					bMatch = strcmp(strName.substr(strName.length() - Pat.strNamePatt.length(), strExt.length()-1).c_str(), Pat.strNamePatt.c_str());
					break;
				case 2:
					bMatch = strcmp(strName.c_str(), Pat.strNamePatt.c_str());
					break;
			}
		}
		else 
			if (!(Pat.bIsNameAster) && !(Pat.bIsExtAster))	// Pattern is "NOT*.NOT*"
			{
				int bAllMatch = 1; //Default is NOT match

				switch (Pat.sNamePattPos)
				{
				case 0:
					if ((strName.length() <= Pat.strNamePatt.length()) || ("." == strName) || (".." == strName))
					{
						bMatch = -1;
						break;
					}
					bMatch = strcmp(strName.substr(0, Pat.strNamePatt.length()).c_str(), Pat.strNamePatt.c_str());
					break;
				case 1:
					if ((strName.length() <= Pat.strNamePatt.length()) || ("." == strName) || (".." == strName))
					{
						bMatch = -1;
						break;
					}
					bMatch = strcmp(strName.substr(strName.length() - Pat.strNamePatt.length(), strExt.length()-1).c_str(), Pat.strNamePatt.c_str());
					break;
				case 2:
					bMatch = strcmp(strName.c_str(), Pat.strNamePatt.c_str());
					break;
				}
				
				bAllMatch = bMatch;		// Save the name match result

				switch (Pat.sExtPattPos)
				{
				case 0:
					if ((strExt.length() <= Pat.strExtPatt.length()) || ("." == strExt) || (".." == strExt))
					{
						bMatch = -1;
						break;
					}
					bMatch = strcmp(strExt.substr(0, Pat.strExtPatt.length()).c_str(), Pat.strExtPatt.c_str());
					break;
				case 1:
					if ((strExt.length() <= Pat.strExtPatt.length()) || ("." == strExt) || (".." == strExt))
					{
						bMatch = -1;
						break;
					}
					bMatch = strcmp(strExt.substr(strExt.length() - Pat.strExtPatt.length(), strExt.length()-1).c_str(), Pat.strExtPatt.c_str());
					break;
				case 2:
					bMatch = strcmp(strExt.c_str(), Pat.strExtPatt.c_str());
					break;
				}
				if ((0 == bMatch) && (0 == bAllMatch))	// ExtName match success and Name match success
					bMatch = 0;
				else
					bMatch = -1;
			}
			
 	if (0 != bMatch)	// bMatch != 0, not match
		return false;
	else				// bMatch == 0, match
		return true;
}

//////////////////////////////////////////////////////////
// 函数名：	CopyDirItem
// 功能：	将已经过判断符合通配符要求的目录项拷贝至List中
// 输入：   pTempData -- 有待拷贝的目录项数据所在的缓冲区
//			bHasExt -- 针对文件来讲，有没有ExtName.　有则true.
// 输出：	ListDirRes -- ListDir的返回结果
// 返回值： 0	  - 成功
//			其它  - 不成功
//			异常EXC_COPYDIRITEM见ErrorDef.h
// 备注：	
//////////////////////////////////////////////////////////

int FS::CopyDirItem(Byte* pTempData, struct tagListDirRes& ListDirRes, bool bHasExt)
{
	Byte* pOffset;
	struct tagDirItemRes ListInfo;
	int iFile;
	int iDir;
	int iTotal;

	pOffset = new Byte [iBlockSize];
	if (NULL == memcpy(pOffset, pTempData, iBlockSize))
		return EXC_COPYDIRITEM;

	ListInfo.strItemName = new char[File_Name_Len];
	ListInfo.strItemExtName = new char[File_ExtName_Len];
	ListInfo.pAttri = new Byte[DirItem_AttriLen];
	
	iFile = ListDirRes.iFileCount;
	iDir = ListDirRes.iDirCount;
	iTotal = ListDirRes.iTotalBytesUsed;
	
	if (NULL == memcpy(ListInfo.strItemName, pOffset, File_Name_Len))
		return EXC_COPYDIRITEM;
	ListInfo.strItemName[File_Name_Len] = '\0';
	if (((ListInfo.strItemName[0] == '.') && (1 == strlen(ListInfo.strItemName))) || ((ListInfo.strItemName[0] == '.') && (ListInfo.strItemName[1] == '.') && (2 == strlen(ListInfo.strItemName))))
	{
		ListInfo.strItemExtName = "";
		ListInfo.bIsFile = 0;
		ListInfo.iItemLen = 0;
		if (NULL == memcpy(ListInfo.pAttri, pOffset+File_ExtName_Len+File_Name_Len, DirItem_AttriLen))
			return EXC_COPYDIRITEM;
		if (NULL == memcpy(&ListInfo.timeLastWrite, pOffset+File_ExtName_Len+File_Name_Len+DirItem_AttriLen, DirItem_TimeDateLen))
			return EXC_COPYDIRITEM; 
		iDir++;
	}
	else
	{
		pOffset += File_Name_Len;			// + 16
		if (NULL == memcpy(ListInfo.strItemExtName, pOffset, File_ExtName_Len))
			return EXC_COPYDIRITEM;
		ListInfo.strItemExtName[File_ExtName_Len] = '\0';
		pOffset += File_ExtName_Len;		// + 19
		if (NULL == memcpy(ListInfo.pAttri, pOffset, DirItem_AttriLen))
			return EXC_COPYDIRITEM;
		
		if (1 == ((int) * pOffset % 4))			// Is directory
		{
			iDir++;
			ListInfo.bIsFile = false;
		}
		else
			if (2 == ((int) * pOffset % 4 ))		// Is file
			{
				iFile++;
				ListInfo.bIsFile = true;
			}
			
			pOffset += DirItem_AttriLen;		// + 22
			if (NULL == memcpy(&ListInfo.timeLastWrite, pOffset, DirItem_TimeDateLen))
				return EXC_COPYDIRITEM; 
			pOffset += (DirItem_TimeDateLen);	// + 26
			pOffset += 2;
			ListInfo.iItemLen = 0;
			if (NULL == memcpy(&ListInfo.iItemLen, pOffset, DirItem_FileLen))
				return EXC_COPYDIRITEM;
			int test = ListInfo.iItemLen;
	}
	
	iTotal += ListInfo.iItemLen;

	ListDirRes.iFileCount = iFile;
	ListDirRes.iDirCount = iDir;
	ListDirRes.iTotalBytesUsed = iTotal;

	ListDirRes.DirContainer.insert(ListDirRes.DirContainer.end(), 1, ListInfo);		// Insert item info into DirResContainer

	return 0;
}

//////////////////////////////////////////////////////////
// 函数名：	GetDirItemInfoList
// 功能：	查找到相应的目录，并将其中符合要求的目录项信息
//			拷贝至List中
// 输入：   strPath  -- 路径(相对或者绝对都可以)
//			PatRes  -- 通配符记录
//			bHasPattern -- 有无通配符
//			bIsFile -- 要list的是否一个已知的文件
//			bHasExt -- 针对文件来讲，有没有ExtName.　有则true.
// 输出：	ListDirRes -- ListDir结果的返回
// 返回值： 0	  - 成功
//			其它  - 不成功
// 备注：	
//////////////////////////////////////////////////////////

int FS::GetItemInfoList(char* strPath, struct tagListDirRes& ListDirRes, struct tagParsePatternRes PatRes, bool bHasPattern, bool bIsFile, bool bHasExt)
{
	int i = 0;
	int iStartBlock;
	int iNextBlock;
	int iBlockIter;
	int iPathLevel;
	char* strName;
	char* strExt;
	char* strFileName;
	char* strNamePatt;
	char* strExtPatt;
	Byte* tempBlockData;

	char CurPath[File_Name_Len+File_ExtName_Len];	// Store the path

	strName = new char[File_Name_Len];
	strExt = new char[File_ExtName_Len];
	strFileName = new char[File_Name_Len];
	strNamePatt = new char[File_Name_Len];
	strExtPatt = new char[File_ExtName_Len];
	tempBlockData = new Byte[iBlockSize];

	//tempStoreBlock = tempBlockData;

	//*
	iPathLevel = Level(strPath);

	if (-1 == ParsePath(strPath, CurPath, 0))
		return EXC_GETITEMINFOLIST;

	if ('\0' == *CurPath)			// Is absolute path
	{
		iStartBlock = iFATBlock;
		i = 1;						// Check for next level of path
	}
	else	                        // Is relative path
		iStartBlock = CurrentPath.iFirstBlockNum;         //Set the start block number for search

	//*
	if (bIsFile)
		iPathLevel--;
	//*
	
	for(; i < iPathLevel; i++)		// Find the directory next to the end of strPath
	{
		if (-1 == ParsePath(strPath, CurPath, i))       // Begin search the directory
			return EXC_GETITEMINFOLIST;
		
		do
		{
		if (0 == FindCurPath(CurPath, -1, 0, iStartBlock, iNextBlock, NULL))
			break;
		if ( EXC_BLOCK_END == FS_FindNextBlock(iStartBlock, iStartBlock))
			return ERR_FILE_NOT_FOUND;
		}
		while(true);

		iStartBlock = iNextBlock;
	}

	iBlockIter = iStartBlock;				  // Store the first block of directory
	
	if (!bIsFile)	// Is a directory to list about
	{
		do
		{
			pFS_Kernel->Read_Block(tempBlockData, iBlockIter, iBlockSize);

			for (int k = 0; k < (iBlockSize/iDirItemSize); k++)
			{
				char* temp;

				temp = new char[File_Name_Len];
				memcpy(temp, tempBlockData, File_Name_Len);

				if ((((int)tempBlockData[File_Name_Len+File_ExtName_Len]) % 4) == 0)      // Directory Item is empty
				{
					tempBlockData += iDirItemSize;
					continue;
				}
					

				if ((bHasPattern) && (!IsPatternMatch(tempBlockData, PatRes)))		// If has pattern, and NOT matches, continue to next DirItem
				{
					tempBlockData += iDirItemSize;
					continue;
				}
				else	// Has pattern which matches with pattern, or there's no pattern to match, then just copy DirItem into ListInfo list
				{
					if (0 != CopyDirItem(tempBlockData, ListDirRes, bHasExt))
						return EXC_GETITEMINFOLIST;
					tempBlockData += iDirItemSize;
				}
				//delete[] temp;		
			} // end for
			if (EXC_BLOCK_END == FS_FindNextBlock(iBlockIter, iBlockIter))
				break;
		}while(true);	// end do
	}
	else		// Is a specific file to list about
	{
		ParsePath(strPath, strFileName, Level(strPath)-1);
		if (bHasExt)
			SplitFileName(strFileName, strNamePatt, strExtPatt);
		else
			strcpy(strNamePatt, strFileName);
		do
		{
			bool bHasFound = false;

			pFS_Kernel->Read_Block(tempBlockData, iBlockIter, iBlockSize);

			for (int k = 0; k < (iBlockSize/iDirItemSize); k++)
			{
				char* temp;

				temp = new char[File_Name_Len];
				memcpy(temp, tempBlockData, File_Name_Len);

				if ((((int)tempBlockData[File_Name_Len+File_ExtName_Len]) % 4) == 0)      // Directory Item is empty
					continue;

				memcpy(strName, tempBlockData, File_Name_Len);
				strName[File_Name_Len] = '\0';
				if (bHasExt)
				{
					memcpy(strExt, tempBlockData+File_Name_Len, File_ExtName_Len);
					strExt[File_ExtName_Len] = '\0';
				}
												
				if ((0 != strcmp(strName, strNamePatt)) || ((bHasExt) && (0 != strcmp(strExt, strExtPatt))))		// Filename NOT matches, continue to next DirItem
				{
					tempBlockData += iDirItemSize;
					continue;
				}
				else	// Filename matches, copy DirItem into ListInfo list
				{
					if (0 != CopyDirItem(tempBlockData, ListDirRes, bHasExt))
						return EXC_GETITEMINFOLIST;
					return 0;
				}
			} // end for
			if (EXC_BLOCK_END == FS_FindNextBlock(iBlockIter, iBlockIter))
				break;
		}while(true);	// end do
	}
	return 0;
}
