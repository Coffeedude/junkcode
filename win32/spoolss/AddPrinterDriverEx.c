/******************************************************************
 * SPOOLSS regression testing code for Samba print servers
 * 
 *****************************************************************/

#include <windows.h>
#include <stdio.h>
#include "printlib.h"

/* we assume the necessary driver files are already uploaded
   on the server.  We also only exercise The info level 6
   since that is the one used by the Win2k Add Printer Wizard */
int main (int argc, char* argv[])
{
	DRIVER_INFO_6	buffer;
	BYTE			pDependentFiles[] = { "PSCRIPT.HLP\0PSCRPTUI.DLL\0PSCRIPT.DLL\0LWNTX518.PPD\0\0" };
	LPVOID			lpMsgBuf;

	if (argc < 2)
	{
		fprintf (stderr, "useage: %s <servername>\n", argv[0]);
		exit (-1);
	}

	/* we'll use the Apple LaserWriter II NTX v51.8 driver
	   for simplcity */
	memset( &buffer, 0, sizeof(DRIVER_INFO_6) );

	buffer.cVersion			= 0;
	buffer.pEnvironment		= strdup("Windows NT x86");
	buffer.pName			= strdup("Apple LaserWriter II NTX v51.8");
	buffer.pDriverPath		= strdup("pscript.dll");
	buffer.pDataFile		= strdup("lwntx518.ppd");
	buffer.pConfigFile		= strdup("pscriptui.dll");
	buffer.pDependentFiles	= pDependentFiles;
	buffer.pHelpFile		= strdup("pscript.hlp");
	buffer.pDefaultDataType = strdup("RAW");

	printf ("Attempting to add Driver Info Level 6 :\n");
	print_driver_info_6 (&buffer);
	printf ("\n");


	if (!AddPrinterDriverEx(argv[1], 6, (LPBYTE)&buffer, APD_COPY_ALL_FILES))
		PrintLastError();
	else
		printf ("Printer Driver installed successfully\n");

	if (!AddPrinterDriver(argv[1], 6, (LPBYTE)&buffer))
		PrintLastError();
	else
		printf ("Printer Driver installed successfully\n");


	return 0;

}