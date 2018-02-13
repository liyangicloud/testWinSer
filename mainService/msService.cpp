// msService.cpp : 定义控制台应用程序的入口点。
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

    // 启动服务的控制分派机线程
    StartServiceCtrlDispatcher(ServiceTable); 


	return 0;
}

/**********************************************************************
服务程序
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
   //注册成功后，就可以进行服务线程的初始化工作了。
   ssTemp.dwServiceType = SERVICE_WIN32_OWN_PROCESS; 
   ssTemp.dwCurrentState = SERVICE_START_PENDING; 
   ssTemp.dwControlsAccepted   =  0;//SERVICE_ACCEPT_STOP | SERVICE_ACCEPT_SHUTDOWN;
   ssTemp.dwWin32ExitCode = NO_ERROR ; 
   ssTemp.dwServiceSpecificExitCode = 0; 
   ssTemp.dwCheckPoint = 0; 
   ssTemp.dwWaitHint = 2000; 
   //初始化开始的时候，设置服务的状态为pending
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

   //初始化结束
   /*************************************/
 
   /***********************************************/
   //下面进入服务的工作内容。
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