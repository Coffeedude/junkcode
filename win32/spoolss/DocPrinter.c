/******************************************************************
 * SPOOLSS regression testing code for Samba print servers
 * 
 *****************************************************************/

#include <windows.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "printlib.h"


int main (int argc, char* argv[])
{
	
	HANDLE	printer;
	DOC_INFO_1	buffer;
	LPVOID	lpMsgBuf;


	if (argc < 2)
	{
		fprintf (stderr, "useage: %s <printername>\n", argv[0]);
		exit (-1);
	}

	if (!OpenPrinter (argv[1], &printer, NULL))
	{
		fprintf (stderr, "Unable to open %s!\n", argv[1]);
		exit (-1);
	}
	else
	{
		printf ("Printer [%s] opened successfully.\n\n", argv[1]);
	}

	buffer.pDocName		= strdup("Testing StartDocPrinter");
	buffer.pDatatype	= strdup("RAW");
	buffer.pOutputFile	= NULL;

	printf ("Attempting to call StartDocPrinter() using DOC_INFO_1 :\n");
	print_doc_info_1 (&buffer);
	printf ("\n");

	if (StartDocPrinter (printer, 1, (LPBYTE)&buffer))
		printf ("Document spooled sucessfully\n");
	else
		PrintLastError();
	printf ("\n");


	printf ("Attempting to call EndDocPrinter\n");
	if (EndDocPrinter (printer))
		printf ("Document removed from spool sucessfully\n");
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