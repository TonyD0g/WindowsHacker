#include"windows.h"
#include"stdio.h"
#include"string.h"

//输出 "存在报错地方" 函数
void FreeRes_ShowError(char pszText[])
{
	char szErr[MAX_PATH] = { 0 };
	::wsprintf(szErr, "%s Error[%d]\n", pszText, ::GetLastError());
#ifdef _DEBUG
	::MessageBox(NULL, szErr, "ERROR", MB_OK | MB_ICONERROR);
#endif
}

BOOL FreeMyResource(UINT uiResouceName, char* lpszResourceType, char* lpszSaveFileName)
{
	char str[20] = "FindResource";
	// 获取指定模块里的指定资源
	HRSRC hRsrc = ::FindResource(NULL, MAKEINTRESOURCE(uiResouceName), lpszResourceType);
	if (NULL == hRsrc)
	{

		FreeRes_ShowError(str);
		return FALSE;
	}
	// 获取资源的大小
	DWORD dwSize = ::SizeofResource(NULL, hRsrc);
	if (0 >= dwSize)
	{
		strcpy_s(str, "SizeofResource");
		FreeRes_ShowError(str);
		return FALSE;
	}
	// 将资源加载到内存里
	HGLOBAL hGlobal = ::LoadResource(NULL, hRsrc);
	if (NULL == hGlobal)
	{
		strcpy_s(str, "LoadResource");
		FreeRes_ShowError(str);
		return FALSE;
	}
	// 锁定资源
	LPVOID lpVoid = ::LockResource(hGlobal);
	if (NULL == lpVoid)
	{
		strcpy_s(str, "LockResource");
		FreeRes_ShowError(str);
		return FALSE;
	}

	// 保存资源为文件
	FILE* fp = NULL;
	fopen_s(&fp, lpszSaveFileName, "wb+");
	if (NULL == fp)
	{
		strcpy_s(str, "LockResource");
		FreeRes_ShowError(str);
		return FALSE;
	}
	fwrite(lpVoid, sizeof(char), dwSize, fp);
	fclose(fp);

	return TRUE;
}

int main() {
	// 要释放的文件，详见 资源释放的作用.txt的[利用VS进行资源的加载与释放：]
	char szSaveName[MAX_PATH] = "520.txt";
	//资源类型，详见 资源释放的作用.txt的[利用VS进行资源的加载与释放：]
	char str1[] = "MYRES";
	// 释放程序里的资源
	BOOL bRet = FreeMyResource(130, str1, szSaveName);
	if (FALSE == bRet)
	{
		//::MessageBox(NULL, "Free Resource Error!", "ERROR", MB_OK);
	}
	else
	{
		::MessageBox(NULL, "Free Resource OK!", "OK", MB_OK);
	}
	return 0;
}