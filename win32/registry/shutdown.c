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
	DWORD i;

	if ( argc != 2 ) {
		printf(TEXT("Usage: %s <server>\n"), argv[0]);
		exit( 1 );
	}

	printf(TEXT("Initaiting system shutdown request...."));
	if ( !InitiateSystemShutdownEx( argv[1], TEXT("Shutting down now"), 60, TRUE, TRUE, SHTDN_REASON_MAJOR_APPLICATION) )
	{
		printf(TEXT("failed!\n"));
		PrintLastError();
	}
	printf(TEXT("ok\n"));

	for( i=0; i<10; i++ ) {
		if ( i%2 == 0 )
			printf(TEXT("."));
	}
	printf(TEXT("\n"));

	printf(TEXT("Aborting system shutdown request...."));
	if ( !AbortSystemShutdown( argv[1]) )
	{
		printf(TEXT("failed!\n"));
		PrintLastError();
	}
	printf(TEXT("ok\n"));



	exit(0);

}