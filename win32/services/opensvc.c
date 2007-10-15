#if 1	/* unicode supprot */
#  define UNICODE
#  define _UNICODE
#  include <tchar.h>
#  include <wchar.h>
#  define printf wprintf
#  define main wmain
#  define SVC_CHAR WCHAR
#else	/* ansi characters */
#  include <stdio.h>
#  define SVC_CHAR char
#endif

#include <windows.h>
/* #include <strsafe.h> */
#include <stdlib.h>



/********************************************************************
********************************************************************/

static void PrintLastError( void )
{
	LPVOID		lpMsgBuf;
	DWORD		status;

	status = GetLastError();

	FormatMessage( FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | 
		FORMAT_MESSAGE_IGNORE_INSERTS, NULL, GetLastError(),
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), // Default language
		(LPTSTR) &lpMsgBuf, 0, NULL);
	printf(TEXT("ERROR  [0x%x] : %s\n"), status, (LPTSTR)lpMsgBuf);
	LocalFree (lpMsgBuf);

		return;
}

void PrintServiceStatus( SERVICE_STATUS *status)
{
	printf(TEXT("\ttype              = 0x%x\n"), status->dwServiceType );
	printf(TEXT("\tstate             = 0x%x\n"), status->dwCurrentState );
	printf(TEXT("\tcontrols accepted = 0x%x\n"), status->dwControlsAccepted );
	printf(TEXT("\twin32 exit code   = 0x%x\n"), status->dwWin32ExitCode );
	printf(TEXT("\tsvc exit code     = 0x%x\n"), status->dwServiceSpecificExitCode );
	printf(TEXT("\tcheck point       = 0x%x\n"), status->dwCheckPoint );
	printf(TEXT("\twait hint         = 0x%x\n"), status->dwWaitHint );
}

/********************************************************************
********************************************************************/

int main( int argc, SVC_CHAR *argv[] )
{
	SC_HANDLE hSCM, hService;
	SERVICE_STATUS status;

	if ( argc != 3 ) {
		printf(TEXT("Usage: %s <server> <servicename>\n"), argv[0]);
		exit( 1 );
	}

	/* StringCbPrintf( server, sizeof(server), TEXT("%s"), (LPTSTR)argv[1]); */

	printf(TEXT("Opening the Service Control Manager on %s..."), (LPTSTR)argv[1]);
	if ( !(hSCM = OpenSCManager( (LPTSTR)argv[1], NULL, SC_MANAGER_ALL_ACCESS )) ) {
		PrintLastError();
		exit(1);
	}
	printf(TEXT("ok\n"));

	printf(TEXT("Opening the Service \"%s\"..."), (LPTSTR)argv[2]);
	if ( !(hService = OpenService( hSCM, (LPTSTR)argv[2], SERVICE_ALL_ACCESS )) ) {
		PrintLastError();
		exit(1);
	}
	printf(TEXT("ok\n"));

	printf(TEXT("Stopping \"%s\"..."), (LPTSTR)argv[2]);
	if ( !ControlService( hService, SERVICE_CONTROL_STOP, &status) ){
		PrintLastError();
		exit(1);
	}
	printf(TEXT("ok\n"));
	PrintServiceStatus( &status );
	printf(TEXT("\n"));


	printf(TEXT("Querying \"%s\"..."), (LPTSTR)argv[2]);
	memset( &status, 0x0, sizeof(SERVICE_STATUS) );
	if ( !QueryServiceStatus( hService, &status ) ){
		PrintLastError();
		exit(1);
	}
	printf(TEXT("ok\n"));
	PrintServiceStatus( &status );
	printf(TEXT("\n"));

	printf(TEXT("Starting \"%s\"..."), (LPTSTR)argv[2]);
	if ( !StartService( hService, 0, NULL) ){
		PrintLastError();
		exit(1);
	}
	printf(TEXT("ok\n"));

	printf(TEXT("Querying \"%s\"..."), (LPTSTR)argv[2]);
	memset( &status, 0x0, sizeof(SERVICE_STATUS) );
	if ( !QueryServiceStatus( hService, &status ) ){
		PrintLastError();
		exit(1);
	}
	printf(TEXT("ok\n"));
	PrintServiceStatus( &status );
	printf(TEXT("\n"));


	printf(TEXT("Closing Service handle..."));
	CloseServiceHandle( hService );
	printf(TEXT("ok\n"));

	printf(TEXT("Closing Service Control Manager handle..."));
	CloseServiceHandle( hSCM );
	printf(TEXT("ok\n"));

	exit(0);

}