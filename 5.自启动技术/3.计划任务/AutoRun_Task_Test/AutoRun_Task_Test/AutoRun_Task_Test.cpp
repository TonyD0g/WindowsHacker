// AutoRun_Task_Test.cpp : �������̨Ӧ�ó������ڵ㡣
//

#include "stdafx.h"
#include "MyTaskSchedule.h"


int _tmain(int argc, _TCHAR* argv[])
{
	CMyTaskSchedule task;
	BOOL bRet = FALSE;

	// ���� ����ƻ�
	bRet = task.NewTask("Test520", "D:\\Coding\\C\\WindowsHackerBackup\\test.exe", "", "Microsoft");
	if (FALSE == bRet)
	{
		printf("Create Task Schedule Error!\n");
	}

	// ��ͣ
	printf("Create Task Schedule OK!\n");
	system("pause");

	// ж�� ����ƻ�
	bRet = task.Delete("520");
	if (FALSE == bRet)
	{
		printf("Delete Task Schedule Error!\n");
	}

	printf("Delete Task Schedule OK!\n");
	system("pause");
	return 0;
}

