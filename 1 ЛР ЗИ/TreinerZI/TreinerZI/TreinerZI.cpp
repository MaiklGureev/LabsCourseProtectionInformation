// TreinerZI.cpp: определяет точку входа для консольного приложения.
//

#include "stdafx.h"
#include "windows.h"
#define PROGNAME L".\\crackme.exe"
STARTUPINFO si;
PROCESS_INFORMATION pi;
WORD nopnop = 0x9090;
DWORD old_flags;
DWORD p = 0x00411442;

int _tmain(int argc, _TCHAR* argv[])
{
	CreateProcess (PROGNAME,L"",NULL,NULL,TRUE,0,NULL,NULL,
		(STARTUPINFO *)&si,(PROCESS_INFORMATION *) &pi);
	HANDLE h=OpenProcess(PROCESS_VM_OPERATION|PROCESS_VM_READ|PROCESS_VM_WRITE,
		TRUE,pi.dwProcessId);
	VirtualProtectEx (h,(void *) p, 2, PAGE_EXECUTE_READWRITE,&old_flags);
	WriteProcessMemory(h,(void *)p, (void *) &nopnop,2, NULL);
	CloseHandle(h);
	system ("pause >nul");
}

