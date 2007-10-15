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
	
	HANDLE			server;
	DWORD			type, pcbData;
	LPBYTE			pData = NULL;
	DWORD			i, status;


	if (argc < 3)
	{
		fprintf (stderr, "useage: %s <servername> <value name>\n", argv[0]);
		exit (-1);
	}

	/* open the server */
	if (!OpenPrinter (argv[1], &server, NULL))
		PrintLastError();
	else
		printf ("Server [%s] opened successfully.\n\n", argv[1]);

	status=GetPrinterDataEx(server, "PrinterDriverData", argv[2], &type, pData, 0, &pcbData);
	if ((status != ERROR_SUCCESS) && (status != ERROR_MORE_DATA))
	{
		PrintLastError();
	}
	else
	{
		if ((pData = (LPBYTE)malloc(pcbData)) == NULL)
		{
			fprintf (stderr, "Unable to malloc memory for Value Data!\n");
			exit (-1);
		}
	
	 	status = GetPrinterDataEx(server, "PrinterDriverData", argv[2], &type, pData, pcbData, &pcbData);
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
	
		printf ("\n");
	}


	/* close the server */
	if (!ClosePrinter(server))
		PrintLastError();
	else
		printf ("Server [%s] closed successfully.\n", argv[1]);

	return 0;

}
