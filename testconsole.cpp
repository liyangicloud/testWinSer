// testconsole.cpp : �������̨Ӧ�ó������ڵ㡣
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
  
    //��һ����NULL��GetModuleFileNameȡ����ǰ���̵�exe�ļ�ȫ·������E:\projects\Win32Service2\Release\Win32Service2.exe  
    if(!GetModuleFileName(NULL, ch, 300))  
    {  
        printf("Cannot install service (%d)\n", GetLastError());  
        return;  
    }  
  
    //��ȡָ��SCM��������ƹ��������ľ��,����1ָ����������ơ�ΪNULL��ȡ����SCM���������3ΪȨ�ޡ�SC_MANAGER_ALL_ACCESS����  
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
  
    //����SCM����ͷ�������Ҫ���ƹر�  
    //�����ں���DeleteServiceж�ط���ʱSCM����Ǹ÷���Ҫж�ص���ִ�У������д򿪸÷���ľ�����رպ��ִ��ж�ء���ȵ�������  
    CloseServiceHandle(schService);   
    CloseServiceHandle(schSCManager);  
}  
