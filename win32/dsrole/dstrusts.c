#include <windows.h>
#include <stdio.h>
#include <dsgetdc.h>
#include <lm.h>

#define UNICODE


void PrintLastError()
{
	LPVOID		lpMsgBuf;
	DWORD		status;

	status = GetLastError();

	FormatMessage( FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | 
		FORMAT_MESSAGE_IGNORE_INSERTS, NULL, GetLastError(),
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), // Default language
		(LPTSTR) &lpMsgBuf, 0, NULL);
	printf ("ERROR  [0x%x] : %s\n", status, (char*)lpMsgBuf);
	LocalFree (lpMsgBuf);

		return;
}




int main ( int argc, char *argv[] )
{
	DS_DOMAIN_TRUSTS	*trusts = NULL;
	DWORD				count = 0;
	DWORD				i, flags;

	if (argc != 2 ) {
		printf( "Usage: %s <server>\n", argv[0] );
		exit (1);
	}

	flags = DS_DOMAIN_IN_FOREST;

	printf( "Looking up domain trusts....\n");

	if ( DsEnumerateDomainTrusts( argv[1], flags, &trusts, &count ) != ERROR_SUCCESS ) {
		PrintLastError();
		exit (1);
	}

	for ( i=0; i<count; i++ ) {
		printf("domain = %s\nrealm = %s\n", 
			trusts[i].NetbiosDomainName,
			trusts[i].DnsDomainName);
		printf("flags           = 0%x\n", trusts[i].Flags);
		printf("parent index    = 0%x\n", trusts[i].ParentIndex);
		printf("trust type      = 0%x\n", trusts[i].TrustType);
		printf("trust attribute = 0%x\n", trusts[i].TrustAttributes);
	}

	if ( trusts )
		NetApiBufferFree( trusts );


	return 0;
}