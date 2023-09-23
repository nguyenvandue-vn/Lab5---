#include<iostream>
#include <windows.h>
#include <stdio.h>
#include <lm.h>
#include <vector>
#include <fstream>
#include <lmerr.h>

using  namespace std;
#pragma comment(lib, "Netapi32.lib")
#pragma comment(lib, "Advapi32.lib")

// Later changed to GBK but the function name was not changed
char* UnicodeToUtf8(const wchar_t* unicode)
{
	int len;
	len = WideCharToMultiByte(CP_ACP, 0, unicode, -1, NULL, 0, NULL, NULL);//CP_ACP.CP_UTF8
	char* szUtf8 = (char*)malloc(len + 1);
	memset(szUtf8, 0, len + 1);
	WideCharToMultiByte(CP_ACP, 0, unicode, -1, szUtf8, len, NULL, NULL);
	return szUtf8;
}

int main()
{
	LMSTR servername;
	LMSTR basepath;
	LMSTR username;
	DWORD  level;
	LPBYTE bufptr;
	FILE_INFO_3* printbuf;
	DWORD prefmaxlen;
	DWORD entriesread, i;
	DWORD totalentries;
	PDWORD_PTR resume_handle;

	servername = NULL;
	basepath = NULL;
	username = NULL;
	level = 3;
	prefmaxlen = sizeof(FILE_INFO_3) * 10000; // MAX_PREFERRED_LENGTH; said that it is available to comment this but always reported NULL
	bufptr = (LPBYTE)malloc(prefmaxlen);
	entriesread = 0;
	totalentries = 0;
	resume_handle = NULL;
	NET_API_STATUS nSTATUS = NetFileEnum(
		servername,
		basepath, 
		username,
		level,
		&bufptr,
		prefmaxlen,
		&entriesread,
		&totalentries,
		NULL
	);
	
	std::vector<DWORD>files;

	if ((nSTATUS == NERR_Success) || (nSTATUS == ERROR_MORE_DATA))
	{
		printbuf = (FILE_INFO_3*)bufptr;
		for (i = 0; i < entriesread; i++)
		{
			cout << (DWORD)printbuf->fi3_id << "\t";
			//cout << (DWORD)(printbuf->fi3_permissions) << "\t";
			cout << (char*)(UnicodeToUtf8(printbuf->fi3_pathname)) << "\t";
			cout << (char*)(UnicodeToUtf8(printbuf->fi3_username)) << "\t";
			cout << (DWORD)printbuf->fi3_num_locks << endl;
			files.push_back((DWORD)printbuf->fi3_id);
			printbuf++;
		}
		if (bufptr != NULL)
		{
			NetApiBufferFree(bufptr);
		}
	}

	int index = 0;
	std::cout << "Input index of string to close ";
	std::cin >> index;
	NetFileClose(NULL, files[index]);

	nSTATUS = NetFileEnum(
		servername,
		basepath, //(LMSTR)"C:\\Users\\vuong",
		username,//(LMSTR)"vuong",
		level,
		&bufptr,
		prefmaxlen,
		&entriesread,
		&totalentries,
		NULL
	);
	if ((nSTATUS == NERR_Success) || (nSTATUS == ERROR_MORE_DATA))
	{
		printbuf = (FILE_INFO_3*)bufptr;
		for (i = 0; i < entriesread; i++)
		{
			cout << (DWORD)printbuf->fi3_id << "\t";
			//cout << (DWORD)(printbuf->fi3_permissions) << "\t";
			cout << (char*)(UnicodeToUtf8(printbuf->fi3_pathname)) << "\t";
			cout << (char*)(UnicodeToUtf8(printbuf->fi3_username)) << "\t";
			cout << (DWORD)printbuf->fi3_num_locks << endl;
			files.push_back((DWORD)printbuf->fi3_id);
			printbuf++;
		}
		if (bufptr != NULL)
		{
			NetApiBufferFree(bufptr);
		}
	}
	return 0;
}
