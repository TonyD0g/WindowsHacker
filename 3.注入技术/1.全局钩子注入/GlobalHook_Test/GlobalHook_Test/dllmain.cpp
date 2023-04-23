// dllmain.cpp : 定义 DLL 应用程序的入口点。
#include "stdafx.h"

HMODULE g_hDllModule = NULL;

//相当于main函数，是dll文件主入口
//标准写法就这么写的
BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
					 )
{
	//参数ul_reason_for_call指明了被调用的原因
	switch (ul_reason_for_call)
	{
	//为每个进程初始化一次
	case DLL_PROCESS_ATTACH:
	{
		//::MessageBox(NULL, "This Is From Dll!\nInject Success!", "OK", MB_OK);
		
		//进入这个目录了
		g_hDllModule = hModule;
		break;
	}
	case DLL_THREAD_ATTACH:
	case DLL_THREAD_DETACH:
	case DLL_PROCESS_DETACH:
		break;
	}
	return TRUE;
	//进入GlobalHook_Test.cpp文件执行
}

