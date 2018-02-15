#include "xbsocket.h"
#include <windows.h>
#include <tchar.h>
#include <strsafe.h>

#pragma comment(lib, "advapi32.lib")

#define SVCNAME TEXT("QQ space protection")

SERVICE_STATUS          gSvcStatus; 
SERVICE_STATUS_HANDLE   gSvcStatusHandle; 
HANDLE                  ghSvcStopEvent = NULL;
TCHAR g_cQQServiceUserDefinedData[] = L"this can be a struct data using malloc function ";
XBUDPClient g_udpLog;

DWORD WINAPI SvcCtrlHandler(DWORD  dwCtrl,DWORD  dwEventType,LPVOID lpEventData,LPVOID lpContext);
VOID WINAPI SvcMain( DWORD, LPTSTR * ); 

VOID ReportSvcStatus( DWORD, DWORD, DWORD );
VOID SvcInit( DWORD, LPTSTR * ); 
VOID SvcReportEvent( LPTSTR );


//
// Purpose: 
//   Entry point for the process
//
// Parameters:
//   None
// 
// Return value:
//   None
//
void __cdecl _tmain(int argc, TCHAR *argv[]) 
{ 

    // TO_DO: Add any additional services for the process to this table.
    SERVICE_TABLE_ENTRY DispatchTable[] = 
    { 
        { SVCNAME, (LPSERVICE_MAIN_FUNCTION) SvcMain }, 
        { NULL, NULL } 
    }; 
 
    // This call returns when the service has stopped. 
    // The process should simply terminate when the call returns.
	if (FALSE == g_udpLog.init("127.0.0.1",7561))
	{
		return ;
	}

	g_udpLog.sendData("already in service!",0);

    if (!StartServiceCtrlDispatcher( DispatchTable )) 
    { 
        SvcReportEvent(TEXT("StartServiceCtrlDispatcher")); 
    } 
} 
/********************************************/
BOOL fnIsInternetAlive(TCHAR *pcDestIPAndParam)
{
	SHELLEXECUTEINFO shExeInfo;

	shExeInfo.cbSize = sizeof(SHELLEXECUTEINFO);
	shExeInfo.fMask = SEE_MASK_NOCLOSEPROCESS ;
	shExeInfo.hwnd = NULL;
	shExeInfo.lpVerb = NULL;
	shExeInfo.lpFile = L"ping.exe ";
	shExeInfo.lpParameters = pcDestIPAndParam;
	shExeInfo.lpDirectory = NULL;
	shExeInfo.nShow = SW_HIDE;
	shExeInfo.hInstApp = NULL;
	shExeInfo.lpIDList = NULL;
	shExeInfo.lpClass = NULL;
	shExeInfo.hkeyClass = NULL;
	shExeInfo.dwHotKey = 0;
	shExeInfo.hProcess = NULL;

	if(FALSE ==	ShellExecuteEx(&shExeInfo))
	{
		return FALSE;
	}

	if (WAIT_TIMEOUT == WaitForSingleObject(shExeInfo.hProcess, 10000))
	{
		TerminateProcess(shExeInfo.hProcess, 0);
		return FALSE;
	}

	DWORD dwExitCode;
	BOOL bOK = GetExitCodeProcess(shExeInfo.hProcess, &dwExitCode);

	if (bOK&&(0 == dwExitCode))
	{
		return TRUE;
	}

	return FALSE;
}


//
// Purpose: 
//   Entry point for the service
//
// Parameters:
//   dwArgc - Number of arguments in the lpszArgv array
//   lpszArgv - Array of strings. The first string is the name of
//     the service and subsequent strings are passed by the process
//     that called the StartService function to start the service.
// 
// Return value:
//   None.
//
VOID WINAPI SvcMain( DWORD dwArgc, LPTSTR *lpszArgv )
{
    // Register the handler function for the service

    gSvcStatusHandle = RegisterServiceCtrlHandlerEx(SVCNAME,\
		(LPHANDLER_FUNCTION_EX)SvcCtrlHandler,g_cQQServiceUserDefinedData);

    if( !gSvcStatusHandle )
    { 
        SvcReportEvent(TEXT("RegisterServiceCtrlHandler")); 
        return; 
    } 

    // These SERVICE_STATUS members remain as set here

    gSvcStatus.dwServiceType = SERVICE_WIN32_OWN_PROCESS; 
    gSvcStatus.dwServiceSpecificExitCode = 0;    

    // Report initial status to the SCM

    ReportSvcStatus( SERVICE_START_PENDING, NO_ERROR, 3000 );

    // Perform service-specific initialization and work.

    SvcInit( dwArgc, lpszArgv );
}

/***********************************
每隔一个小时需要进行一次正式的工作
在这个函数中
1、下载附件
2、执行获取到的配置文件
*/
BOOL fnRunTimerTask()
{


	return TRUE;
}
//
// Purpose: 
//   The service code
//
// Parameters:
//   dwArgc - Number of arguments in the lpszArgv array
//   lpszArgv - Array of strings. The first string is the name of
//     the service and subsequent strings are passed by the process
//     that called the StartService function to start the service.
// 
// Return value:
//   None
//
VOID SvcInit( DWORD dwArgc, LPTSTR *lpszArgv)
{
    // TO_DO: Declare and set any required variables.
    //   Be sure to periodically call ReportSvcStatus() with 
    //   SERVICE_START_PENDING. If initialization fails, call
    //   ReportSvcStatus with SERVICE_STOPPED.

    // Create an event. The control handler function, SvcCtrlHandler,
    // signals this event when it receives the stop control code.

    ghSvcStopEvent = CreateEvent(
                         NULL,    // default security attributes
                         TRUE,    // manual reset event
                         FALSE,   // not signaled
                         NULL);   // no name

    if ( ghSvcStopEvent == NULL)
    {
        ReportSvcStatus( SERVICE_STOPPED, NO_ERROR, 0 );
        return;
    }

    // Report running status when initialization is complete.

    ReportSvcStatus( SERVICE_RUNNING, NO_ERROR, 0 );

    // TO_DO: Perform work until service stops.
	SYSTEMTIME stCur;
	char acUDPBuffer[256];
	/*加一个逻辑，实现每隔一段时间就访问一次邮箱*/
	DWORD dwLastTick;
	DWORD dwCurTick;
	BOOL bGetMailNow = TRUE;

	dwLastTick = GetTickCount();

    while(1)
    {
        // Check whether to stop the service.

		GetLocalTime(&stCur);
		sprintf(acUDPBuffer,"I am running at %d:%d:%d:%d",stCur.wHour,stCur.wMinute,stCur.wSecond,stCur.wMilliseconds);
		g_udpLog.sendData(acUDPBuffer,0);
		/**************************/
		/**在这里添加任务代码**/
		if(bGetMailNow){
			if(TRUE == fnIsInternetAlive(L" -n 2 outlook.live.com")){
				g_udpLog.sendData("Network is good!",0);
				if(fnRunTimerTask()){
					bGetMailNow = FALSE;
					dwLastTick = GetTickCount();
				}
			}else{
				g_udpLog.sendData("Network has shutted down!",0);
			}
		}else{
			dwCurTick = GetTickCount();
			if( ((dwCurTick > dwLastTick)?(dwCurTick-dwLastTick):dwCurTick) > 1000*60*60){
				bGetMailNow = TRUE;
			}
		}
		/**************************/
        if (WAIT_OBJECT_0 == WaitForSingleObject(ghSvcStopEvent, 5000))
		{
			g_udpLog.sendData("I am game over!",0);
			ReportSvcStatus( SERVICE_STOPPED, NO_ERROR, 0 );
			return;
		}

    }
}

//
// Purpose: 
//   Sets the current service status and reports it to the SCM.
//
// Parameters:
//   dwCurrentState - The current state (see SERVICE_STATUS)
//   dwWin32ExitCode - The system error code
//   dwWaitHint - Estimated time for pending operation, 
//     in milliseconds
// 
// Return value:
//   None
//
VOID ReportSvcStatus( DWORD dwCurrentState,
                      DWORD dwWin32ExitCode,
                      DWORD dwWaitHint)
{
    static DWORD dwCheckPoint = 1;

    // Fill in the SERVICE_STATUS structure.

    gSvcStatus.dwCurrentState = dwCurrentState;
    gSvcStatus.dwWin32ExitCode = dwWin32ExitCode;
    gSvcStatus.dwWaitHint = dwWaitHint;

    if (dwCurrentState == SERVICE_START_PENDING)
        gSvcStatus.dwControlsAccepted = 0;
    else gSvcStatus.dwControlsAccepted = SERVICE_ACCEPT_STOP;

    if ( (dwCurrentState == SERVICE_RUNNING) ||
           (dwCurrentState == SERVICE_STOPPED) )
        gSvcStatus.dwCheckPoint = 0;
    else gSvcStatus.dwCheckPoint = dwCheckPoint++;

    // Report the status of the service to the SCM.
    SetServiceStatus( gSvcStatusHandle, &gSvcStatus );
}

//
// Purpose: 
//   Called by SCM whenever a control code is sent to the service
//   using the ControlService function.
//
// Parameters:
//   dwCtrl - control code
// 
// Return value:
//   None
//
DWORD WINAPI SvcCtrlHandler(DWORD  dwCtrl,DWORD  dwEventType,LPVOID lpEventData,LPVOID lpContext)
{
   switch(dwCtrl) 
   {  
      case SERVICE_CONTROL_STOP: 
         ReportSvcStatus(SERVICE_STOP_PENDING, NO_ERROR, 0);

         // Signal the service to stop.

         SetEvent(ghSvcStopEvent);
         
         return 0;
 
      case SERVICE_CONTROL_INTERROGATE: 
         break; 
 
      default: 
         break;
   } 

	return 0;
}
//
// Purpose: 
//   Logs messages to the event log
//
// Parameters:
//   szFunction - name of function that failed
// 
// Return value:
//   None
//
// Remarks:
//   The service must have an entry in the Application event log.
//
VOID SvcReportEvent(LPTSTR szFunction) 
{ 

}
