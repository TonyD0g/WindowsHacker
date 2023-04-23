// GlobalHook_Test.cpp : 定义 DLL 应用程序的导出函数。
//

#include "stdafx.h"


extern HMODULE g_hDllModule;
// 共享内存
//创建了mydata数据段
#pragma data_seg("mydata")
    HHOOK g_hHook = NULL;
#pragma data_seg()
//将mydata数据段设置为可读，可写，可共享的数据段
#pragma comment(linker, "/SECTION:mydata,RWS")


// 钩子回调函数,钩子回调函数名称任意，函数参数和返回值必须是这几个.
//该函数必须写在dll模块里,并指定该dll模块的句柄.
LRESULT GetMsgProc(
	int code,//当前钩子句柄
	WPARAM wParam,
	LPARAM lParam)
{
	//将钩子信息传递给当前钩子链中的下一个钩子过程。 钩子过程可以在处理钩子信息之前或之后调用此函数。 
	//返回0则表示中断钩子传递，对钩子进行拦截.
	return ::CallNextHookEx(g_hHook, code, wParam, lParam);
}


// 设置全局钩子
BOOL SetGlobalHook()
{
	//1. WH_GETMESSAGE：监视消息队列,使所有使用消息队列的进程加载此钩子.
	//2. GetMsgProc：钩子回调函数
	//3. g_hDllModule：包含钩子回调函数的dll模块句柄,如果要设置全局钩子,则该参数必须指定dll模块句柄.
	//4. 第四个参数表示与钩子关联的线程ID,0表示全局钩子
	g_hHook = ::SetWindowsHookEx(WH_GETMESSAGE, (HOOKPROC)GetMsgProc, g_hDllModule, 0);
	if (NULL == g_hHook)
	{
		return FALSE;
	}
	return TRUE;
}


// 卸载钩子：钩子不用时要卸载钩子
BOOL UnsetGlobalHook()
{
	//因为g_hHook是共享内存里的，所以所有进程共用，如果某个进程改变了这个值，其他所有进程都改变.
	if (g_hHook)
	{
		::UnhookWindowsHookEx(g_hHook);
	}
	return TRUE;
}