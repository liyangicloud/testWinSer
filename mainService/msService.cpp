// msService.cpp : �������̨Ӧ�ó������ڵ㡣
//

#include "stdafx.h"


#include <Windows.h>
#include <stdio.h>


SERVICE_STATUS g_ssCur; 
SERVICE_STATUS_HANDLE g_hStatus; 
TCHAR g_cServiceNameForServiceMain[] = L"QQ space protection";

TCHAR g_cQQServiceUserDefinedData[] = L"this can be a struct data using malloc function ";

void WINAPI ServiceMain(DWORD argc, LPTSTR** argv); 
void WINAPI ControlHandler(DWORD request); 
DWORD WINAPI fnControlHandler(DWORD  dwControl,DWORD  dwEventType,LPVOID lpEventData,LPVOID lpContext);
int InitService();


int WriteToLog(char* str)
{
    FILE* log;
    log = fopen("c:\\msconfigs.txt", "a+");
    if (log == NULL)
		return -1;
    fprintf(log, "%s/n", str);
    fclose(log);
    return 0;
}




int _tmain(int argc, _TCHAR* argv[])
{

	SERVICE_TABLE_ENTRY ServiceTable[2];
    ServiceTable[0].lpServiceName = g_cServiceNameForServiceMain;
    ServiceTable[0].lpServiceProc = (LPSERVICE_MAIN_FUNCTION)ServiceMain;
    
    ServiceTable[1].lpServiceName = NULL;
    ServiceTable[1].lpServiceProc = NULL;

    // ��������Ŀ��Ʒ��ɻ��߳�
    StartServiceCtrlDispatcher(ServiceTable); 


	return 0;
}

/**********************************************************************
�������
**********************************************************************/
void WINAPI ServiceMain(DWORD argc, LPTSTR** argv)
{ 
   int error; 
   SERVICE_STATUS ssTemp;
   SERVICE_STATUS_HANDLE hStatus;
 
   hStatus = RegisterServiceCtrlHandlerEx(g_cServiceNameForServiceMain, 
      (LPHANDLER_FUNCTION_EX)fnControlHandler,g_cQQServiceUserDefinedData); 
   if (hStatus == (SERVICE_STATUS_HANDLE)0) 
   { 
      // Registering Control Handler failed
      return; 
   }  
   /***********************************************/
   //ע��ɹ��󣬾Ϳ��Խ��з����̵߳ĳ�ʼ�������ˡ�
   ssTemp.dwServiceType = SERVICE_WIN32_OWN_PROCESS; 
   ssTemp.dwCurrentState = SERVICE_START_PENDING; 
   ssTemp.dwControlsAccepted   =  0;//SERVICE_ACCEPT_STOP | SERVICE_ACCEPT_SHUTDOWN;
   ssTemp.dwWin32ExitCode = NO_ERROR ; 
   ssTemp.dwServiceSpecificExitCode = 0; 
   ssTemp.dwCheckPoint = 0; 
   ssTemp.dwWaitHint = 2000; 
   //��ʼ����ʼ��ʱ�����÷����״̬Ϊpending
   SetServiceStatus(hStatus, &ssTemp);


 
   error = InitService(); 

   if (error) 
   {
      // Initialization failed
      ssTemp.dwCurrentState = SERVICE_STOPPED; 
      ssTemp.dwWin32ExitCode = -1; 
      SetServiceStatus(hStatus, &ssTemp); 
      return; 
   } 
   // We report the running status to SCM. 
   ssTemp.dwCurrentState = SERVICE_RUNNING; 
   ssTemp.dwControlsAccepted   =  SERVICE_ACCEPT_STOP | SERVICE_ACCEPT_SHUTDOWN;
   ssTemp.dwWaitHint = 0;
   SetServiceStatus (hStatus, &ssTemp);

   //��ʼ������
   /*************************************/
 
   /***********************************************/
   //����������Ĺ������ݡ�
   MEMORYSTATUS memory;
   // The worker loop of a service
   while (ssTemp.dwCurrentState == SERVICE_RUNNING)
   {
      char buffer[16];
      GlobalMemoryStatus(&memory);
      sprintf(buffer, "%d\n", memory.dwAvailPhys);
      int result = WriteToLog(buffer);
      if (result)
      {
         ssTemp.dwCurrentState = SERVICE_STOPPED; 
         ssTemp.dwWin32ExitCode  = -1; 
         SetServiceStatus(hStatus, &ssTemp);
         return;
      }
      Sleep(5000);
   }
   return; 
}



int InitService() 
{ 
    int result;
    result = WriteToLog("Monitoring started.");
    return(result); 
}



DWORD WINAPI fnControlHandler(DWORD  dwControl,DWORD  dwEventType,LPVOID lpEventData,LPVOID lpContext)
{


	return 0;
}