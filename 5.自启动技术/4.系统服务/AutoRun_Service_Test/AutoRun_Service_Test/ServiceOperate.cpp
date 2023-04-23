#include "stdafx.h"
#include "ServiceOperate.h"


void ShowError(char *lpszText)
{
	char szErr[MAX_PATH] = { 0 };
	::wsprintf(szErr, "%s Error!\nError Code Is:%d\n", lpszText, ::GetLastError());
#ifdef _DEBUG
	::MessageBox(NULL, szErr, "ERROR", MB_OK | MB_ICONERROR);
#endif
}


/*
	首先通过OpenSCManager函数打开服务控制管理器数据库并获取数据库的句柄。
调用CreateService 开始创建服务，指明创建的服务类型是 SERVICE_WIN32_OWN_PROCESS 系统服务
同时设置 SERVICE_AUTO_START 为开机自启动，若是其他操作，则调用OpenService打开服务，获取服务句柄。

接着根据服务句柄考虑一下操作，
	若操作是启动，则使用StartService启动服务,若操作是停止，则使用ControlService停止服务
	若操作是删除，则使用DeleteService删除服务。
最后关闭服务句柄和服务控制管理器数据库句柄。
*/

// 0 加载服务    1 启动服务    2 停止服务    3 删除服务
BOOL SystemServiceOperate(char *lpszDriverPath, int iOperateType)
{
	BOOL bRet = TRUE;
	char szName[MAX_PATH] = { 0 };

	::lstrcpy(szName, lpszDriverPath);
	// 过滤掉文件目录，获取文件名
	::PathStripPath(szName);                   

	SC_HANDLE shOSCM = NULL, shCS = NULL;
	SERVICE_STATUS ss;
	DWORD dwErrorCode = 0;
	BOOL bSuccess = FALSE;
	// 打开服务控制管理器数据库
	shOSCM = ::OpenSCManager(NULL, NULL, SC_MANAGER_ALL_ACCESS);
	if (!shOSCM)
	{
		ShowError("OpenSCManager");
		return FALSE;
	}

	if (0 != iOperateType)
	{
		// 打开一个已经存在的服务
		shCS = OpenService(shOSCM, szName, SERVICE_ALL_ACCESS);
		if (!shCS)
		{
			ShowError("OpenService");
			::CloseServiceHandle(shOSCM);
			shOSCM = NULL;
			return FALSE;
		}
	}

	switch (iOperateType)
	{
	case 0:
	{
		// 创建服务
		// SERVICE_AUTO_START   随系统自动启动
		// SERVICE_DEMAND_START 手动启动
		shCS = ::CreateService(shOSCM, szName, szName,
			SERVICE_ALL_ACCESS,
			SERVICE_WIN32_OWN_PROCESS | SERVICE_INTERACTIVE_PROCESS,
			SERVICE_AUTO_START,
			SERVICE_ERROR_NORMAL,
			lpszDriverPath, NULL, NULL, NULL, NULL, NULL);
		if (!shCS)
		{
			ShowError("CreateService");
			bRet = FALSE;
		}
		break;
	}
	case 1:
	{
		// 启动服务
		if (!::StartService(shCS, 0, NULL))
		{
			ShowError("StartService");
			bRet = FALSE;
		}
		break;
	}
	case 2:
	{
		// 停止服务
		if (!::ControlService(shCS, SERVICE_CONTROL_STOP, &ss))
		{
			ShowError("ControlService");
			bRet = FALSE;
		}
		break;
	}
	case 3:
	{
		// 删除服务
		if (!::DeleteService(shCS))
		{
			ShowError("DeleteService");
			bRet = FALSE;
		}
		break;
	}
	default:
		break;
	}
	// 关闭句柄
	if (shCS)
	{
		::CloseServiceHandle(shCS);
		shCS = NULL;
	}
	if (shOSCM)
	{
		::CloseServiceHandle(shOSCM);
		shOSCM = NULL;
	}

	return bRet;
}