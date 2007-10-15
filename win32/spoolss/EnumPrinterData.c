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
	DWORD			dwIndex, pcbValue, type, pcbData;
	DWORD			pcbMaxValue, pcbMaxData;
	LPTSTR			pValueName = NULL;
	LPBYTE			pData = NULL;
	DWORD			i, status;


	if (argc < 2)
	{
		fprintf (stderr, "useage: %s <printername>\n", argv[0]);
		exit (-1);
	}

	/* open the printer */
	if (!OpenPrinter (argv[1], &printer, NULL))
		PrintLastError();
	else
		printf ("Printer [%s] opened successfully.\n\n", argv[1]);

	dwIndex = 0;
	status = ERROR_SUCCESS;

	pcbMaxValue = pcbMaxData = 0;
	status=EnumPrinterData(printer, dwIndex, pValueName, 0, &pcbMaxValue, &type, pData, 0, &pcbMaxData);
	printf ("\tMax Size of Value = %d\n", pcbMaxValue);
	printf ("\tMax Size of Data  = %d\n", pcbMaxData);
	printf("\n");
	if (status != ERROR_SUCCESS)
	{
		PrintLastError();
		exit (-1);
	}

	if ((pValueName = (LPTSTR)malloc(pcbMaxValue)) == NULL)
	{
		fprintf (stderr, "Unable to malloc memory for Value Name!\n");
		exit (-1);
	}
	if ((pData = (LPBYTE)malloc(pcbMaxData)) == NULL)
	{
		fprintf (stderr, "Unable to malloc memory for Value Data!\n");
		exit (-1);
	}


	while (status != ERROR_NO_MORE_ITEMS)
	{
		pcbValue = pcbData = 0;
		status = EnumPrinterData(printer, dwIndex, pValueName, pcbMaxValue, &pcbValue, &type, pData, pcbMaxData, &pcbData);
		PrintLastError();
		if (status == ERROR_SUCCESS)
		{
			printf ("\tValue Name [size] = %s [%d]\n", pValueName, pcbValue);
			printf ("\tType = %d\n", type);
			printf ("\tData = 0x%x [%d] bytes\n", pcbData, pcbData);
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

		dwIndex++;
	}
	printf ("\n");

#if 0
	free (pValueName);
	free (pData);
#endif


	/* close the printer */
	if (!ClosePrinter(printer))
		PrintLastError();
	else
		printf ("Printer [%s] closed successfully.\n", argv[1]);

	return 0;

}
