// ZwCreateThreadEx_Test.cpp : �������̨Ӧ�ó������ڵ㡣
//ע�⣺�Թ���Ա���Ȩ������.
//���ڻỰ���룬��ϵͳ��������ﲻ����ʾ������,Ҳ�������ó��淽ʽ�����û�����.
//Ϊ�˽���������û���Ľ������⣬������WTS��ͷ�ĺ�����ʵ����Щ����.

#include "stdafx.h"
#include "InjectDll.h"
#include "AdjustTokenPrivilegesTest.h"


int _tmain(int argc, _TCHAR* argv[])
{
	// ������ǰ��������Ȩ��
	EnbalePrivileges(::GetCurrentProcess(), SE_DEBUG_NAME);
	// Զ�߳�ע�� DLL
#ifndef _WIN64
	BOOL bRet = ZwCreateThreadExInjectDll(8080, "C:\\Users\\DemonGan\\Desktop\\ZwCreateThreadEx_Test\\Debug\\TestDll.dll");
#else 
	BOOL bRet = ZwCreateThreadExInjectDll(15912,"D:\\Coding\\C\\WindowsHacker\\3.ע�뼼��\\3.ͻ��SESSION0�����Զ�߳�ע��\\ZwCreateThreadEx_Test\\x64\Debug\\TestDll.dll");
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

