// APC_Test.cpp : �������̨Ӧ�ó������ڵ㡣
//

#include "stdafx.h"
#include "ApcInject.h"


int _tmain(int argc, _TCHAR* argv[])
{
	BOOL bRet = FALSE;

	// APCע��
#ifdef _WIN64
	bRet = ApcInjectDll("explorer.exe", "D:\\Coding\\C\\WindowsHacker\\3.ע�뼼��\\4.APCע��\\APC_Test\\x64\Debug\\TestDll.dll");
#else
	bRet = ApcInjectDll("test.exe", "D:\\Coding\\C\\WindowsHacker\\3.ע�뼼��\\4.APCע��\\APC_Test\Debug\\TestDll.dll");
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

