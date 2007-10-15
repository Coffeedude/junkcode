/******************************************************************
 * SPOOLSS regression testing code for Samba print servers
 * 
 *****************************************************************/

#include <windows.h>
#include <stdio.h>
#include "printlib.h"


int main (int argc, char* argv[])
{
	
	HANDLE	printer;


	if (argc < 3)
	{
		fprintf (stderr, "useage: %s <printername> <data name>\n", argv[0]);
		exit (-1);
	}

	/* open the server */
	if (!OpenPrinter (argv[1], &printer, NULL))
		PrintLastError();
	else
		printf ("Server [%s] opened successfully.\n\n", argv[1]);

	if (DeletePrinterDataEx(printer, "DsSpool", argv[2]) != ERROR_SUCCESS)
		PrintLastError();
	else
		printf ("Printer Data [%s] deleted successfully\n", argv[2]);


	if (!ClosePrinter(printer))
		PrintLastError();
	else
		printf ("Printer [%s] closed successfully.\n", argv[1]);

	return 0;

}