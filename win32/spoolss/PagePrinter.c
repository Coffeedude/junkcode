/******************************************************************
 * SPOOLSS regression testing code for Samba print servers
 * 
 *****************************************************************/

#include <windows.h>
#include <stdio.h>
#include <fcntl.h>
#include <io.h>
#include <string.h>
#include "printlib.h"


int main (int argc, char* argv[])
{
	
	HANDLE				printer;
	int					fd;
	BYTE				buffer[256];
	DWORD				bytes, wrote;
	DOC_INFO_1			buffer1;
	PRINTER_DEFAULTS	print_defaults;

	if (argc < 3)
	{
		fprintf (stderr, "useage: %s <printername> <filename>\n", argv[0]);
		exit (-1);
	}

	printf ("This test program exercises the following win32 functions:\n");
	printf ("\tResetPrinter()\n");
	printf ("\tStartDocPrinter()\n");
	printf ("\tEndDocPrinter()\n");
	printf ("\tStartPagePrinter()\n");
	printf ("\tEndPagePrinter()\n");
	printf ("\tWritePrinter()\n");
	printf ("\n\n");

	printf ("This test program exercises both the StartPagePrinter()\n");
	printf ("and EndPagePrinter() Win32 functions.\n\n");

	if (!OpenPrinter (argv[1], &printer, NULL))
	{
		fprintf (stderr, "Unable to open %s!\n", argv[1]);
		exit (-1);
	}
	else
	{
		printf ("Printer [%s] opened successfully.\n\n", argv[1]);
	}

	/* set the printer defaults */
	print_defaults.pDatatype		= strdup ("RAW");
	print_defaults.DesiredAccess	= NULL;
	print_defaults.pDevMode			= NULL;
	if (ResetPrinter(printer, &print_defaults))
		printf ("ResetPrinter call succeeded\n");
	else
		PrintLastError();
	printf ("\n");

	/* StartDocPrinter */
	buffer1.pDocName	= strdup("Testing Printer");
	buffer1.pDatatype	= strdup("RAW");
	buffer1.pOutputFile	= NULL;

	printf ("Attempting to call StartDocPrinter() using DOC_INFO_1 :\n");
	print_doc_info_1 (&buffer1);
	printf ("\n");

	if (StartDocPrinter (printer, 1, (LPBYTE)&buffer1))
		printf ("StartDocPrinter called successfully\n");
	else
		PrintLastError();
	printf ("\n");


	if (StartPagePrinter(printer))
	{
		printf ("StartPagePrinter returned success.\n");

		if ((fd=_open(argv[2], _O_RDONLY)) == -1)
		{
			fprintf (stderr, "ERROR: Unable to open [%s] for read access!\n", argv[2]);
			ClosePrinter (printer);
			exit (-1);
		}

		while ((bytes=_read(fd, buffer, 256)) != 0)
		{
			if (!WritePrinter(printer, (LPVOID)buffer, bytes, &wrote))
			{
				fprintf (stderr, "ERROR: WritePrinter failed for [%d] bytes!\n", bytes);
				PrintLastError();
			}
			else
				printf ("Successfully wrote [%d] bytes to the printer\n", bytes);
		}
		_close (fd);

		if (EndPagePrinter (printer))
			printf ("EndPagePrinter returned success.\n");
		else
			PrintLastError();
	}
	else
		PrintLastError();
	printf ("\n");
			
	printf ("Attempting to call EndDocPrinter\n");
	if (EndDocPrinter (printer))
		printf ("EndDocPrinter called successfully\n");
	else
		PrintLastError();
	printf ("\n");



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