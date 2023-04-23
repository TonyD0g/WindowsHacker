#include "stdafx.h"
#include "MyTaskSchedule.h"
#include <CString>


void ShowError(char *lpszText, DWORD dwErrCode)
{
	char szErr[MAX_PATH] = {0};
	::wsprintf(szErr, "%s Error!\nError Code Is:0x%08x\n", lpszText, dwErrCode);
	::MessageBox(NULL, szErr, "ERROR", MB_OK | MB_ICONERROR);
}

/* 
��һ��
1.����ʹ��com��������Ա���Ҫ���� CoInitialize ��������ʼ��com�ӿڻ���
2.���� CoCreateInstance ������������������ ITaskService �� ���������ӵ���������ϡ�
3.���� ITaskService �����л�ȡ������ Root Task Folder ��ָ����� ITaskFolder ��
  ���ָ��ָ�������ע�������

  ����⣺��ʼ�� => �������� => �ҵ�����ĸ��ڵ� => ���ڵ��ָ��ָ�� �·���
*/
CMyTaskSchedule::CMyTaskSchedule(void)
{
	m_lpITS = NULL;
	m_lpRootFolder = NULL;
	// ��ʼ��COM
	HRESULT hr = ::CoInitialize(NULL);
	if(FAILED(hr))
	{
		ShowError("CoInitialize", hr);
	}
	// ����һ���������Task Service��ʵ��
	hr = ::CoCreateInstance(CLSID_TaskScheduler,
		NULL,
		CLSCTX_INPROC_SERVER,
		IID_ITaskService,
		(LPVOID *)(&m_lpITS));
	if(FAILED(hr))
	{
		ShowError("CoCreateInstance", hr);
	}
	// ���ӵ��������Task Service��
	hr = m_lpITS->Connect(_variant_t(), _variant_t(), _variant_t(), _variant_t());
	if(FAILED(hr))
	{
		ShowError("ITaskService::Connect", hr);
	}
	// ��ȡRoot Task Folder��ָ�룬���ָ��ָ�������ע�������
	hr = m_lpITS->GetFolder(_bstr_t("\\"), &m_lpRootFolder);
	if(FAILED(hr))
	{
		ShowError("ITaskService::GetFolder", hr);
	}
}


CMyTaskSchedule::~CMyTaskSchedule(void)
{
	if(m_lpITS)
	{
		m_lpITS->Release();
	}
	if(m_lpRootFolder)
	{
		m_lpRootFolder->Release();
	}
	// ж��COM
	::CoUninitialize();
}

// ɾ��ָ������ƻ�
BOOL CMyTaskSchedule::Delete(char *lpszTaskName)
{
	if(NULL == m_lpRootFolder)
	{
		return FALSE;
	}
	CComVariant variantTaskName(NULL);
	variantTaskName = lpszTaskName;
	HRESULT hr = m_lpRootFolder->DeleteTask(variantTaskName.bstrVal, 0);
	if(FAILED(hr))
	{
		return FALSE;
	}

	return TRUE;
}	

// ɾ��ָ������ƻ�
BOOL CMyTaskSchedule::DeleteFolder(char *lpszFolderName)
{
	if(NULL == m_lpRootFolder)
	{
		return FALSE;
	}
	CComVariant variantFolderName(NULL);
	variantFolderName = lpszFolderName;
	HRESULT hr = m_lpRootFolder->DeleteFolder(variantFolderName.bstrVal, 0);
	if(FAILED(hr))
	{
		return FALSE;
	}

	return TRUE;
}

//�ڶ��������üƻ�����Ĵ����������Զ�����Ϣ����������Ϣ�����г����·���Ͳ����ȣ�
BOOL CMyTaskSchedule::NewTask(char *lpszTaskName, char *lpszProgramPath, char *lpszParameters, char *lpszAuthor)
{
	if(NULL == m_lpRootFolder)
	{
		return FALSE;
	}
	// ���������ͬ�ļƻ�������ɾ��
	Delete(lpszTaskName);
	// �����������������������
	ITaskDefinition *pTaskDefinition = NULL;
	HRESULT hr = m_lpITS->NewTask(0, &pTaskDefinition);
	if(FAILED(hr))
	{
		ShowError("ITaskService::NewTask", hr);
		return FALSE;
	}

	/* ����ע����Ϣ */
	IRegistrationInfo *pRegInfo = NULL;
	CComVariant variantAuthor(NULL);
	variantAuthor = lpszAuthor;
	hr = pTaskDefinition->get_RegistrationInfo(&pRegInfo);
	
	if(FAILED(hr))
	{
		ShowError("pTaskDefinition::get_RegistrationInfo", hr);
		return FALSE;
	}

	// ����������Ϣ
	hr = pRegInfo->put_Author(variantAuthor.bstrVal);

	//����������Ϣ
	variantAuthor = "TEST";
	pRegInfo->get_Description(&variantAuthor.bstrVal);
	//����������Ϣ
	//char *stringBuffer = "ʹ��� Microsoft �����������״̬������������ѽ��û�ֹͣ���� Microsoft ������޷���������״̬������ζ���޷��޸����ܲ����İ�ȫ©�������ҹ���Ҳ�����޷�ʹ�á����û�� Microsoft ���ʹ�ô����������������ж�ء�";
	
	//BSTR bstr = ::SysAllocString(L"test123");// _com_util::ConvertStringToBSTR(stringBuffer);

	//pRegInfo->get_Description(&bstr);
	//
	//SysFreeString(bstr);

	pRegInfo->Release();

	/* ���õ�¼���ͺ�����Ȩ�� */
	IPrincipal *pPrincipal = NULL;
	hr = pTaskDefinition->get_Principal(&pPrincipal);
	if(FAILED(hr))
	{
		ShowError("pTaskDefinition::get_Principal", hr);
		return FALSE;
	}
	// ���õ�¼����
	hr = pPrincipal->put_LogonType(TASK_LOGON_INTERACTIVE_TOKEN);
	// ��������Ȩ��
	// ���Ȩ��
	hr = pPrincipal->put_RunLevel(TASK_RUNLEVEL_HIGHEST);  
	pPrincipal->Release();

	/* ����������Ϣ */
	ITaskSettings *pSettting = NULL;
	hr = pTaskDefinition->get_Settings(&pSettting);
	if(FAILED(hr))
	{
		ShowError("pTaskDefinition::get_Settings", hr);
		return FALSE;
	}
	// ����������Ϣ
	hr = pSettting->put_StopIfGoingOnBatteries(VARIANT_FALSE);
	hr = pSettting->put_DisallowStartIfOnBatteries(VARIANT_FALSE);
	hr = pSettting->put_AllowDemandStart(VARIANT_TRUE);
	hr = pSettting->put_StartWhenAvailable(VARIANT_FALSE);
	hr = pSettting->put_MultipleInstances(TASK_INSTANCES_PARALLEL);
	pSettting->Release();

	/* ����ִ�ж��� */
	IActionCollection *pActionCollect = NULL;
	hr = pTaskDefinition->get_Actions(&pActionCollect);
	if(FAILED(hr))
	{
		ShowError("pTaskDefinition::get_Actions", hr);
		return FALSE;
	}
	IAction *pAction = NULL;
	// ����ִ�в���
	hr = pActionCollect->Create(TASK_ACTION_EXEC, &pAction);
	pActionCollect->Release();

	/* ����ִ�г���·���Ͳ��� */
	CComVariant variantProgramPath(NULL);
	CComVariant variantParameters(NULL);
	IExecAction *pExecAction = NULL;
	hr = pAction->QueryInterface(IID_IExecAction, (PVOID *)(&pExecAction));
	if(FAILED(hr))
	{
		pAction->Release();
		ShowError("IAction::QueryInterface", hr);
		return FALSE;
	}
	pAction->Release();
	// ���ó���·���Ͳ���
	variantProgramPath = lpszProgramPath;
	variantParameters = lpszParameters;
	pExecAction->put_Path(variantProgramPath.bstrVal);
	pExecAction->put_Arguments(variantParameters.bstrVal);
	pExecAction->Release();

	/* ������������ʵ���û���½������ */
	ITriggerCollection *pTriggers = NULL;
	hr = pTaskDefinition->get_Triggers(&pTriggers);
	if (FAILED(hr))
	{
		ShowError("pTaskDefinition::get_Triggers", hr);
		return FALSE;
	}
	// ����������
	ITrigger *pTrigger = NULL;
	hr = pTriggers->Create(TASK_TRIGGER_LOGON, &pTrigger);
	if (FAILED(hr))
	{
		ShowError("ITriggerCollection::Create", hr);
		return FALSE;
	}

	/* ע������ƻ�  */
	IRegisteredTask *pRegisteredTask = NULL;
	CComVariant variantTaskName(NULL);
	variantTaskName = lpszTaskName;

	//RegisterTaskDefinition ������ https://docs.microsoft.com/zh-cn/windows/win32/taskschd/taskfolder-registertaskdefinition
	hr = m_lpRootFolder->RegisterTaskDefinition(variantTaskName.bstrVal,
		pTaskDefinition,
		TASK_CREATE_OR_UPDATE,
		//���ĸ�����Ϊ��ʲôȨ�����У����� _variant_t("system") ������systemȨ������
		_variant_t(),
		_variant_t(),
		TASK_LOGON_INTERACTIVE_TOKEN,
		_variant_t(""),
		&pRegisteredTask);
	if(FAILED(hr))
	{
		pTaskDefinition->Release();
		ShowError("ITaskFolder::RegisterTaskDefinition", hr);
		return FALSE;
	}
	pTaskDefinition->Release();
	pRegisteredTask->Release();

	return TRUE;
}

// �ж�ָ������ƻ��Ƿ����
BOOL CMyTaskSchedule::IsExist(char *lpszTaskName)
{
	if(NULL == m_lpRootFolder)
	{
		return FALSE;
	}
	HRESULT hr = S_OK;
	CComVariant variantTaskName(NULL);
	CComVariant variantEnable(NULL);
	variantTaskName = lpszTaskName;                     // ����ƻ�����
	IRegisteredTask *pRegisteredTask = NULL;
	// ��ȡ����ƻ�
	hr = m_lpRootFolder->GetTask(variantTaskName.bstrVal, &pRegisteredTask);
	if(FAILED(hr) || (NULL == pRegisteredTask))
	{
		return FALSE;
	}
	pRegisteredTask->Release();

	return TRUE;
}

// �ж�ָ������ƻ�״̬�Ƿ���Ч
BOOL CMyTaskSchedule::IsTaskValid(char *lpszTaskName)
{
	if(NULL == m_lpRootFolder)
	{
		return FALSE;
	}
	HRESULT hr = S_OK;
	CComVariant variantTaskName(NULL);
	CComVariant variantEnable(NULL);
	variantTaskName = lpszTaskName;                     // ����ƻ�����
	IRegisteredTask *pRegisteredTask = NULL;
	// ��ȡ����ƻ�
	hr = m_lpRootFolder->GetTask(variantTaskName.bstrVal, &pRegisteredTask);
	if(FAILED(hr) || (NULL == pRegisteredTask))
	{
		return FALSE;
	}
	// ��ȡ����״̬
	TASK_STATE taskState;
	hr = pRegisteredTask->get_State(&taskState);
	if(FAILED(hr))
	{
		pRegisteredTask->Release();
		return FALSE;
	}
	pRegisteredTask->Release();
	// ��Ч
	if(TASK_STATE_DISABLED == taskState)
	{
		return FALSE;
	}

	return TRUE;
}

// ����ָ������ƻ�
BOOL CMyTaskSchedule::Run(char *lpszTaskName, char *lpszParam)
{
	if(NULL == m_lpRootFolder)
	{
		return FALSE;
	}
	HRESULT hr = S_OK;
	CComVariant variantTaskName(NULL);
	CComVariant variantParameters(NULL);
	variantTaskName = lpszTaskName;
	variantParameters = lpszParam;

	// ��ȡ����ƻ�
	IRegisteredTask *pRegisteredTask = NULL;
	hr = m_lpRootFolder->GetTask(variantTaskName.bstrVal, &pRegisteredTask);
	if(FAILED(hr) || (NULL == pRegisteredTask))
	{
		return FALSE;
	}
	// ����
	hr = pRegisteredTask->Run(variantParameters, NULL);
	if(FAILED(hr))
	{
		pRegisteredTask->Release();
		return FALSE;
	}
	pRegisteredTask->Release();

	return TRUE;
}

// �ж�ָ������ƻ��Ƿ�����
BOOL CMyTaskSchedule::IsEnable(char *lpszTaskName)
{
	if(NULL == m_lpRootFolder)
	{
		return FALSE;
	}
	HRESULT hr = S_OK;
	CComVariant variantTaskName(NULL);
	CComVariant variantEnable(NULL);
	variantTaskName = lpszTaskName;                     // ����ƻ�����
	IRegisteredTask *pRegisteredTask = NULL;
	// ��ȡ����ƻ�
	hr = m_lpRootFolder->GetTask(variantTaskName.bstrVal, &pRegisteredTask);
	if(FAILED(hr) || (NULL == pRegisteredTask))
	{
		return FALSE;
	}
	// ��ȡ�Ƿ��Ѿ�����
	pRegisteredTask->get_Enabled(&variantEnable.boolVal);
	pRegisteredTask->Release();
	if(ATL_VARIANT_FALSE == variantEnable.boolVal)
	{
		return FALSE;
	}

	return TRUE;
}

// ����ָ������ƻ��Ƿ��������ǽ���
BOOL CMyTaskSchedule::SetEnable(char *lpszTaskName, BOOL bEnable)
{
	if(NULL == m_lpRootFolder)
	{
		return FALSE;
	}
	HRESULT hr = S_OK;
	CComVariant variantTaskName(NULL);
	CComVariant variantEnable(NULL);
	variantTaskName = lpszTaskName;                     // ����ƻ�����
	variantEnable = bEnable;                            // �Ƿ�����
	IRegisteredTask *pRegisteredTask = NULL;
	// ��ȡ����ƻ�
	hr = m_lpRootFolder->GetTask(variantTaskName.bstrVal, &pRegisteredTask);
	if(FAILED(hr) || (NULL == pRegisteredTask))
	{
		return FALSE;
	}
	// �����Ƿ�����
	pRegisteredTask->put_Enabled(variantEnable.boolVal);
	pRegisteredTask->Release();

	return TRUE;
}

