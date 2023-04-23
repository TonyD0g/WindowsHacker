// CreateMutex_Test.cpp : 定义控制台应用程序的入口点。
//

//stdafx.h在C++中起到的作用是头文件预编译,以加快以后的编译速度
#include "stdafx.h" 
#include <Windows.h>


// 判断是否重复运行
BOOL IsAlreadyRun()
{
	//HANDLE:句柄类型
	HANDLE hMutex = NULL;
	//CreateMutex是一个计算机函数，作用是找出当前系统是否已经存在指定进程的实例。如果没有则创建一个互斥体。
	hMutex = ::CreateMutex(NULL, FALSE, "TEST");
	if (hMutex)
	{
		//创建失败则返回ERROR_ALREADY_EXISTS
		if (ERROR_ALREADY_EXISTS == ::GetLastError())
		{
			return TRUE;
		}
	}
	return FALSE;
}

//_tmain相当于main函数
int _tmain(int argc, _TCHAR* argv[])
{
	// 判断是否重复运行
	if (IsAlreadyRun())
	{
		printf("Already Run!!!!\n");
	}
	else
	{
		printf("NOT Already Run!\n");
	}

	system("pause");
	return 0;
}

