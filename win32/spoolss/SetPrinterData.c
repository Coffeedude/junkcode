/******************************************************************
 * SPOOLSS regression testing code for Samba print servers
 * 
 *****************************************************************/

#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include "printlib.h"

#define START_JOB	0
#define MAX_JOBS	100

int main (int argc, char* argv[])
{
	
	HANDLE			printer;
	DWORD			type, pcbData;
	LPBYTE			pData = NULL;
	DWORD			i, status;


	if (argc < 3)
	{
		fprintf (stderr, "useage: %s <printername> <value name>\n", argv[0]);
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

	status=GetPrinterData(printer, argv[2], &type, pData, 0, &pcbData);
	if ((status != ERROR_SUCCESS) && (status != ERROR_MORE_DATA))
	{
		PrintLastError();
		ClosePrinter (printer);
		exit (-1);
	}

	if ((pData = (LPBYTE)malloc(pcbData)) == NULL)
	{
		fprintf (stderr, "Unable to malloc memory for Value Data!\n");
		exit (-1);
	}
	
	status = GetPrinterData(printer, argv[2], &type, pData, pcbData, &pcbData);
	if (status == ERROR_SUCCESS)
	{
		LPBYTE		ptr = pData;

		printf ("\tValue Name = %s\n", argv[2]);
		printf ("\tType = %d\n", type);
		printf ("\tData = 0x%x bytes\n", pcbData);
		i = 0;
		while (i < pcbData)
		{
			printf ("\t0x%x", *(ptr));
			*ptr = *ptr < 1;
			ptr++;
			if (i%4 == 3)
				printf ("\n");
			i++;
		}
		printf ("\n");
	}

	printf ("Shifting everything to the right one bit...\n\n");

	status = SetPrinterData(printer, argv[2], type, pData, pcbData);
	if (status == ERROR_SUCCESS)
	{
		printf ("\tValue Name = %s\n", argv[2]);
		printf ("\tType = %d\n", type);
		printf ("\tData = 0x%x bytes\n", pcbData);
		i = 0;
		while (i < pcbData)
		{
			printf ("\t0x%x", *(pData++));
			if (i%4 == 3)
				printf ("\n");
			i++;
		}
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
