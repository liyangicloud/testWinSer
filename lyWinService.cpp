#include "stdafx.h"
#include "lyWinService.h"
#include "windows.h"  //"winsvc.h",����ĿΪmfc��ʱ��ʹ��winsvc.h��Ϊ������ģʽ��ʱ��ʹ��windows.h
#include "atlstr.h"

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
	//Ҫ���ҵķ��������а������ض��ַ���
	LPTSTR lpDestServiceName = L"Just";

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

	DWORD dwSizeOfBuffer = 0;
	DWORD dwServicesReturned = 0;
	DWORD dwResumeHandle = 0;
	BYTE *lpBuffer;
	ENUM_SERVICE_STATUS_PROCESS * lpServicesInfo;
	if(TRUE ==EnumServicesStatusEx(schSCManager,SC_ENUM_PROCESS_INFO,SERVICE_WIN32,SERVICE_STATE_ALL,NULL,\
		0,&dwSizeOfBuffer,&dwServicesReturned,&dwResumeHandle,NULL))
	{
		lys("can not get the size of service info buffer ,error is %d",GetLastError());
    	CloseServiceHandle(schSCManager);
		return;
	}

	lys("pcbBytesNeeded=%d,lpServicesReturned=%d,lpResumeHandle=%d",dwSizeOfBuffer,dwServicesReturned,dwResumeHandle);

	if(ERROR_MORE_DATA != GetLastError())
	{
		lys("Getlasterror wrong!");
    	CloseServiceHandle(schSCManager);
		return;
	}

	lpBuffer = (BYTE *)malloc(dwSizeOfBuffer);

	if (NULL == lpBuffer)
	{
		lys("malloc failed!");
		CloseServiceHandle(schSCManager);
		return;
	}

	if(FALSE == EnumServicesStatusEx(schSCManager,SC_ENUM_PROCESS_INFO,\
		SERVICE_WIN32,SERVICE_STATE_ALL,lpBuffer,dwSizeOfBuffer,\
		&dwSizeOfBuffer,&dwServicesReturned,&dwResumeHandle,NULL))
	{
		lys("can not enum services,error is %d",GetLastError());
		free(lpBuffer);
		lpBuffer = NULL;
    	CloseServiceHandle(schSCManager);
		return;
	}

	lys("pcbBytesNeeded=%d,lpServicesReturned=%d,lpResumeHandle=%d",dwSizeOfBuffer,dwServicesReturned,dwResumeHandle);

	//�ڴ˴�ӡ���е�ö�ٳ��ķ���Ҳ���Թ��˳��ض��Ľ��

	lpServicesInfo = (ENUM_SERVICE_STATUS_PROCESS *)lpBuffer;
	for(DWORD i=0;i < dwServicesReturned;i++)
	{
		
		if(NULL != StrStr((lpServicesInfo+i)->lpServiceName,lpDestServiceName))
		{
			lys("    %d",i);
			wprintf(L"    service name is     :%s\n",(lpServicesInfo+i)->lpServiceName);
			wprintf(L"service display info is :%s\n",(lpServicesInfo+i)->lpDisplayName);
			lys("current state is %d",(lpServicesInfo+i)->ServiceStatusProcess.dwCurrentState);
			////�Ѿ��ҵ��ض��ĳ���Ȼ���жϷ����״̬�������Ϊֹͣ״̬�����ֹͣ������Ȼ��ɾ��
			schService = OpenService(schSCManager,(lpServicesInfo+i)->lpServiceName,SC_MANAGER_ALL_ACCESS );
			if (NULL == schService)
			{
				lys("can not open servcie,error is %d",GetLastError());
				continue ;
			}

			if(FALSE == QueryServiceStatus(schService,&ssCur))
			{
				lys("query status is failed! error is %d",GetLastError());
				CloseServiceHandle(schService);
				continue ;
			}

			if(SERVICE_RUNNING == ssCur.dwCurrentState)
			{//��������״̬����ֹͣ����ô����ͣ���÷���
				SERVICE_CONTROL_STATUS_REASON_PARAMS scsp;
				ControlServiceEx(schService,SERVICE_CONTROL_STOP,SERVICE_CONTROL_STATUS_REASON_INFO,&scsp);
			}
			DeleteService(schService);
			CloseServiceHandle(schService);
			lys("success in deletling the service.");
		}
	}


	free(lpBuffer);
	lpBuffer = NULL;
	CloseServiceHandle(schSCManager);
	return;
}

void fnFindSomeServiceWithCommitString()
{
	//Ҫ���ҵķ��������а������ض��ַ���
	LPTSTR lpDestServiceName = L"Just";

	SC_HANDLE schSCManager;



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

	DWORD dwSizeOfBuffer = 0;
	DWORD dwServicesReturned = 0;
	DWORD dwResumeHandle = 0;
	BYTE *lpBuffer;
	ENUM_SERVICE_STATUS_PROCESS * lpServicesInfo;
	if(TRUE ==EnumServicesStatusEx(schSCManager,SC_ENUM_PROCESS_INFO,SERVICE_WIN32,SERVICE_STATE_ALL,NULL,\
		0,&dwSizeOfBuffer,&dwServicesReturned,&dwResumeHandle,NULL))
	{
		lys("can not get the size of service info buffer ,error is %d",GetLastError());
    	CloseServiceHandle(schSCManager);
		return;
	}

	lys("pcbBytesNeeded=%d,lpServicesReturned=%d,lpResumeHandle=%d",dwSizeOfBuffer,dwServicesReturned,dwResumeHandle);

	if(ERROR_MORE_DATA != GetLastError())
	{
		lys("Getlasterror wrong!");
    	CloseServiceHandle(schSCManager);
		return;
	}

	lpBuffer = (BYTE *)malloc(dwSizeOfBuffer);

	if (NULL == lpBuffer)
	{
		lys("malloc failed!");
		CloseServiceHandle(schSCManager);
		return;
	}

	if(FALSE == EnumServicesStatusEx(schSCManager,SC_ENUM_PROCESS_INFO,\
		SERVICE_WIN32,SERVICE_STATE_ALL,lpBuffer,dwSizeOfBuffer,\
		&dwSizeOfBuffer,&dwServicesReturned,&dwResumeHandle,NULL))
	{
		lys("can not enum services,error is %d",GetLastError());
		free(lpBuffer);
		lpBuffer = NULL;
    	CloseServiceHandle(schSCManager);
		return;
	}

	lys("pcbBytesNeeded=%d,lpServicesReturned=%d,lpResumeHandle=%d",dwSizeOfBuffer,dwServicesReturned,dwResumeHandle);

	//�ڴ˴�ӡ���е�ö�ٳ��ķ���Ҳ���Թ��˳��ض��Ľ��

	lpServicesInfo = (ENUM_SERVICE_STATUS_PROCESS *)lpBuffer;
	for(DWORD i=0;i < dwServicesReturned;i++)
	{
		
		if(NULL != StrStr((lpServicesInfo+i)->lpServiceName,lpDestServiceName))
		{
			lys("    %d",i);
			wprintf(L"    service name is     :%s\n",(lpServicesInfo+i)->lpServiceName);
			wprintf(L"service display info is :%s\n",(lpServicesInfo+i)->lpDisplayName);
			lys("current state is %d",(lpServicesInfo+i)->ServiceStatusProcess.dwCurrentState);
		}
		//Sleep(500);
	}


	free(lpBuffer);
	lpBuffer = NULL;
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

	//��ȫ�ֱ�����������һ����������֣����������Ȼ���ѯ����״̬
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



void fnInstallService(TCHAR *lpBinPath,TCHAR *lpServiceName,TCHAR *lpServiceDisplayName)
{
	SC_HANDLE schSCManager;
	SC_HANDLE schService;
	TCHAR chBinPath[MAX_PATH]=L"\"C:\\Owntools\\testWS.exe\"";


	//��ѯ·�����Ƿ��пո�����пո��������ض���ʽ
	if(NULL == StrChr(lpBinPath,L' '))
	{
		wsprintf(chBinPath,L"%s",lpBinPath);
	}
	else
	{
		wsprintf(chBinPath,L"\"%s\"",lpBinPath);
	}
	
	lys("into the func,bin is :%s",chBinPath);


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
		lpServiceName,                   // name of service   
		lpServiceDisplayName,                   // service name to display   
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



