#include "stdafx.h"
#include "lyWinService.h"
#include "windows.h"  //"winsvc.h",����ĿΪmfc��ʱ��ʹ��winsvc.h��Ϊ������ģʽ��ʱ��ʹ��windows.h

#pragma comment(lib,"Advapi32")

#define __LY_DEBUG

#ifdef __LY_DEBUG
//("File: "__FILE__", Line: %05d: "format"\n", __LINE__, ##__VA_ARGS__)
#define lys(format,...) printf("Line%3d: "format"\n", __LINE__, ##__VA_ARGS__)
#else
#define lys(format,...)
#endif



TCHAR achServiceName[] = L"LY---Just Test hahaha!";
TCHAR achServiceNameOfDisplay[] = L"LY---Service name of display,hahhaha!";



void fnTestSomeFunc()
{
	SC_HANDLE schSCManager;
	SC_HANDLE schService;
	TCHAR chBinPath[MAX_PATH]=L"\"C:\\Owntools\\testWS.exe\"";
	SERVICE_STATUS ssCur;


	lys("into the func");


	//��ȡָ��SCM��������ƹ��������ľ��,����1ָ����������ơ�ΪNULL��ȡ����SCM���������3ΪȨ�ޡ�SC_MANAGER_ALL_ACCESS����  
	schSCManager = OpenSCManager(
		NULL,                    // local computer  
		NULL,                    // ServicesActive database   
		SC_MANAGER_ALL_ACCESS);  // full access rights   

	if (NULL == schSCManager)
	{
		lys("can not open scm,error is %d",GetLastError());
		return;
	}

	lys("sch handler is 0x%x",schSCManager->unused);


	if(FALSE == EnumServicesStatusEx(schSCManager,))
	{
		lys("can not enum services,error is %d",GetLastError());
    	CloseServiceHandle(schSCManager);
		return;
	}

	//�ڴ˴�ӡ���е�ö�ٳ��ķ���Ҳ���Թ��˳��ض��Ľ��



	CloseServiceHandle(schSCManager);
	return;
}


void fnQueryService()
{
	SC_HANDLE schSCManager;
	SC_HANDLE schService;
	TCHAR chBinPath[MAX_PATH]=L"\"C:\\Owntools\\testWS.exe\"";
	SERVICE_STATUS ssCur;


	lys("into the func");


	//��ȡָ��SCM��������ƹ��������ľ��,����1ָ����������ơ�ΪNULL��ȡ����SCM���������3ΪȨ�ޡ�SC_MANAGER_ALL_ACCESS����  
	schSCManager = OpenSCManager(
		NULL,                    // local computer  
		NULL,                    // ServicesActive database   
		SC_MANAGER_ALL_ACCESS);  // full access rights   

	if (NULL == schSCManager)
	{
		lys("can not open scm,error is %d",GetLastError());
		return;
	}

	lys("sch handler is 0x%x",schSCManager->unused);

	schService = OpenService(schSCManager,achServiceName,SC_MANAGER_ALL_ACCESS );
	if (NULL == schService)
	{
		lys("can not open servcie,error is %d",GetLastError());
		CloseServiceHandle(schSCManager);
		return ;
	}

	if(FALSE == QueryServiceStatus(schService,&ssCur))
	{
		lys("query status is failed! error is %d",GetLastError());
		CloseServiceHandle(schService);
		CloseServiceHandle(schSCManager);
		return ;
	}

	lys("service status is %d",ssCur.dwCurrentState);



	CloseServiceHandle(schService);
	CloseServiceHandle(schSCManager);
	return;
}



void fnInstallService()
{
	SC_HANDLE schSCManager;
	SC_HANDLE schService;
	TCHAR chBinPath[MAX_PATH]=L"\"C:\\Owntools\\testWS.exe\"";



	lys("into the func");


	//��ȡָ��SCM��������ƹ��������ľ��,����1ָ����������ơ�ΪNULL��ȡ����SCM���������3ΪȨ�ޡ�SC_MANAGER_ALL_ACCESS����  
	schSCManager = OpenSCManager(
		NULL,                    // local computer  
		NULL,                    // ServicesActive database   
		SC_MANAGER_ALL_ACCESS);  // full access rights   

	if (NULL == schSCManager)
	{
		lys("can not open scm,error is %d",GetLastError());
		return;
	}

	lys("sch handler is 0x%x",schSCManager->unused);

	// Create the service  
	/*
	��ָ����SCM�ϴ���һ������,����1��SCM���������2��Ҫ�����ķ������ƣ�����3�Ƿ�����SCM����ʾ�����֣�����4Ȩ�ޡ�
	����5�������ͱ�ע�ý������м������񣬼�StartServiceCtrlDispatcher(ServiceTable)����SERVICE_TABLE_ENTRY ServiceTable[]��
	ע���˼�������1����SERVICE_WIN32_OWN_PROCESS������̣�����1��SERVICE_WIN32_SHARE_PROCESS������̡�
	����6Ϊ������ʽ��SERVICE_AUTO_START���������������Զ�������
	����7ֱ��SERVICE_ERROR_NORMAL
	����8��ؼ����Ƿ���Ҫִ�е�exe�ļ���ȫ·������E:\projects\Win32Service2\Release\Win32Service2.exe��������������Զ����и�exe�ļ���
	*/
	schService = CreateService(
		schSCManager,              // SCM database   
		achServiceName,                   // name of service   
		achServiceNameOfDisplay,                   // service name to display   
		SERVICE_ALL_ACCESS,        // desired access   
		SERVICE_WIN32_OWN_PROCESS, // service type   
		SERVICE_AUTO_START,      // start type   
		SERVICE_ERROR_NORMAL,      // error control type   
		chBinPath,                    // path to service's binary   
		NULL,                      // no load ordering group   
		NULL,                      // no tag identifier   
		NULL,                      // no dependencies   
		NULL,                      // LocalSystem account   
		NULL);                     // no password   

	if (NULL == schService)
	{
		lys("CreateService failed (%d)", GetLastError());
		CloseServiceHandle(schSCManager);
		return;
	}
	else 
	{
		lys("Service installed successfully");
	}

	//����SCM����ͷ�������Ҫ���ƹر�  
	//�����ں���DeleteServiceж�ط���ʱSCM����Ǹ÷���Ҫж�ص���ִ�У������д򿪸÷���ľ�����رպ��ִ��ж�ء���ȵ�������  
	CloseServiceHandle(schService);
	CloseServiceHandle(schSCManager);

	lys("exit from install windows service!");
}



