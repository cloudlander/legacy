#include <windows.h>
#include <stdio.h>
#include <iostream>
#include "md5.h"
using namespace std;
void
DisplayErrorText()
{
	DWORD dwLastError=GetLastError();
    HMODULE hModule = NULL; // default to system source
    LPSTR MessageBuffer;
    DWORD dwBufferLength;
	
    DWORD dwFormatFlags = FORMAT_MESSAGE_ALLOCATE_BUFFER |
        FORMAT_MESSAGE_IGNORE_INSERTS |
        FORMAT_MESSAGE_FROM_SYSTEM ;
	
    //
    // If dwLastError is in the network range, 
    //  load the message source.
    //
	
	//
    // Call FormatMessage() to allow for message 
    //  text to be acquired from the system 
    //  or from the supplied module handle.
    //
	
    if(dwBufferLength = FormatMessageA(
        dwFormatFlags,
        hModule, // module to get message from (NULL == system)
        dwLastError,
        MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), // default language
        (LPSTR) &MessageBuffer,
        0,
        NULL
        ))
    {
        DWORD dwBytesWritten;
//		AfxMessageBox(MessageBuffer);
//*
        //
        // Output message string on stderr.
        //
        WriteFile(
            GetStdHandle(STD_ERROR_HANDLE),
            MessageBuffer,
            dwBufferLength,
            &dwBytesWritten,
            NULL
            );
//*/		
        //
        // Free the buffer allocated by the system.
        //
        LocalFree(MessageBuffer);
    }
	
    //
    // If we loaded a message source, unload it.
    //
}
int main(int argc,char** argv)
{
	char* input=argv[1];
	int count;
	int length_read;
	unsigned long file_size;
	int incSize=64*1024;
	unsigned char *pbuf;
	MD5* md5sess=new MD5;
	unsigned char md5sum[16];
#if 1
	FILE* file;
	unsigned char* buf=new unsigned char[incSize];
	md5sess->SessionInit();
	if ((file = fopen (input, "rb")) == NULL)
		cerr<<"can't be opened";
	fseek(file,0,SEEK_END);
	file_size=ftell(file);
	fseek(file,0,SEEK_SET);
	while (length_read = fread (buf, 1, incSize, file))
	{
		md5sess->SessionUpdate(buf,length_read);
		cout<<'-';
	}
	md5sess->SessionFinal(md5sum);
	fclose (file);
#endif
#if 0
	HANDLE hmap;
	HANDLE hFile=CreateFile(input,GENERIC_READ,0,
		NULL,OPEN_EXISTING,FILE_ATTRIBUTE_NORMAL,NULL);

	file_size=GetFileSize(hFile,NULL);
	if(hFile==INVALID_HANDLE_VALUE)
	{
		DisplayErrorText();
		exit(-1);
	}
	
	hmap=CreateFileMapping(hFile,NULL,
						PAGE_READONLY|SEC_COMMIT,0,0,NULL);


	if(hmap==(HANDLE)NULL)
	{
		DisplayErrorText();
		exit(-1);
	}
/*
		pbuf=(unsigned char*)MapViewOfFile(hmap,FILE_MAP_READ,0,0,0);
		if(NULL==pbuf)
		{
			DisplayErrorText();
			VERIFY(CloseHandle(hmap));
			curJob->SetStatus(FAILURE);
			goto done;
			ASSERT(0);
		}
*/
		
		count=0;
		length_read=0;
		

		md5sess->SessionInit();
		while(length_read+incSize <= file_size)
		{
//			md5sess->SessionUpdate(pbuf+length_read,incSize);
//*
			pbuf=(unsigned char*)MapViewOfFile(hmap,FILE_MAP_READ,0,length_read,incSize);
			if(NULL==pbuf)
			{
				DisplayErrorText();
				exit(-1);
			}
			md5sess->SessionUpdate(pbuf,incSize);
//*/
			length_read+=incSize;
			cout<<"-";
			UnmapViewOfFile(pbuf);
		}

		pbuf=(unsigned char*)MapViewOfFile(hmap,FILE_MAP_READ,0,length_read,file_size-length_read);
		if(file_size>length_read)
//			md5sess->SessionUpdate(pbuf+length_read,file_size-length_read);
			md5sess->SessionUpdate(pbuf,file_size-length_read);
		md5sess->SessionFinal(md5sum);
		UnmapViewOfFile(pbuf);
//		VERIFY(UnmapViewOfFile(pbuf));
		CloseHandle(hmap);
#endif
		return 0;
}