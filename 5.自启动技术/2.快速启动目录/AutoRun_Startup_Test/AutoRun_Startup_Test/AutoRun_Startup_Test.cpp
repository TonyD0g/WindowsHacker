//AutoRun_Startup_Test.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include <Windows.h>
#include <Shlobj.h>
#pragma comment(lib, "shell32.lib")

/*
windows系统有自带的快速启动文件夹，它是最为简单的自启动方式。只要把程序放入到这个快速启动文件夹中
系统在启动时就会自动地加载并运行相应的程序，实现开机在启动功能。
快速启动目录并不是固定目录，所以通过 SHGetSpecialFolderPath 函数获取快速启动目录的路径。
快速启动目录的CSIDL标识值为CSIDL_STARTUP

然后使用CopyFile函数，将想要自启动的程序复制到快速启动目录下即可。
也可以为程序创建快捷方式，并把快捷方式放入到快速启动目录中，也同样可以达到开机自启动的效果。

*/

BOOL AutoRun_Startup(char *lpszSrcFilePath, char *lpszDestFileName)
{
	BOOL bRet = FALSE;
	char szStartupPath[MAX_PATH] = {0};
	char szDestFilePath[MAX_PATH] = {0};
	// 获取 快速启动目录 路径
	bRet = ::SHGetSpecialFolderPath(NULL, szStartupPath, CSIDL_STARTUP, TRUE);
	printf("szStartupPath=%s\n", szStartupPath);
	if (FALSE == bRet)
	{
		return FALSE;
	}
	// 构造拷贝的 目的文件路径
	::wsprintf(szDestFilePath, "%s\\%s", szStartupPath, lpszDestFileName);
	// 拷贝文件到快速启动目录下
	bRet = ::CopyFile(lpszSrcFilePath, szDestFilePath, FALSE);
	if (FALSE == bRet)
	{
		return FALSE;
	}

	return TRUE;
}


int _tmain(int argc, _TCHAR* argv[])
{
	if (FALSE == AutoRun_Startup("D:\\Coding\\C\\WindowsHacker\\test.exe", "test.exe"))
	{
		printf("Startup Error!\n");
	}
	printf("Startup OK!\n");

	system("pause");
	return 0;
}

