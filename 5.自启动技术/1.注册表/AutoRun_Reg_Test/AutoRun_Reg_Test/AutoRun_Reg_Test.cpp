// AutoRun_Reg_Test.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include "AutoRun_Reg.h"


int _tmain(int argc, _TCHAR* argv[])
{
	//注册表路径：计算机\HKEY_CURRENT_USER\Software\Microsoft\Windows\CurrentVersion\Run
	// 第一种方式修改注册表:HKEY_CURRENT_USER （无需管理员）
	if (FALSE == Reg_CurrentUser("D:\\Coding\\C\\WindowsHacker\\test.exe", "UserTest520"))
	{
		printf("Reg_CurrentUser Error!\n");
	}
	
	
	/*
	注册表路径：
	计算机\HKEY_LOCAL_MACHINE\SOFTWARE\Microsoft\Windows\CurrentVersion\Run 
	
	会重定向到
	
	计算机\HKEY_LOCAL_MACHINE\SOFTWARE\WOW6432Node\Microsoft\Windows\CurrentVersion\Run
	
	*/
	// 第二种方式修改注册表:HKEY_LOCAL_MACHINE （需管理员）
	if (FALSE == Reg_LocalMachine("D:\\Coding\\C\\WindowsHacker\\test.exe", "AdminTest520"))
	{
		printf("Reg_LocalMachine Error!\n");
	}

	printf("Reg OK.\n");

	system("pause");
	return 0;
}
