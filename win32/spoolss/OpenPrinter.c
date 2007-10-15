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


	if (argc < 2)
	{
		fprintf (stderr, "useage: %s <printername>\n", argv[0]);
		exit (-1);
	}

	if (!OpenPrinter (argv[1], &printer, NULL))
	{
		PrintLastError();
		exit (-1);
	}
	else
		printf ("Printer [%s] opened successfully.\n\n", argv[1]);

	if (!ClosePrinter(printer))
		PrintLastError();
	else
		printf ("Printer [%s] closed successfully.\n", argv[1]);

	return 0;

}