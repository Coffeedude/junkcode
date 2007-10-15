/******************************************************************
 * SPOOLSS regression testing code for Samba print servers
 * 
 *****************************************************************/

#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include "printlib.h"


#define ENUM_KEY_NAME	"PrinterDriverData"

int main (int argc, char* argv[])
{
	
	HANDLE			printer;
	DWORD			i, status, needed, returned;
	PRINTER_ENUM_VALUES	*buffer = NULL;


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


	needed = returned = 0;
	status = EnumPrinterDataEx(printer, ENUM_KEY_NAME, (LPBYTE)buffer, 0, &needed, &returned);
	if (status == ERROR_MORE_DATA)
	{
		if ((buffer = malloc(needed)) == NULL)
		{
			fprintf (stderr, "Unable to malloc enough space for [%d] bytes\n", needed);
			exit (-1);
		}

		status = EnumPrinterDataEx(printer, ENUM_KEY_NAME, (LPBYTE)buffer, needed, &needed, &returned);
		if (status == ERROR_SUCCESS)
		{
			printf ("Printer Enum Values :\n");
			for (i=0; i<returned; i++)
			{
				print_printer_enum_values (&buffer[i]);
				printf ("\n");
			}
		}
		free (buffer);
	}
	else if (status != ERROR_SUCCESS)
		PrintLastError();


	/* close the server */
	if (!ClosePrinter(printer))
		PrintLastError();
	else
		printf ("Printer [%s] closed successfully.\n", argv[1]);

	return 0;

}
