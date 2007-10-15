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


	status = EnumPrinterKey(printer, "", buffer, 0, &returned);
	if (status == ERROR_MORE_DATA)
	{
		if ((buffer = (LPTSTR)malloc(returned)) == NULL)
		{
			fprintf (stderr, "Unable to malloc enough space for [%d] bytes\n", returned);
			exit (-1);
		}

		status = EnumPrinterKey(printer, "", buffer, returned, &returned);
		if (status == ERROR_SUCCESS)
		{
			LPTSTR	p = NULL;

			printf ("Enumurated Key Values : [%d]\n", returned);

			p = buffer;	
			while (*p)
			{ 
				printf ("\t%s\n", p);
				for (; *p; p=CharNext(p));
				p=CharNext(p);
			}

		}
		free (buffer);

	}
	else if (status != ERROR_SUCCESS)
		PrintLastError();
	else
		printf ("No data!\n");

	returned = 0;
	status = EnumPrinterKey(printer, "PrinterDriverData", buffer, 0, &returned);
	if (status == ERROR_MORE_DATA)
	{
		if ((buffer = (LPTSTR)malloc(returned)) == NULL)
		{
			fprintf (stderr, "Unable to malloc enough space for [%d] bytes\n", returned);
			exit (-1);
		}

		status = EnumPrinterKey(printer, "PrinterDriverData", buffer, returned, &returned);
		if (status == ERROR_SUCCESS)
		{
			LPTSTR	p = NULL;

			printf ("Enumurated Key Values : [%d]\n", returned);

			p = buffer;	
			while (*p)
			{ 
				printf ("\t%s\n", p);
				for (p = buffer; *p; p=CharNext(p));
				p=CharNext(p);
			}

		}
		free (buffer);

	}
	else if (status != ERROR_SUCCESS)
		PrintLastError();
	else
		printf ("No data!\n");






	/* close the server */
	if (!ClosePrinter(printer))
		PrintLastError();
	else
		printf ("Printer [%s] closed successfully.\n", argv[1]);

	return 0;

}
