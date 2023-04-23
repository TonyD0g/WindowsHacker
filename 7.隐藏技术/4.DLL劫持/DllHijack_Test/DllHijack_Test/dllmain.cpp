//// dllmain.cpp : 定义 DLL 应用程序的入口点。
#include "stdafx.h"

/*
例如，假设导出函数为 MessageBoxATest ，若直接转发user32.dll 中的 MessageBoxA 导出函数，那么编译指令如下

# pragma comment(linker,"/EXPORT:MessageBoxATest=user32.MessageBoxA")

上述代码中的 user32 表示 user32.dll 模块。当加载dll程序的时候，它会根据上述dll的搜索路径自动搜索并加载 user32.dll 模块到进程中。当调用 MessageBoxATest 导出函数的时候，系统会将其直接转发给 user32.dll 模块中的 MessageBoxA 导出函数去执行。
*/

BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
					 )
{
	switch (ul_reason_for_call)
	{
	case DLL_PROCESS_ATTACH:
	{
		::MessageBox(NULL, "直接转发函数方法", "From DLL Hijack", MB_OK | MB_ICONWARNING);
		break;
	}
	case DLL_THREAD_ATTACH:
	case DLL_THREAD_DETACH:
	case DLL_PROCESS_DETACH:
		break;
	}
	return TRUE;
}


// 直接转发函数
#pragma comment(linker, "/EXPORT:GetFileVersionInfoA=OLD_VERSION.GetFileVersionInfoA")
#pragma comment(linker, "/EXPORT:GetFileVersionInfoByHandle=OLD_VERSION.GetFileVersionInfoByHandle")
#pragma comment(linker, "/EXPORT:GetFileVersionInfoExA=OLD_VERSION.GetFileVersionInfoExA")
#pragma comment(linker, "/EXPORT:GetFileVersionInfoExW=OLD_VERSION.GetFileVersionInfoExW")
#pragma comment(linker, "/EXPORT:GetFileVersionInfoSizeA=OLD_VERSION.GetFileVersionInfoSizeA")
#pragma comment(linker, "/EXPORT:GetFileVersionInfoSizeExA=OLD_VERSION.GetFileVersionInfoSizeExA")
#pragma comment(linker, "/EXPORT:GetFileVersionInfoSizeExW=OLD_VERSION.GetFileVersionInfoSizeExW")
#pragma comment(linker, "/EXPORT:GetFileVersionInfoSizeW=OLD_VERSION.GetFileVersionInfoSizeW")
#pragma comment(linker, "/EXPORT:GetFileVersionInfoW=OLD_VERSION.GetFileVersionInfoW")
#pragma comment(linker, "/EXPORT:VerFindFileA=OLD_VERSION.VerFindFileA")
#pragma comment(linker, "/EXPORT:VerFindFileW=OLD_VERSION.VerFindFileW")
#pragma comment(linker, "/EXPORT:VerInstallFileA=OLD_VERSION.VerInstallFileA")
#pragma comment(linker, "/EXPORT:VerInstallFileW=OLD_VERSION.VerInstallFileW")
#pragma comment(linker, "/EXPORT:VerLanguageNameA=OLD_VERSION.VerLanguageNameA")
#pragma comment(linker, "/EXPORT:VerLanguageNameW=OLD_VERSION.VerLanguageNameW")
#pragma comment(linker, "/EXPORT:VerQueryValueA=OLD_VERSION.VerQueryValueA")
#pragma comment(linker, "/EXPORT:VerQueryValueW=OLD_VERSION.VerQueryValueW")
//


