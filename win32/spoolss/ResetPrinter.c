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
	PRINTER_DEFAULTS defaults;
	PPRINTER_INFO_2	buffer2 = NULL;
	DWORD needed, returned;

	memset(&defaults, 0x0, sizeof(PRINTER_DEFAULTS));
	defaults.pDatatype = strdup("RAW");

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

	/* INFO LEVEL 2 */
	needed = returned = 0;
	GetPrinter(printer, 2, (LPBYTE)buffer2, 0, &needed);
	if (GetLastError() != ERROR_INSUFFICIENT_BUFFER)
	{
		if (needed != 0)
			fprintf (stderr, "Error GetPrinter Info Level 2 for [%s] using NULL buffer.\n", argv[1]);
		else
			printf ("Info level 2 returned no information\n");

		goto done;
	}

	if ((buffer2 = (LPBYTE)malloc(needed*2)) == NULL)
	{
		fprintf (stderr, "Unable to malloc memory for buffer!\n");
		exit (-1);
	}

	if (!GetPrinter(printer, 2, (LPBYTE)buffer2, needed, &returned))
	{
		fprintf (stderr, "Error getting printers Info Level 2 for [%s].\nSize of buffer = 0x%x [0x%x]\n", 
				argv[1], needed, returned);
		PrintLastError();
	}

	

	defaults.pDevMode = buffer2->pDevMode;
	if (!ResetPrinter(printer, &defaults))
		PrintLastError();

done:
	if (!ClosePrinter(printer))
		PrintLastError();
	else
		printf ("Printer [%s] closed successfully.\n", argv[1]);

	return 0;

}