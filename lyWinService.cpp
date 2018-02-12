#include "stdafx.h"
#include "lyWinService.h"
#include "windows.h"  //"winsvc.h",当项目为mfc的时候，使用winsvc.h，为命令行模式的时候使用windows.h
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
	//要查找的服务名称中包含的特定字符串
	LPTSTR lpDestServiceName = L"Just";

	SC_HANDLE schSCManager;
	SC_HANDLE schService;
	TCHAR chBinPath[MAX_PATH]=L"\"C:\\Owntools\\testWS.exe\"";
	SERVICE_STATUS ssCur;



	lys("into the func");


	//获取指定SCM（服务控制管理器）的句柄,参数1指定计算机名称、为NULL则取本机SCM句柄，参数3为权限、SC_MANAGER_ALL_ACCESS即可  
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

	//在此打印所有的枚举出的服务，也可以过滤出特定的结果

	lpServicesInfo = (ENUM_SERVICE_STATUS_PROCESS *)lpBuffer;
	for(DWORD i=0;i < dwServicesReturned;i++)
	{
		
		if(NULL != StrStr((lpServicesInfo+i)->lpServiceName,lpDestServiceName))
		{
			lys("    %d",i);
			wprintf(L"    service name is     :%s\n",(lpServicesInfo+i)->lpServiceName);
			wprintf(L"service display info is :%s\n",(lpServicesInfo+i)->lpDisplayName);
			lys("current state is %d",(lpServicesInfo+i)->ServiceStatusProcess.dwCurrentState);
			////已经找到特定的程序，然后判断服务的状态，如果不为停止状态则进行停止操作，然后删除
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
			{//如果服务的状态不是停止，那么必须停掉该服务
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
	//要查找的服务名称中包含的特定字符串
	LPTSTR lpDestServiceName = L"Just";

	SC_HANDLE schSCManager;



	lys("into the func");


	//获取指定SCM（服务控制管理器）的句柄,参数1指定计算机名称、为NULL则取本机SCM句柄，参数3为权限、SC_MANAGER_ALL_ACCESS即可  
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

	//在此打印所有的枚举出的服务，也可以过滤出特定的结果

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


	//获取指定SCM（服务控制管理器）的句柄,参数1指定计算机名称、为NULL则取本机SCM句柄，参数3为权限、SC_MANAGER_ALL_ACCESS即可  
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

	//在全局变量处定义了一个服务的名字，打开这个服务然后查询他的状态
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


	//查询路径中是否有空格，如果有空格必须符合特定格式
	if(NULL == StrChr(lpBinPath,L' '))
	{
		wsprintf(chBinPath,L"%s",lpBinPath);
	}
	else
	{
		wsprintf(chBinPath,L"\"%s\"",lpBinPath);
	}
	
	lys("into the func,bin is :%s",chBinPath);


	//获取指定SCM（服务控制管理器）的句柄,参数1指定计算机名称、为NULL则取本机SCM句柄，参数3为权限、SC_MANAGER_ALL_ACCESS即可  
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
	于指定的SCM上创建一个服务,参数1即SCM句柄，参数2是要创建的服务名称，参数3是服务在SCM中显示的名字，参数4权限。
	参数5服务类型标注该进程中有几个服务，即StartServiceCtrlDispatcher(ServiceTable)数组SERVICE_TABLE_ENTRY ServiceTable[]中
	注册了几个服务，1个则SERVICE_WIN32_OWN_PROCESS独享进程，多于1个SERVICE_WIN32_SHARE_PROCESS共享进程。
	参数6为启动方式，SERVICE_AUTO_START则电脑启动后服务自动开启。
	参数7直接SERVICE_ERROR_NORMAL
	参数8最关键就是服务要执行的exe文件的全路径，如E:\projects\Win32Service2\Release\Win32Service2.exe，服务启动后会自动运行该exe文件。
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

	//所有SCM句柄和服务句柄都要妥善关闭  
	//否则在后续DeleteService卸载服务时SCM仅标记该服务要卸载但不执行，等所有打开该服务的句柄均关闭后才执行卸载、或等电脑重启  
	CloseServiceHandle(schService);
	CloseServiceHandle(schSCManager);

	lys("exit from install windows service!");
}



