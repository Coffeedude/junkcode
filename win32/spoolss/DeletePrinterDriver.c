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
	LPVOID			lpMsgBuf;

	if (argc < 3)
	{
		fprintf (stderr, "useage: %s <servername> <arch> <driver name>\n", argv[0]);
		exit (-1);
	}


	if (!DeletePrinterDriver(argv[1], argv[2], argv[3]))
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
		printf ("Printer Driver deleted successfully\n");
	}


	return 0;

}