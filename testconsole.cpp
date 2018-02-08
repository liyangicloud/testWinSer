// testconsole.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include <Windows.h>
#pragma comment(lib,"Advapi32")

TCHAR achServiceName[] = L"Just Test hahaha!";
TCHAR achServiceNameOfDisplay[] = L"Service name of display,hahhaha!";

int _tmain(int argc, _TCHAR* argv[])
{
	return 0;
}


void InstallService()  
{  
    SC_HANDLE schSCManager;  
    SC_HANDLE schService;  
    TCHAR ch[300] = {0};  
  
    //第一参数NULL，GetModuleFileName取出当前进程的exe文件全路径，如E:\projects\Win32Service2\Release\Win32Service2.exe  
    if(!GetModuleFileName(NULL, ch, 300))  
    {  
        printf("Cannot install service (%d)\n", GetLastError());  
        return;  
    }  
  
    //获取指定SCM（服务控制管理器）的句柄,参数1指定计算机名称、为NULL则取本机SCM句柄，参数3为权限、SC_MANAGER_ALL_ACCESS即可  
    schSCManager = OpenSCManager(   
        NULL,                    // local computer  
        NULL,                    // ServicesActive database   
        SC_MANAGER_ALL_ACCESS);  // full access rights   
  
    if (NULL == schSCManager)   
    {  
        printf("OpenSCManager failed (%d)\n", GetLastError());  
        return;  
    }  
  
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
        achServiceName,                   // name of service   
        achServiceNameOfDisplay,                   // service name to display   
        SERVICE_ALL_ACCESS,        // desired access   
        SERVICE_WIN32_OWN_PROCESS, // service type   
        SERVICE_AUTO_START,      // start type   
        SERVICE_ERROR_NORMAL,      // error control type   
        ch,                    // path to service's binary   
        NULL,                      // no load ordering group   
        NULL,                      // no tag identifier   
        NULL,                      // no dependencies   
        NULL,                      // LocalSystem account   
        NULL);                     // no password   
  
    if (schService == NULL)   
    {  
        printf("CreateService failed (%d)\n", GetLastError());   
        CloseServiceHandle(schSCManager);  
        return;  
    }  
    else printf("Service installed successfully\n");   
  
    //所有SCM句柄和服务句柄都要妥善关闭  
    //否则在后续DeleteService卸载服务时SCM仅标记该服务要卸载但不执行，等所有打开该服务的句柄均关闭后才执行卸载、或等电脑重启  
    CloseServiceHandle(schService);   
    CloseServiceHandle(schSCManager);  
}  
