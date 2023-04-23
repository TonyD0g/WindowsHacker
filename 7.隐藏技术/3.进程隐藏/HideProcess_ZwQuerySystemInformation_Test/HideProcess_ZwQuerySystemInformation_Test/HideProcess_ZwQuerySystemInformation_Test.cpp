// HideProcess_ZwQuerySystemInformation_Test.cpp : 定义 DLL 应用程序的导出函数。
//

#include "stdafx.h"


extern HMODULE g_hModule;

#pragma data_seg("mydata")
	HHOOK g_hHook = NULL;
#pragma data_seg()
#pragma comment(linker, "/SECTION:mydata,RWS")


/*
均写在dll工程中。程序要实现的是隐藏指定进程，而不单单在自己的进程空间内隐藏指定进程。写在dll文件。可以方便将dll文件注入到其他进程空间，从而隐藏其他进程空间中的 ZwQuerySystemInformation 函数，这样就实现对其他进程空间隐藏指定进程了。
*/

// 消息全局钩子回调函数
LRESULT CALLBACK GetMsgProc(
	int code,       // hook code
	WPARAM wParam,  // removal option  
	LPARAM lParam   // message
	)
{
	// 不进行任何操作，设置全局钩子的目的就是进行DLL注入而已，主要是主入口进行的API挂钩

	return ::CallNextHookEx(g_hHook, code, wParam, lParam);
}


// 设置全局钩子
HHOOK SetHook()
{
	// 设置全局钩子
	HHOOK hHook = ::SetWindowsHookEx(WH_GETMESSAGE, (HOOKPROC)GetMsgProc, g_hModule, 0);
	if (NULL == hHook)
	{
		return NULL;
	}
	g_hHook = hHook;
	return hHook;
}


// 卸载全局钩子
BOOL UnsetHook(HHOOK hHook)
{
	if (FALSE == ::UnhookWindowsHookEx(hHook))
	{
		return FALSE;
	}
	return TRUE;
}