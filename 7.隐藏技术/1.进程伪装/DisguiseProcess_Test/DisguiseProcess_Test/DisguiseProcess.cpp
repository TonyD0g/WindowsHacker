#include "stdafx.h"
#include "DisguiseProcess.h"


void ShowError(char *pszText)
{
	char szErr[MAX_PATH] = { 0 };
	::wsprintf(szErr, "%s Error[%d]\n", pszText, ::GetLastError());
	::MessageBox(NULL, szErr, "ERROR", MB_OK);
}

/*
由于进程空间独立性的缘故，所以需要通过调用WIN32 API函数 ReadProcessMemory/WriteProcessMemory 来读写目标进程内存

简单流程：
1.根据进程的PID打开指定进程，并获取句柄

2.从ntdll.dll中获取 NtQueryInformationProcess 函数
  获取指定进程的基本信息 PROCESS_BASIC_INFORMATION，并从中获取指定进程的PEB

3.根据进程环境块中的 ProcessParameters 来获取指定进程的 RTL_USER_PROCESS_PARAMETERS 信息，
  这是因为PEB的路径信息，命令行信息存储在这个结构体中

4.调用 ReadProcessMemory 和 WriteProcessMemory 函数可以修改PEB中的路径信息，命令行信息，从而实现进程伪装

*/

// 修改指定进程的进程环境块PEB中的路径和命令行信息, 实现进程伪装
BOOL DisguiseProcess(DWORD dwProcessId, wchar_t *lpwszPath, wchar_t *lpwszCmd)
{
	// 打开进程获取句柄
	HANDLE hProcess = ::OpenProcess(PROCESS_ALL_ACCESS, FALSE, dwProcessId);
	if (NULL == hProcess)
	{
		ShowError("OpenProcess");
		return FALSE;
	}

	typedef_NtQueryInformationProcess NtQueryInformationProcess = NULL;
	PROCESS_BASIC_INFORMATION pbi = { 0 };
	PEB peb = { 0 };
	RTL_USER_PROCESS_PARAMETERS Param = { 0 };
	USHORT usCmdLen = 0;
	USHORT usPathLen = 0;
	// 需要通过 LoadLibrary、GetProcessAddress 从 ntdll.dll 中获取地址
	NtQueryInformationProcess = (typedef_NtQueryInformationProcess)::GetProcAddress(
		::LoadLibrary("ntdll.dll"), "NtQueryInformationProcess");
	if (NULL == NtQueryInformationProcess)
	{
		ShowError("GetProcAddress");
		return FALSE;
	}

	// 获取指定进程的基本信息
	NTSTATUS status = NtQueryInformationProcess(hProcess, ProcessBasicInformation, &pbi, sizeof(pbi), NULL);
	if (!NT_SUCCESS(status))
	{
		ShowError("NtQueryInformationProcess");
		return FALSE;
	}

	/*
  	    注意在读写其他进程的时候，注意要使用ReadProcessMemory/WriteProcessMemory进行操作，
	    每个指针指向的内容都需要获取，因为指针只能指向本进程的地址空间，必须要读取到本进程空间。
	    要不然一直提示位置访问错误!
	*/
	// 获取指定进程进本信息结构中的PebBaseAddress
	::ReadProcessMemory(hProcess, pbi.PebBaseAddress, &peb, sizeof(peb), NULL);

	// 获取指定进程环境块结构中的ProcessParameters, 注意指针指向的是指定进程空间中
	::ReadProcessMemory(hProcess, peb.ProcessParameters, &Param, sizeof(Param), NULL);

	// 修改指定进程环境块PEB中命令行信息, 注意指针指向的是指定进程空间中
	usCmdLen = 2 + 2 * ::wcslen(lpwszCmd);
	::WriteProcessMemory(hProcess, Param.CommandLine.Buffer, lpwszCmd, usCmdLen, NULL);
	::WriteProcessMemory(hProcess, &Param.CommandLine.Length, &usCmdLen, sizeof(usCmdLen), NULL);
	// 修改指定进程环境块PEB中路径信息, 注意指针指向的是指定进程空间中
	usPathLen = 2 + 2 * ::wcslen(lpwszPath);
	::WriteProcessMemory(hProcess, Param.ImagePathName.Buffer, lpwszPath, usPathLen, NULL);
	::WriteProcessMemory(hProcess, &Param.ImagePathName.Length, &usPathLen, sizeof(usPathLen), NULL);

	return TRUE;
}