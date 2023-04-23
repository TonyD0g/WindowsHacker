#include "stdafx.h"
#include "AdjustTokenPrivilegesTest.h"


void ShowError(char *pszText)
{
	char szErr[MAX_PATH] = {0};
	::wsprintf(szErr, "%s Error[%d]\n", pszText, ::GetLastError());
	::MessageBox(NULL, szErr, "ERROR", MB_OK);
}

/*
首先要获取进程的访问令牌，然后将访问令牌的权限修改为指定权限。系统不识别权限名称，
识别LUID值，所以需要根据权限名称获取对应的LUID值，之后传递给系统，实现进程访问令牌权限的修改。

*/
BOOL EnbalePrivileges(HANDLE hProcess, char *pszPrivilegesName)
{
	HANDLE hToken = NULL;
	LUID luidValue = {0};
	TOKEN_PRIVILEGES tokenPrivileges = {0};
	BOOL bRet = FALSE;
	DWORD dwRet = 0;


	// 打开进程令牌并获取具有 TOKEN_ADJUST_PRIVILEGES 权限的进程令牌句柄
	// 只有 TOKEN_ADJUST_PRIVILEGES 才能修改权限
	bRet = ::OpenProcessToken(hProcess, TOKEN_ADJUST_PRIVILEGES, &hToken);
	if (FALSE == bRet)
	{
		ShowError("OpenProcessToken");
		return FALSE;
	}

	// 获取本地系统的 pszPrivilegesName 特权的LUID值
	bRet = ::LookupPrivilegeValue(NULL, pszPrivilegesName, &luidValue);
	if (FALSE == bRet)
	{
		ShowError("LookupPrivilegeValue");
		return FALSE;
	}

	// 设置提升权限信息：
	
	tokenPrivileges.PrivilegeCount = 1;	//新特权的数量
	tokenPrivileges.Privileges[0].Luid = luidValue; //第一个特权的Luid值
	tokenPrivileges.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED; //第一个特权的属性，SE_PRIVILEGE_ENABLED表示启动该特权
	
	//调用该函数进行权限修改																 // 提升进程令牌访问权限
	bRet = ::AdjustTokenPrivileges(hToken, FALSE, &tokenPrivileges, 0, NULL, NULL);
	if (FALSE == bRet)
	{
		ShowError("AdjustTokenPrivileges");
		return FALSE;
	}
	else
	{
		// 根据错误码判断是否特权都设置成功
		dwRet = ::GetLastError();
		if (ERROR_SUCCESS == dwRet)
		{
			return TRUE;
		}
		else if (ERROR_NOT_ALL_ASSIGNED == dwRet)
		{
			ShowError("ERROR_NOT_ALL_ASSIGNED");
			return FALSE;
		}
	}

	return FALSE;
}