// GlobalHook_Test.cpp : ���� DLL Ӧ�ó���ĵ���������
//

#include "stdafx.h"


extern HMODULE g_hDllModule;
// �����ڴ�
//������mydata���ݶ�
#pragma data_seg("mydata")
    HHOOK g_hHook = NULL;
#pragma data_seg()
//��mydata���ݶ�����Ϊ�ɶ�����д���ɹ�������ݶ�
#pragma comment(linker, "/SECTION:mydata,RWS")


// ���ӻص�����,���ӻص������������⣬���������ͷ���ֵ�������⼸��.
//�ú�������д��dllģ����,��ָ����dllģ��ľ��.
LRESULT GetMsgProc(
	int code,//��ǰ���Ӿ��
	WPARAM wParam,
	LPARAM lParam)
{
	//��������Ϣ���ݸ���ǰ�������е���һ�����ӹ��̡� ���ӹ��̿����ڴ�������Ϣ֮ǰ��֮����ô˺����� 
	//����0���ʾ�жϹ��Ӵ��ݣ��Թ��ӽ�������.
	return ::CallNextHookEx(g_hHook, code, wParam, lParam);
}


// ����ȫ�ֹ���
BOOL SetGlobalHook()
{
	//1. WH_GETMESSAGE��������Ϣ����,ʹ����ʹ����Ϣ���еĽ��̼��ش˹���.
	//2. GetMsgProc�����ӻص�����
	//3. g_hDllModule���������ӻص�������dllģ����,���Ҫ����ȫ�ֹ���,��ò�������ָ��dllģ����.
	//4. ���ĸ�������ʾ�빳�ӹ������߳�ID,0��ʾȫ�ֹ���
	g_hHook = ::SetWindowsHookEx(WH_GETMESSAGE, (HOOKPROC)GetMsgProc, g_hDllModule, 0);
	if (NULL == g_hHook)
	{
		return FALSE;
	}
	return TRUE;
}


// ж�ع��ӣ����Ӳ���ʱҪж�ع���
BOOL UnsetGlobalHook()
{
	//��Ϊg_hHook�ǹ����ڴ���ģ��������н��̹��ã����ĳ�����̸ı������ֵ���������н��̶��ı�.
	if (g_hHook)
	{
		::UnhookWindowsHookEx(g_hHook);
	}
	return TRUE;
}