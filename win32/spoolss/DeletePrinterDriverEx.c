/******************************************************************
 * SPOOLSS regression testing code for Samba print servers
 * 
 *****************************************************************/

#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include "printlib.h"

/* we assume the necessary driver files are already uploaded
   on the server.  We also only exercise The info level 3
   since that is the one used by the NT Add Printer Wizard */
int main (int argc, char* argv[])
{
	LPVOID			lpMsgBuf;

	if (argc < 3)
	{
		fprintf (stderr, "useage: %s <servername> <arch> <driver name> <version>\n", argv[0]);
		exit (-1);
	}


	if (!DeletePrinterDriverEx( argv[1], argv[2], argv[3], DPD_DELETE_SPECIFIC_VERSION|DPD_DELETE_UNUSED_FILES, atoi(argv[4]) ) ) 
		PrintLastError();
	else
		printf ("Printer Driver deleted successfully\n");

	return 0;

}