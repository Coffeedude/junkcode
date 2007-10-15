/******************************************************************
 * SPOOLSS regression testing code for Samba print servers
 * 
 *****************************************************************/

#include <windows.h>
#include <stdio.h>
#include "printlib.h"

/* we will excercise info level 2 (cange the drivername)
   and info level 3 (security descriptor) */
int main (int argc, char* argv[])
{
	DWORD	needed;
	HANDLE	printer;
	PPRINTER_INFO_2	buffer2 = NULL;
	PRINTER_INFO_3	buffer3;
	
	if (argc < 3)
	{
		fprintf (stderr, "useage: %s <printername> <drivername>\n", argv[0]);
		exit (-1);
	}

	/* open the printer */
	if (!OpenPrinter (argv[1], &printer, NULL))
	{
		fprintf (stderr, "Unable to open %s!\n", argv[1]);
		exit (-1);
	}
	else
	{
		printf ("Printer [%s] opened successfully.\n\n", argv[1]);
	}

	/* INFO LEVEL 2 */
	GetPrinter(printer, 2, (LPBYTE)buffer2, 0, &needed);
	if (GetLastError() != ERROR_INSUFFICIENT_BUFFER)
	{
		if (needed != 0)
			fprintf (stderr, "Error GetPrinter Info Level 2 for [%s] using NULL buffer.\n", argv[1]);
		else
			printf ("Info level 2 returned no information\n");
	}
	else
	{
		if ((buffer2 = (LPBYTE)malloc(needed)) == NULL)
		{
			fprintf (stderr, "Unable to malloc memory for buffer!\n");
			exit (-1);
		}
		if (!GetPrinter(printer, 2, (LPBYTE)buffer2, needed, &needed))
		{
			fprintf (stderr, "Error getting printers Info Level 2 for [%s].\nSize of buffer = %d\n", 
					argv[1], needed);
		}

		printf ("Printer Info Level 2:\n");
		print_printer_info_2(buffer2);
		printf ("\n");
	}

	/* set the new drivername */
	buffer2->pDriverName = strdup (argv[2]);
	if (SetPrinter(printer, 2, (LPBYTE)buffer2, 0))
	{
		printf ("Printer driver successfully changed to [%s]\n", argv[2]);
		printf ("Printer Info 2 :\n");
		print_printer_info_2 (buffer2);
		printf ("\n");
	}
	else
		PrintLastError();

	/* exercise the various command options */
	printf ("command = PRINTER_CONTROL_PAUSE\n");
	if (SetPrinter (printer, 2, (LPBYTE)buffer2, PRINTER_CONTROL_PAUSE))
		printf ("Printer successfully paused.\n");
	else
		PrintLastError();
	printf ("\n");

	printf ("command = PRINTER_CONTROL_RESUME\n");
	if (SetPrinter (printer, 2, (LPBYTE)buffer2, PRINTER_CONTROL_RESUME))
		printf ("Printer successfully paused.\n");
	else
		PrintLastError();
	printf ("\n");

	printf ("command = PRINTER_CONTROL_PURGE\n");
	if (SetPrinter (printer, 2, (LPBYTE)buffer2, PRINTER_CONTROL_PURGE))
		printf ("Printer successfully paused.\n");
	else
		PrintLastError();
	printf ("\n");


	/* we'll just use the security descriptor from the buffer2 */
	printf ("Testing setting of security descriptor using PRINTER_INFO_3...\n");
	buffer3.pSecurityDescriptor = buffer2->pSecurityDescriptor;

	printf ("Current security descriptor settings for [%s]:\n", argv[1]);
	/* print_secdesc (buffer3.pSecurityDescriptor); */
	printf ("\n");

	printf ("no changes to security descriptor...\n\n");


	if (SetPrinter (printer, 3, (LPBYTE)&buffer3, 0))
	{
		printf ("Printer descriptor information updated successfully.\n");
		printf ("New security descriptor settings for [%s]:\n", argv[1]);
		/* print_secdesc (buffer3.pSecurityDescriptor); */
		printf ("\n");
	}
	else
		PrintLastError();
	printf ("\n");

	/* close the printer */
	if (!ClosePrinter(printer))
	{
		fprintf (stderr, "Error closing printer!\n");
		exit (-1);
	}
	else
	{
		printf ("Printer [%s] closed successfully.\n", argv[1]);
	}

	return 0;

}

