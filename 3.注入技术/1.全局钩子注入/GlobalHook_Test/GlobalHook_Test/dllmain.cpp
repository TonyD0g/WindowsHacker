// dllmain.cpp : ���� DLL Ӧ�ó������ڵ㡣
#include "stdafx.h"

HMODULE g_hDllModule = NULL;

//�൱��main��������dll�ļ������
//��׼д������ôд��
BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
					 )
{
	//����ul_reason_for_callָ���˱����õ�ԭ��
	switch (ul_reason_for_call)
	{
	//Ϊÿ�����̳�ʼ��һ��
	case DLL_PROCESS_ATTACH:
	{
		//::MessageBox(NULL, "This Is From Dll!\nInject Success!", "OK", MB_OK);
		
		//�������Ŀ¼��
		g_hDllModule = hModule;
		break;
	}
	case DLL_THREAD_ATTACH:
	case DLL_THREAD_DETACH:
	case DLL_PROCESS_DETACH:
		break;
	}
	return TRUE;
	//����GlobalHook_Test.cpp�ļ�ִ��
}

