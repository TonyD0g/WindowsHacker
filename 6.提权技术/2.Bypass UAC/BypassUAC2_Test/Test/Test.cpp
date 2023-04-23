// Test.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include <Windows.h>

//从这调用
int _tmain(int argc, _TCHAR* argv[])
{
	char szCmdLine[MAX_PATH] = { 0 };
	char szRundll32Path[MAX_PATH] = "C:\\Windows\\system32\\rundll32.exe";
	char szDllPath[MAX_PATH] = "D:\\Coding\\C\\WindowsHackerBackup\\6.提权技术\\2.Bypass UAC\\BypassUAC2_Test\\Debug\\BypassUAC2_Test.dll";
	::sprintf_s(szCmdLine, "%s \"%s\" %s", szRundll32Path, szDllPath, "BypassUAC");
	::WinExec(szCmdLine, SW_HIDE);

	printf("Run OK.\n");
	system("pause");
	return 0;
}

