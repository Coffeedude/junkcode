/******************************************************************
 * SPOOLSS regression testing code for Samba print servers
 * 
 *****************************************************************/

#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include "printlib.h"

int main (int argc, char* argv[])
{
	
	HANDLE			printer;
	DWORD			returned=0, 
					status;
	LPTSTR			buffer = NULL;


	if (argc < 2)
	{
		fprintf (stderr, "useage: %s <printername>\n", argv[0]);
		exit (-1);
	}

	/* open the server */
	if (!OpenPrinter (argv[1], &printer, NULL))
		PrintLastError();
	else
		printf ("Printer [%s] opened successfully.\n\n", argv[1]);


	status = DeletePrinterKey(printer, "PrinterDriverData");
	if (status != ERROR_SUCCESS)
		PrintLastError();
	else
		printf ("Success!\n");


	/* close the server */
	if (!ClosePrinter(printer))
		PrintLastError();
	else
		printf ("Printer [%s] closed successfully.\n", argv[1]);

	return 0;

}
