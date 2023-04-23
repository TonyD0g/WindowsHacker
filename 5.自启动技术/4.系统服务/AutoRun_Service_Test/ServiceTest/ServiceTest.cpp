// ServiceTest.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include <Windows.h>


// 服务入口函数以及处理回调函数
void __stdcall ServiceMain(DWORD dwArgc, char *lpszArgv);
void __stdcall ServiceCtrlHandle(DWORD dwOperateCode);
BOOL TellSCM(DWORD dwState, DWORD dwExitCode, DWORD dwProgress);
void DoTask();

// 全局变量
char g_szServiceName[MAX_PATH] = "ServiceTest.exe";    // 服务名称 
SERVICE_STATUS_HANDLE g_ServiceStatusHandle = { 0 };

/*
	自启动服务程序并不是普通的程序，而是要求程序创建服务入口点函数，否则不能创建系统服务。
创建系统服务流程：
	调用系统函数 StartServiceCtrlDispatcher 将程序主线程连接到服务控制管理程序（其中定义了服务入口点函数是 ServiceMain )
服务控制管理程序启动服务程序后，等待服务程序主函数调用 StartServiceCtrlDispatcher 函数。如果没有调用该函数时设置服务入口点会报错
	执行服务初始化任务（同时执行的多个服务有多个入口点函数），首先调用 RegisterServiceCtrlHandle 定义控制处理程序函数（本例中是 ServiceCtrlHandler )
初始化后通过 SetServiceStatus 设定运行状态，然后运行服务代码。
	在服务收到控制请求后由控制分发线程来引用（至少要有停止服务的能力）
*/

//服务程序主函数
int _tmain(int argc, _TCHAR* argv[])
{
	// 注册服务入口函数,启动“服务程序”
	SERVICE_TABLE_ENTRY stDispatchTable[] = { { g_szServiceName, (LPSERVICE_MAIN_FUNCTION)ServiceMain }, { NULL, NULL } };
	//等待服务程序主函数调用 StartServiceCtrlDispatcher 函数。如果没有调用该函数时设置服务入口点会报错
	::StartServiceCtrlDispatcher(stDispatchTable);

	return 0;
}

//服务入口点函数：ServiceMain ,用于创建服务程序
void __stdcall ServiceMain(DWORD dwArgc, char *lpszArgv)
{
	//首先调用 RegisterServiceCtrlHandler 定义控制处理程序函数（本例中是 ServiceCtrlHandle )
	g_ServiceStatusHandle = ::RegisterServiceCtrlHandler(g_szServiceName, ServiceCtrlHandle);
	
	//初始化后通过 TellSCM 函数中的 SetServiceStatus 设定运行状态，然后运行服务代码。
	TellSCM(SERVICE_START_PENDING, 0, 1);
	TellSCM(SERVICE_RUNNING, 0, 0);

	// 自己程序实现部分代码放在这里
	// !!注意!! 此处一定要为死循环, 否则在关机再开机的情况(不是点击重启), 不能创建用户进程
	while (TRUE)
	{
		Sleep(5000);
		DoTask();
	}
}


void __stdcall ServiceCtrlHandle(DWORD dwOperateCode)
{
	switch (dwOperateCode)
	{
	case SERVICE_CONTROL_PAUSE:
	{
		// 暂停
		TellSCM(SERVICE_PAUSE_PENDING, 0, 1);
		TellSCM(SERVICE_PAUSED, 0, 0);
		break;
	}
	case SERVICE_CONTROL_CONTINUE:
	{
		// 继续
		TellSCM(SERVICE_CONTINUE_PENDING, 0, 1);
		TellSCM(SERVICE_RUNNING, 0, 0);
		break;
	}
	case SERVICE_CONTROL_STOP:
	{
		// 停止
		TellSCM(SERVICE_STOP_PENDING, 0, 1);
		TellSCM(SERVICE_STOPPED, 0, 0);
		break;
	}
	case SERVICE_CONTROL_INTERROGATE:
	{
		// 询问
		break;
	}
	default:
		break;
	}
}

BOOL TellSCM(DWORD dwState, DWORD dwExitCode, DWORD dwProgress)
{
	SERVICE_STATUS serviceStatus = { 0 };
	BOOL bRet = FALSE;

	::RtlZeroMemory(&serviceStatus, sizeof(serviceStatus));
	serviceStatus.dwServiceType = SERVICE_WIN32_OWN_PROCESS;
	serviceStatus.dwCurrentState = dwState;
	serviceStatus.dwControlsAccepted = SERVICE_ACCEPT_STOP | SERVICE_ACCEPT_PAUSE_CONTINUE | SERVICE_ACCEPT_SHUTDOWN;
	serviceStatus.dwWin32ExitCode = dwExitCode;
	serviceStatus.dwWaitHint = 3000;

	//初始化后通过 SetServiceStatus 设定运行状态，然后运行服务代码。
	bRet = ::SetServiceStatus(g_ServiceStatusHandle, &serviceStatus);
	return bRet;
}

void DoTask()
{
	// 自己程序实现部分代码放在这里
	
}