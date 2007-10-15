/******************************************************************
 * SPOOLSS regression testing code for Samba print servers
 * 
 *****************************************************************/

#include <windows.h>
#include <stdio.h>
#include "printlib.h"


int main (int argc, char* argv[])
{
	
	HANDLE			printer;
	DWORD			needed;
	PFORM_INFO_1	buffer1 = NULL;

	if (argc < 3)
	{
		fprintf (stderr, "useage: %s <printername> <form>\n", argv[0]);
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

	/* INFO LEVEL 1 */
	GetForm(printer, argv[2], 1, (LPBYTE)buffer1, 0, &needed);
	if (GetLastError() != ERROR_INSUFFICIENT_BUFFER)
	{
		fprintf (stderr, "Error getting form information for [%s] using NULL buffer.\n", argv[2]);
		exit (-1);
	}

	if ((buffer1 = (LPBYTE)malloc(needed)) == NULL)
	{
		fprintf (stderr, "Unable to malloc memory for FORM_INFO_1 buffer!\n");
		exit (-1);
	}
	if (!GetForm(printer, argv[2], 1, (LPBYTE)buffer1, needed, &needed))
	{
		fprintf (stderr, "Error getting form information for [%2].\nSize of buffer = %d\n", 
			     argv[2], needed);
		exit (-1);
	}

	printf ("Form Info Level 1:\n");
	print_form_info_1 (buffer1);
	printf ("\n");
	free (buffer1);


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