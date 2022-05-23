// CreateMutex_Test.cpp : �������̨Ӧ�ó������ڵ㡣
//

//stdafx.h��C++���𵽵�������ͷ�ļ�Ԥ����,�Լӿ��Ժ�ı����ٶ�
#include "stdafx.h" 
#include <Windows.h>


// �ж��Ƿ��ظ�����
BOOL IsAlreadyRun()
{
	//HANDLE:�������
	HANDLE hMutex = NULL;
	//CreateMutex��һ��������������������ҳ���ǰϵͳ�Ƿ��Ѿ�����ָ�����̵�ʵ�������û���򴴽�һ�������塣
	hMutex = ::CreateMutex(NULL, FALSE, "TEST");
	if (hMutex)
	{
		//����ʧ���򷵻�ERROR_ALREADY_EXISTS
		if (ERROR_ALREADY_EXISTS == ::GetLastError())
		{
			return TRUE;
		}
	}
	return FALSE;
}

//_tmain�൱��main����
int _tmain(int argc, _TCHAR* argv[])
{
	// �ж��Ƿ��ظ�����
	if (IsAlreadyRun())
	{
		printf("Already Run!!!!\n");
	}
	else
	{
		printf("NOT Already Run!\n");
	}

	system("pause");
	return 0;
}

