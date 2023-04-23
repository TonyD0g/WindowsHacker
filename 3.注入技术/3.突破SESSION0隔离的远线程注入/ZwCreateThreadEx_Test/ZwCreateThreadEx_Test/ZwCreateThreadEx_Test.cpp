// ZwCreateThreadEx_Test.cpp : 定义控制台应用程序的入口点。
//注意：以管理员身份权限运行.
//由于会话隔离，在系统服务程序里不能显示程序窗体,也不能以用常规方式创建用户进程.
//为了解决服务层和用户层的交互问题，必须用WTS开头的函数来实现这些函数.

#include "stdafx.h"
#include "InjectDll.h"
#include "AdjustTokenPrivilegesTest.h"


int _tmain(int argc, _TCHAR* argv[])
{
	// 提升当前进程令牌权限
	EnbalePrivileges(::GetCurrentProcess(), SE_DEBUG_NAME);
	// 远线程注入 DLL
#ifndef _WIN64
	BOOL bRet = ZwCreateThreadExInjectDll(8080, "C:\\Users\\DemonGan\\Desktop\\ZwCreateThreadEx_Test\\Debug\\TestDll.dll");
#else 
	BOOL bRet = ZwCreateThreadExInjectDll(15912,"D:\\Coding\\C\\WindowsHacker\\3.注入技术\\3.突破SESSION0隔离的远线程注入\\ZwCreateThreadEx_Test\\x64\Debug\\TestDll.dll");
#endif
	if (FALSE == bRet)
	{
		printf("Inject Dll Error.\n");
	}
	else{ 
		printf("Inject Dll OK.\n"); 
	}
	
	system("pause");
	return 0;
}

