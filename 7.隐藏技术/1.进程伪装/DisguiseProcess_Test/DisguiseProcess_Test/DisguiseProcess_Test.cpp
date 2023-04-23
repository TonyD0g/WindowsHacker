// DisguiseProcess_Test.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include "DisguiseProcess.h"

//先运行 test.exe ，然后用进程查看器查看test.exe的进程PID，填到下面，然后运行本项目即可隐藏
int _tmain(int argc, _TCHAR* argv[])
{
	if (FALSE == DisguiseProcess(8048, L"C:\\Windows\\explorer.exe", L"explorer.exe"))
	{
		printf("Dsisguise Process Error.\n");
	}
	printf("Dsisguise Process OK.\n");

	system("pause");
	return 0;
}

