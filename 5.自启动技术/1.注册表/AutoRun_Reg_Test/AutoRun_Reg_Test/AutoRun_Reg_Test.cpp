// AutoRun_Reg_Test.cpp : �������̨Ӧ�ó������ڵ㡣
//

#include "stdafx.h"
#include "AutoRun_Reg.h"


int _tmain(int argc, _TCHAR* argv[])
{
	//ע���·���������\HKEY_CURRENT_USER\Software\Microsoft\Windows\CurrentVersion\Run
	// ��һ�ַ�ʽ�޸�ע���:HKEY_CURRENT_USER ���������Ա��
	if (FALSE == Reg_CurrentUser("D:\\Coding\\C\\WindowsHacker\\test.exe", "UserTest520"))
	{
		printf("Reg_CurrentUser Error!\n");
	}
	
	
	/*
	ע���·����
	�����\HKEY_LOCAL_MACHINE\SOFTWARE\Microsoft\Windows\CurrentVersion\Run 
	
	���ض���
	
	�����\HKEY_LOCAL_MACHINE\SOFTWARE\WOW6432Node\Microsoft\Windows\CurrentVersion\Run
	
	*/
	// �ڶ��ַ�ʽ�޸�ע���:HKEY_LOCAL_MACHINE �������Ա��
	if (FALSE == Reg_LocalMachine("D:\\Coding\\C\\WindowsHacker\\test.exe", "AdminTest520"))
	{
		printf("Reg_LocalMachine Error!\n");
	}

	printf("Reg OK.\n");

	system("pause");
	return 0;
}
