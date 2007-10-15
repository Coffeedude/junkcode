#include <windows.h>
#include <stdio.h>
#include <dsrole.h>


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
	DSROLE_PRIMARY_DOMAIN_INFO_BASIC	*info;
	DSROLE_UPGRADE_STATUS_INFO		*info2;	
	PBYTE	buf;
	WCHAR	server[256];

	if (argc != 2 ) {
		printf( "Usage: %s <server>\n", argv[0] );
		exit (1);
	}

	MultiByteToWideChar( CP_ACP, 0, argv[1], strlen(argv[1])+1, server, 256);

	printf( "Looking up domain information....\n");
	if ( DsRoleGetPrimaryDomainInformation( server, DsRolePrimaryDomainInfoBasic, &buf ) != ERROR_SUCCESS ) 
		PrintLastError();
	else
	{
		info = (DSROLE_PRIMARY_DOMAIN_INFO_BASIC*)buf;	

		printf ("Domain Controller [%s] is in ", argv[1]);
		if ( !(info->Flags & DSROLE_PRIMARY_DS_RUNNING) ) 
			printf("Directory Service not running on %s\n", argv[1]);		
		else if ( info->Flags&DSROLE_PRIMARY_DS_MIXED_MODE )
			printf ("mixed mode\n");
		else
			printf ("native mode\n");

		printf( "flags = [0x%x]\n", info->Flags );
		printf( "role = [0x%x]\n", info->MachineRole );

		if ( info->Flags & DSROLE_PRIMARY_DOMAIN_GUID_PRESENT ) 
		{
			int i;

			printf( "Domain Guid = \n");
			printf( "\t0x%x\n", info->DomainGuid.Data1 );
			printf( "\t0x%x\n", info->DomainGuid.Data2 );
			printf( "\t0x%x\n", info->DomainGuid.Data3 );
			for ( i=0; i<8; i++ )
				printf( "0x%x ", info->DomainGuid.Data4[i] );
			printf( "\n" );
		}

	}

	if ( buf )
		DsRoleFreeMemory( buf );


	buf = NULL;

	if ( DsRoleGetPrimaryDomainInformation( server, DsRoleUpgradeStatus, &buf ) != ERROR_SUCCESS ) 
		PrintLastError();
	else
	{
		info2 = (DSROLE_UPGRADE_STATUS_INFO*)buf;	

		printf( "DsRoleUpgradeStatus:\n");
		printf( "Current  = [0x%x]\n", info2->OperationState);
		printf( "Previous = [0x%x]\n", info2->PreviousServerState );

	}

	if ( buf )
		DsRoleFreeMemory( buf );

	return 0;
}