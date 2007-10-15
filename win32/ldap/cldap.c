#if 1   /* unicode supprot */
#  define UNICODE
#  define _UNICODE
#  include <tchar.h>
#  include <wchar.h>
#  define printf wprintf
#  define MAIN wmain
#  define WINCHAR WCHAR
#else   /* ansi characters */
#  include <stdio.h>
#  define WINCHAR char
#  define MAIN main
#endif

#include <windows.h>
/* #include <strsafe.h> */
#include <stdlib.h>

#include <winldap.h>

/********************************************************************
********************************************************************/

static void PrintLastError( void )
{
        LPVOID          lpMsgBuf;
        DWORD           status;

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
	LDAP *ld = NULL;
	ULONG rc;
	LDAPMessage *msg = NULL;
	WINCHAR *base = TEXT("");
	WINCHAR *filter = TEXT("(objectclass=*)");
	WINCHAR *attrs[] = { TEXT("currentTime"), NULL };

	if ( argc != 2 ) {
		printf(TEXT("Usage: %s hostname"), (LPTSTR)argv[0]);
		exit( 1 );
	}

	printf(TEXT("Setting up udp state for host %s..."), (LPTSTR)argv[1]);
	if ( (ld = cldap_open( (LPTSTR)argv[1], 389 )) == NULL ) {
		printf(TEXT("failed.\n"));
		PrintLastError();
		exit( 2 );
	}
	printf(TEXT("ok.\n"));

	rc = ldap_search_s( ld, base, LDAP_SCOPE_BASE, filter, attrs, 0, &msg );

	if ( rc == -1 ) {
		printf(TEXT("Search failed!\n"));
	}

	if ( msg )
			ldap_msgfree( msg );

	ldap_unbind( ld );



	return 0;
}
