// CreateProcess_Test.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include <Windows.h>


BOOL WinExec_Test(char *pszExePath, UINT uiCmdShow)
{
	UINT uiRet = 0;
	uiRet = ::WinExec(pszExePath, uiCmdShow);
	if (31 < uiRet)
	{
		return TRUE;
	}
	return FALSE;
}


BOOL ShellExecute_Test(char *pszExePath, UINT uiCmdShow)
{
	HINSTANCE hInstance = 0;
	hInstance = ::ShellExecute(NULL, NULL, pszExePath, NULL, NULL, uiCmdShow);
	if (32 < (DWORD)hInstance)
	{
		return TRUE;
	}
	return FALSE;
}


BOOL CreateProcess_Test(char *pszExePath, UINT uiCmdShow)
{
	STARTUPINFO si = { 0 };
	PROCESS_INFORMATION pi;
	BOOL bRet = FALSE;
	si.cb = sizeof(si);
	si.dwFlags = STARTF_USESHOWWINDOW;  //指定wShowWindow成员有效
	si.wShowWindow = uiCmdShow;
	bRet = ::CreateProcess(NULL, pszExePath, NULL, NULL, FALSE, CREATE_NEW_CONSOLE, NULL, NULL, &si, &pi);
	if (bRet)
	{
		//不使用的句柄最好关掉
		::CloseHandle(pi.hThread);
		::CloseHandle(pi.hProcess);
		return TRUE;
	}
	return FALSE;
}


int _tmain(int argc, _TCHAR* argv[])
{
	BOOL bRet = FALSE;
	// WinExec函数
	bRet = WinExec_Test("D:\\软件\\QQ\\Bin\\QQScLauncher.exe", SW_SHOWNORMAL);
	if (bRet)
	{
		printf("520.exe Run OK.\n");
	}
	else
	{
		printf("520.exe Run ERROR.\n");
	}
	// ShellExecute函数
	bRet = ShellExecute_Test("D:\\软件\\QQ\\Bin\\QQScLauncher.exe", SW_SHOWNORMAL);
	if (bRet)
	{
		printf("521.exe Run OK.\n");
	}
	else
	{
		printf("521.exe Run ERROR.\n");
	}
	// CreateProcess函数
	bRet = CreateProcess_Test("D:\\软件\\QQ\\Bin\\QQScLauncher.exe", SW_SHOWNORMAL);
	if (bRet)
	{
		printf("522.exe Run OK.\n");
	}
	else
	{
		printf("522.exe Run ERROR.\n");
	}

	system("pause");
	return 0;
}