#if 1	/* unicode supprot */
#  define UNICODE
#  define _UNICODE
#  include <tchar.h>
#  include <wchar.h>
#  define printf wprintf
#  define main wmain
#  define MYCHAR WCHAR
#else	/* ansi characters */
#  include <stdio.h>
#  define MYCHAR char
#endif

#include <windows.h>
/* #include <strsafe.h> */
#include <stdlib.h>



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


int main( int argc, MYCHAR *argv[] )
{
	HKEY					h, hPrint;
	SECURITY_ATTRIBUTES		acl;
	MYCHAR                  valname[1024];
	BYTE					data[4096];
	DWORD					type;
	DWORD					name_len = 1024, data_len = 4096;
	DWORD					result;

	acl.nLength = sizeof(SECURITY_ATTRIBUTES);
	acl.bInheritHandle = FALSE;
	acl.lpSecurityDescriptor = NULL;

#if 0
	if ( argc != 4)
	{
		printf(TEXT("Usage: %s <machine> <path> <file>\n"), (LPTSTR)argv[0]);
		exit(1);
	}
#endif
	printf(TEXT("Opening HKLM on %s..."), (LPTSTR)argv[1]);
	if (RegConnectRegistry(argv[1], HKEY_LOCAL_MACHINE,  &h) != ERROR_SUCCESS)
	{
		PrintLastError();
		exit(1);
	}
	printf(TEXT("ok\n"));

	printf(TEXT("Opening subkey [%s]..."), argv[2]);
	if ( RegOpenKey( h, (LPTSTR)argv[2], &hPrint ) != ERROR_SUCCESS ) {
		PrintLastError();
		RegCloseKey( h );
		exit(1);
	}
	printf(TEXT("ok\n"));

#if 0
	printf(TEXT("Enumerating values..."));
	result = RegEnumValue( hPrint, 0, (LPTSTR)valname, &name_len, NULL, &type, (LPBYTE)data, &data_len);

	if ( result!=ERROR_SUCCESS && result!=ERROR_NO_MORE_ITEMS ) {
		PrintLastError();
		RegCloseKey( h );
		exit(1);
	}
	printf(TEXT("ok\n"));
#endif

	printf(TEXT("Restoring Print subkey..."));
	if ( RegSaveKey( hPrint, (LPTSTR)argv[3], &acl ) != ERROR_SUCCESS ) {
		PrintLastError();
		RegCloseKey( hPrint );
		RegCloseKey( h );
		exit(1);		
	}
	printf(TEXT("ok\n"));

	printf(TEXT("Closing Keys..."));
	if (RegCloseKey( hPrint ) != ERROR_SUCCESS) {
		PrintLastError();
		exit(1);
	}
	if (RegCloseKey( h ) != ERROR_SUCCESS) {
		PrintLastError();
		exit(1);
	}
	printf(TEXT("ok\n"));

	return 0;
}

