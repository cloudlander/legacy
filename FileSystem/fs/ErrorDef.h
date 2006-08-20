
//----------------------------------------------------------------------------

//////////////////////////////////////////////////////////
//  错误定义头文件
//
//  创建者：pentium, rick
//
//  说明： EXC前缀表明 Exception
//         ERR前缀表明 Error
//         EXC前缀从0xF开始向下递减。
//         ERRR前缀从0x1开始递增
//         为防止与块号冲突，数值设为16位
//////////////////////////////////////////////////////////
#ifndef _ERR_DEF_H
#define _ERR_DEF_H

//  异常定义 {

#define EXC_BLOCK_END            0xFFFF
#define EXC_LISTDIR_FAIL	       0xFFFE		
#define EXC_END_OF_FILE          0xFFFD
#define EXC_FILE_EXIST           0xFFFC
#define EXC_PARSE_PATTERN        0xFFFB		//Parse pattern exception
#define EXC_PATTERN_MATCH		     0xFFFA		//Pattern match exception
#define EXC_GETITEMINFOLIST		   0xFFF9		//GetItemInfoList exception
#define EXC_PARSEPATH			       0xFFF8		//ParsePath exception
#define EXC_COPYDIRITEM			     0xFFF7		//CopyDirItem exception

//  }

//  错误定义 {

#define ERR_FILE_NOT_FOUND       0xFF10
#define ERR_DISKFILE             0xFF11
#define ERR_NOT_FORMAT           0xFF12
#define ERR_BLOCK_MAX_REACH      0xFF13
#define ERR_DISK_FULL            0xFF14
#define ERR_PARA_INVALID         0xFF15
#define ERR_NULL_FILE            0xFF16
#define ERR_POS_INVALID          0xFF17
#define ERR_DIR_DUPLICATE        0xFF18
#define ERR_DIR_NOT_FOUND        0xFF19
#define ERR_DIR_NOT_EMPTY        0xFF1A
#define ERR_ATTRI_NOT_SUPPORT    0xFF1B
#define ERR_NAME_INVALID         0xFF1C
#define ERR_BAD_COMMAND          0xFF1D
#define ERR_SYNTAX               0xFF1E
#define ERR_MODE_INVALID         0xFF1F
#define ERR_FILE_NOT_OPENED      0xFF20
#define ERR_HANDLE_INVALID       0xFF21
#define ERR_OPEN_READ            0xFF22
#define ERR_OPEN_WRITE           0xFF23
#define ERR_FILE_ALREADY_OPENED  0xFF24
#define ERR_ACCESS_DENIED        0xFF25
#define ERR_REMOVE_CURRENT_DIR   0xFF26
#define ERR_SUCCESS              0x0
//  }
//----------------------------------------------------------------------------

#endif 