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

/********************************************************************
********************************************************************/

int main( int argc, SVC_CHAR *argv[] )
{
	SC_HANDLE hndScm;
	DWORD needed, returned, resume;
	LPENUM_SERVICE_STATUS pServices = NULL;
	DWORD i;

	if ( argc != 2 ) {
		printf(TEXT("Usage: %s <server>\n"), argv[0]);
		exit( 1 );
	}

	/* StringCbPrintf( server, sizeof(server), TEXT("%s"), (LPTSTR)argv[1]); */

	printf(TEXT("Opening the Service Control Manager on %s..."), (LPTSTR)argv[1]);
	if ( !(hndScm = OpenSCManager( (LPTSTR)argv[1], NULL, SC_MANAGER_ALL_ACCESS )) ) {
		PrintLastError();
		exit(1);
	}
	printf(TEXT("ok\n"));

	printf(TEXT("Enumerating services...."));
	needed = returned = resume = 0;
	if ( !EnumServicesStatus( hndScm, SERVICE_WIN32, SERVICE_ACTIVE, NULL, 0, &needed, &returned, &resume ) ) 
	{
		DWORD status;
		DWORD bufsize;

		status = GetLastError();
		if ( status != ERROR_MORE_DATA ) 
		{
			printf(TEXT("error == %d\n"), status);
			CloseServiceHandle( hndScm );
			exit(1);
		}

		pServices = (LPENUM_SERVICE_STATUS)malloc(needed);
		bufsize = needed;
		if ( !EnumServicesStatus( hndScm, SERVICE_WIN32, SERVICE_ACTIVE, pServices, bufsize, &needed, &returned, &resume ) ) 
		{
			PrintLastError();
			CloseServiceHandle( hndScm );
			exit(1);
		}
	}
	printf(TEXT("ok\n"));

	for (i=0; i<returned; i++) {
		printf(TEXT("%s (%s)\n"), pServices[i].lpDisplayName, pServices[i].lpServiceName );
		printf(TEXT("\ttype              = 0x%x\n"), pServices[i].ServiceStatus.dwServiceType );
		printf(TEXT("\tstate             = 0x%x\n"), pServices[i].ServiceStatus.dwCurrentState );
		printf(TEXT("\tcontrols accepted = 0x%x\n"), pServices[i].ServiceStatus.dwControlsAccepted );
		printf(TEXT("\twin32 exit code   = 0x%x\n"), pServices[i].ServiceStatus.dwWin32ExitCode );
		printf(TEXT("\tsvc exit code     = 0x%x\n"), pServices[i].ServiceStatus.dwServiceSpecificExitCode );
		printf(TEXT("\tcheck point       = 0x%x\n"), pServices[i].ServiceStatus.dwCheckPoint );
		printf(TEXT("\twait hint         = 0x%x\n"), pServices[i].ServiceStatus.dwWaitHint );
	}
	free( pServices );

	printf(TEXT("Closing Service Control Manager handle..."));
	CloseServiceHandle( hndScm );
	printf(TEXT("ok\n"));

	exit(0);

}