// CreateRemoteThread_Test.cpp : �������̨Ӧ�ó������ڵ㡣
//ע�⣺��Ϊûдж��dll�ĺ�����ÿ��ע��һ�����̺����ٴ�ע��ý��̣������ý��̼��ɽ������

#include "stdafx.h"
#include "InjectDll.h"
#include "AdjustTokenPrivilegesTest.h"


int _tmain(int argc, _TCHAR* argv[])
{
	DWORD processid;
	
	// ������ǰ��������Ȩ��(��Ҫ����ԱȨ������VS2022)
	//EnbalePrivileges(::GetCurrentProcess(), SE_DEBUG_NAME);

	// Զ�߳�ע�� DLL
	//�ж��Ƿ���win64λ����win32
#ifndef _WIN64									//dwProcessId��pid����˼,��Ϊpidÿ�����������ʱ�����������Ҫ�Լ�����,����������������������û�����Process Explorer��
	BOOL bRet = CreateRemoteThreadInjectDll(1916, "D:\\Coding\\C\\WindowsHacker\\3.ע�뼼��\\2.Զ�߳�ע��\\CreateRemoteThread_Test\\Debug\\TestDll.dll");
#else 
	BOOL bRet = CreateRemoteThreadInjectDll(22180, "D:\\Coding\\C\\WindowsHacker\\3.ע�뼼��\\2.Զ�߳�ע��\\CreateRemoteThread_Test\\Debug\\TestDll.dll");
#endif
	
	if (FALSE == bRet)
	{
		printf("Inject Dll Error.\n");
	}
	else{
		printf("Inject Dll OK.\n");
	}
	
	system("pause");
	return 0;
}

