// CreateRemoteThread_Test.cpp : 定义控制台应用程序的入口点。
//注意：因为没写卸载dll的函数，每次注入一个进程后不能再次注入该进程，重启该进程即可解决问题

#include "stdafx.h"
#include "InjectDll.h"
#include "AdjustTokenPrivilegesTest.h"


int _tmain(int argc, _TCHAR* argv[])
{
	DWORD processid;
	
	// 提升当前进程令牌权限(需要管理员权限启动VS2022)
	//EnbalePrivileges(::GetCurrentProcess(), SE_DEBUG_NAME);

	// 远线程注入 DLL
	//判断是否是win64位或者win32
#ifndef _WIN64									//dwProcessId是pid的意思,因为pid每次启动程序的时候随机所以需要自己输入,用任务管理器看或者命令敲或者用Process Explorer看
	BOOL bRet = CreateRemoteThreadInjectDll(1916, "D:\\Coding\\C\\WindowsHacker\\3.注入技术\\2.远线程注入\\CreateRemoteThread_Test\\Debug\\TestDll.dll");
#else 
	BOOL bRet = CreateRemoteThreadInjectDll(22180, "D:\\Coding\\C\\WindowsHacker\\3.注入技术\\2.远线程注入\\CreateRemoteThread_Test\\Debug\\TestDll.dll");
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

