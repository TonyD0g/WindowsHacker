// DisguiseProcess_Test.cpp : �������̨Ӧ�ó������ڵ㡣
//

#include "stdafx.h"
#include "DisguiseProcess.h"

//������ test.exe ��Ȼ���ý��̲鿴���鿴test.exe�Ľ���PID������棬Ȼ�����б���Ŀ��������
int _tmain(int argc, _TCHAR* argv[])
{
	if (FALSE == DisguiseProcess(8048, L"C:\\Windows\\explorer.exe", L"explorer.exe"))
	{
		printf("Dsisguise Process Error.\n");
	}
	printf("Dsisguise Process OK.\n");

	system("pause");
	return 0;
}

