// APC_Test.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include "ApcInject.h"


int _tmain(int argc, _TCHAR* argv[])
{
	BOOL bRet = FALSE;

	// APC注入
#ifdef _WIN64
	bRet = ApcInjectDll("explorer.exe", "D:\\Coding\\C\\WindowsHacker\\3.注入技术\\4.APC注入\\APC_Test\\x64\Debug\\TestDll.dll");
#else
	bRet = ApcInjectDll("test.exe", "D:\\Coding\\C\\WindowsHacker\\3.注入技术\\4.APC注入\\APC_Test\Debug\\TestDll.dll");
#endif
	if (bRet)
	{
		printf("APC Inject OK.\n");
	}
	else
	{
		printf("APC Inject ERROR.\n");
	}

	system("pause");
	return 0;
}

