/******************************************************************
 * SPOOLSS regression testing code for Samba print servers
 * 
 *****************************************************************/

#include <windows.h>
#include <stdio.h>
#include "printlib.h"


int main (int argc, char* argv[])
{
	
	HANDLE				printer;
	PRINTER_DEFAULTS	defaults;
	DWORD				returned, needed, i;
	PFORM_INFO_1		buffer1 = NULL;

	if ( argc < 3) {
		fprintf (stderr, "useage: %s <localprinter> <networkprinter>\n", argv[0]);
		exit (-1);
	}

	defaults.DesiredAccess = PRINTER_ACCESS_ADMINISTER;
	defaults.pDatatype = NULL;
	defaults.pDevMode = NULL;

	/* open the printer */
	if (!OpenPrinter (argv[1], &printer, &defaults))
	{
		fprintf (stderr, "Unable to open %s!\n", argv[1]);
		PrintLastError();
		exit (-1);
	}

	/* INFO LEVEL 1 */

	EnumForms(printer, 1, (LPBYTE)buffer1, 0, &needed, &returned);
	if (GetLastError() != ERROR_INSUFFICIENT_BUFFER) {
		fprintf (stderr, "Error getting form information for [%s] using NULL buffer.\n", argv[1]);
		exit (-1);
	}

	if ( (buffer1 = (FORM_INFO_1*)malloc(needed)) == NULL ) {
		fprintf (stderr, "Unable to malloc memory for FORM_INFO_1 buffer!\n");
		exit (-1);
	}

	if (!EnumForms(printer, 1, (LPBYTE)buffer1, needed, &needed, &returned))
	{
		fprintf (stderr, "Error getting form information for [%s].\nSize of buffer = %d\n", 
			     argv[1], needed);
		exit (-1);
	}

	/* close the printer */
	if ( !ClosePrinter(printer) ) {
		PrintLastError();
		exit (-1);
	}


	/* open the printer */
	if (!OpenPrinter (argv[2], &printer, &defaults))
	{
		fprintf (stderr, "Unable to open %s!\n", argv[1]);
		PrintLastError();
		exit (-1);
	}

	for (i=0; i<returned; i++)
	{
		if ( buffer1[i].Flags == FORM_PRINTER ) {
			printf("Adding form \"%s\"\n", buffer1[i].pName );
			if ( !AddForm( printer, 1, (LPBYTE)&buffer1[i] ) )
				PrintLastError();
		}
	}	


	free (buffer1);


	/* close the printer */
	if ( !ClosePrinter(printer) ) {
		PrintLastError();
		exit (-1);
	}

	return 0;

}