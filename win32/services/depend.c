#if 1	/* unicode supprot */
#  define UNICODE
#  define _UNICODE
#  include <tchar.h>
#  include <wchar.h>
#  define printf wprintf
#  define MAIN wmain
#  define WINCHAR WCHAR
#else	/* ansi characters */
#  include <stdio.h>
#  define WINCHAR char
#  define MAIN main
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

int MAIN( int argc, WINCHAR *argv[] )
{
	SC_HANDLE hSCM, hService;
	DWORD needed, returned;
	LPENUM_SERVICE_STATUS pServices = NULL;
	DWORD i;

	if ( argc != 3 ) {
		printf(TEXT("Usage: %s <server> <service>\n"), argv[0]);
		exit( 1 );
	}

	printf(TEXT("Opening the Service Control Manager on %s..."), (LPTSTR)argv[1]);
	if ( !(hSCM = OpenSCManager( (LPTSTR)argv[1], NULL, SC_MANAGER_ALL_ACCESS )) ) {
		printf(TEXT("failed.\n"));
		PrintLastError();
		exit(1);
	}
	printf(TEXT("ok\n"));

	printf(TEXT("Opening service \"%s\"..."), (LPTSTR)argv[2]);
	if ( !(hService = OpenService( hSCM, (LPTSTR)argv[2], SERVICE_ALL_ACCESS )) ) {
		printf(TEXT("failed.\n"));
		PrintLastError();
		CloseServiceHandle( hSCM );
		exit(1);
	}
	printf(TEXT("ok\n"));

	printf(TEXT("Enumerating dependent services...."));
	needed = returned = 0;
	if ( !EnumDependentServices( hService, SERVICE_ACTIVE, NULL, 0, &needed, &returned ) ) 
	{
		DWORD status;
		DWORD bufsize;

		status = GetLastError();
		if ( status != ERROR_MORE_DATA ) 
		{
			printf(TEXT("failed.\n"));
			printf(TEXT("error == %d\n"), status);
			CloseServiceHandle( hSCM );
			exit(1);
		}

		pServices = (LPENUM_SERVICE_STATUS)malloc(needed);
		bufsize = needed;
		if ( !EnumDependentServices( hService, SERVICE_ACTIVE, pServices, bufsize, &needed, &returned ) ) 
		{
			PrintLastError();
			CloseServiceHandle( hSCM );
			exit(1);
		}
	}
	printf(TEXT("ok\n"));

	if ( returned == 0 )	
		printf(TEXT("**no dependent services**\n"));

	for (i=0; i<returned; i++) {
		printf(TEXT("%s (%s)\n"), pServices[i].lpDisplayName, pServices[i].lpServiceName );
		printf(TEXT("\ttype              = 0x%x\n"), pServices[i].ServiceStatus.dwServiceType );
		printf(TEXT("\tstate             = 0x%x\n"), pServices[i].ServiceStatus.dwCurrentState );
		printf(TEXT("\tcontrols accepted = 0x%x\n"), pServices[i].ServiceStatus.dwControlsAccepted );
		printf(TEXT("\twin32 exit code   = 0x%x\n"), pServices[i].ServiceStatus.dwWin32ExitCode );
		printf(TEXT("\tsvc exit code     = 0x%x\n"), pServices[i].ServiceStatus.dwServiceSpecificExitCode );
		printf(TEXT("\tcheck point       = 0x%x\n"), pServices[i].ServiceStatus.dwCheckPoint );
		printf(TEXT("\twait hont         = 0x%x\n"), pServices[i].ServiceStatus.dwWaitHint );
	}

	free( pServices );

	printf(TEXT("Closing Service handles..."));
	CloseServiceHandle( hSCM );
	CloseServiceHandle( hService );
	printf(TEXT("ok\n"));

	exit(0);

}