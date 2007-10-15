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
	DWORD			returned, needed, i;
	PFORM_INFO_1	buffer1 = NULL;

	if (argc < 2)
	{
		fprintf (stderr, "useage: %s <printername>\n", argv[0]);
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
	EnumForms(printer, 1, (LPBYTE)buffer1, 0, &needed, &returned);
	if (GetLastError() != ERROR_INSUFFICIENT_BUFFER)
	{
		fprintf (stderr, "Error getting form information for [%s] using NULL buffer.\n", argv[1]);
		exit (-1);
	}

	if ((buffer1 = (LPBYTE)malloc(needed)) == NULL)
	{
		fprintf (stderr, "Unable to malloc memory for FORM_INFO_1 buffer!\n");
		exit (-1);
	}
	if (!EnumForms(printer, 1, (LPBYTE)buffer1, needed, &needed, &returned))
	{
		fprintf (stderr, "Error getting form information for [%s].\nSize of buffer = %d\n", 
			     argv[1], needed);
		exit (-1);
	}

	printf ("Form Info Level 1:\n");
	for (i=0; i<returned; i++)
	{
		print_form_info_1 (&buffer1[i]);
		printf ("\n");
	}
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