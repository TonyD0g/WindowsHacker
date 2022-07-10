//AutoRun_Startup_Test.cpp : �������̨Ӧ�ó������ڵ㡣
//

#include "stdafx.h"
#include <Windows.h>
#include <Shlobj.h>
#pragma comment(lib, "shell32.lib")

/*
windowsϵͳ���Դ��Ŀ��������ļ��У�������Ϊ�򵥵���������ʽ��ֻҪ�ѳ�����뵽������������ļ�����
ϵͳ������ʱ�ͻ��Զ��ؼ��ز�������Ӧ�ĳ���ʵ�ֿ������������ܡ�
��������Ŀ¼�����ǹ̶�Ŀ¼������ͨ�� SHGetSpecialFolderPath ������ȡ��������Ŀ¼��·����
��������Ŀ¼��CSIDL��ʶֵΪCSIDL_STARTUP

Ȼ��ʹ��CopyFile����������Ҫ�������ĳ����Ƶ���������Ŀ¼�¼��ɡ�
Ҳ����Ϊ���򴴽���ݷ�ʽ�����ѿ�ݷ�ʽ���뵽��������Ŀ¼�У�Ҳͬ�����Դﵽ������������Ч����

*/

BOOL AutoRun_Startup(char *lpszSrcFilePath, char *lpszDestFileName)
{
	BOOL bRet = FALSE;
	char szStartupPath[MAX_PATH] = {0};
	char szDestFilePath[MAX_PATH] = {0};
	// ��ȡ ��������Ŀ¼ ·��
	bRet = ::SHGetSpecialFolderPath(NULL, szStartupPath, CSIDL_STARTUP, TRUE);
	printf("szStartupPath=%s\n", szStartupPath);
	if (FALSE == bRet)
	{
		return FALSE;
	}
	// ���쿽���� Ŀ���ļ�·��
	::wsprintf(szDestFilePath, "%s\\%s", szStartupPath, lpszDestFileName);
	// �����ļ�����������Ŀ¼��
	bRet = ::CopyFile(lpszSrcFilePath, szDestFilePath, FALSE);
	if (FALSE == bRet)
	{
		return FALSE;
	}

	return TRUE;
}


int _tmain(int argc, _TCHAR* argv[])
{
	if (FALSE == AutoRun_Startup("D:\\Coding\\C\\WindowsHacker\\test.exe", "test.exe"))
	{
		printf("Startup Error!\n");
	}
	printf("Startup OK!\n");

	system("pause");
	return 0;
}

