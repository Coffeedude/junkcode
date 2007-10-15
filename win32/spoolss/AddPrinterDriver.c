/******************************************************************
 * SPOOLSS regression testing code for Samba print servers
 * 
 *****************************************************************/

#include <windows.h>
#include <stdio.h>
#include "printlib.h"

/* we assume the necessary driver files are already uploaded
   on the server.  We also only exercise The info level 3
   since that is the one used by the NT Add Printer Wizard */
int main (int argc, char* argv[])
{
	DRIVER_INFO_3	buffer3;
	BYTE			pDependentFiles[] = { "PSCRIPT.HLP\0PSCRPTUI.DLL\0PSCRIPT.DLL\0LWNTX518.PPD\0\0" };
	LPVOID			lpMsgBuf;

	if (argc < 2)
	{
		fprintf (stderr, "useage: %s <servername>\n", argv[0]);
		exit (-1);
	}

	/* we'll use the Apple LaserWriter II NTX v51.8 driver
	   for simplcity */
	buffer3.cVersion		= 2;
	buffer3.pEnvironment	= strdup("Windows NT x86");
	buffer3.pName			= strdup("Apple LaserWriter II NTX v51.8");
	buffer3.pDriverPath		= strdup("pscript.dll");
	buffer3.pDataFile		= strdup("lwntx518.ppd");
	buffer3.pConfigFile		= strdup("pscriptui.dll");
	buffer3.pDependentFiles	= pDependentFiles;
	buffer3.pHelpFile		= strdup("pscript.hlp");
	buffer3.pMonitorName	= NULL;
	buffer3.pDefaultDataType = strdup("RAW");

	printf ("Attempting to add Driver Info Level 3 :\n");
	print_driver_info_3 (&buffer3);
	printf ("\n");


	if (!AddPrinterDriver(argv[1], 3, (LPBYTE)&buffer3))
	{
		FormatMessage( FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | 
			FORMAT_MESSAGE_IGNORE_INSERTS, NULL, GetLastError(),
			MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), // Default language
			(LPTSTR) &lpMsgBuf, 0, NULL);
		printf ("ERROR : %s\n", (char*)lpMsgBuf);
		LocalFree (lpMsgBuf);
	}
	else
	{
		printf ("Printer Driver installed successfully\n");
	}


	return 0;

}