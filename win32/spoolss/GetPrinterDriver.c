/******************************************************************
 * SPOOLSS regression testing code for Samba print servers
 * 
 *****************************************************************/

#include <windows.h>
#include <stdio.h>
#include "printlib.h"

#define NUM_ARCH	5
const LPTSTR arch [NUM_ARCH] = {	"Windows 4.0",
							"Windows NT x86",
							"Windows NT R4000",
							"Windows NT PowerPC",
							"Windows NT Alpha_AXP"	};


int main (int argc, char* argv[])
{
	
	HANDLE			printer;
	DWORD			returned, needed, i;
	PDRIVER_INFO_1	buffer1 = NULL;
	PDRIVER_INFO_2	buffer2 = NULL;
	PDRIVER_INFO_3	buffer3 = NULL;
	PDRIVER_INFO_4	buffer4 = NULL;
	PDRIVER_INFO_5	buffer5 = NULL;
	PDRIVER_INFO_6	buffer6 = NULL;

	/* printf ("argc = %d\n", argc);*/

	if (argc < 2)
	{
		fprintf (stderr, "useage: %s <printername>\n", argv[0]);
		exit (-1);
	}

	/* open the printer */
	if (!OpenPrinter (argv[1], &printer, NULL))
	{
		PrintLastError();
		exit (-1);
	}
	else
	{
		printf ("Printer [%s] opened successfully.\n\n", argv[1]);
	}

	for (i=0; i<NUM_ARCH; i++)
	{	
		printf ("Environment = [%s]\n\n", arch[i]);

		/* INFO LEVEL 1 */
		GetPrinterDriver(printer, arch[i], 1, (LPBYTE)buffer1, 0, &needed);
		if (GetLastError() != ERROR_INSUFFICIENT_BUFFER)
		{
			fprintf (stderr, "Error getting printer driver information for [%s] using NULL buffer.\n", argv[1]);
			PrintLastError();
			continue;

		}

		if ((buffer1 = (LPBYTE)malloc(needed*1024)) == NULL)
		{
			fprintf (stderr, "Unable to malloc memory for DRIVER_INFO_1 buffer!\n");
			exit (-1);
		}
		needed *= 1024;
		if (!GetPrinterDriver(printer, arch[i], 1, (LPBYTE)buffer1, needed, &returned))
		{
			fprintf (stderr, "Error getting printer driver information for [%s].\nSize of buffer = %d\n", 
				     argv[1], needed);
			PrintLastError();
			continue;
		}

		printf ("Printer Driver Info Level 1:\n");
		print_driver_info_1 (buffer1);
		free (buffer1);

		/* INFO LEVEL 2 */
		GetPrinterDriver(printer, arch[i], 2, (LPBYTE)buffer2, 0, &needed);
		if (GetLastError() != ERROR_INSUFFICIENT_BUFFER)
		{
			fprintf (stderr, "Error getting printer driver information for [%s] using NULL buffer.\n", argv[1]);
			PrintLastError();
			continue;
		}

		if ((buffer2 = (LPBYTE)malloc(needed)) == NULL)
		{
			fprintf (stderr, "Unable to malloc memory for DRIVER_INFO_2 buffer!\n");
			exit (-1);
		}
		if (!GetPrinterDriver(printer, arch[i], 2, (LPBYTE)buffer2, needed, &returned))
		{
			fprintf (stderr, "Error getting printer driver information for [%s].\nSize of buffer = %d\n", 
				     argv[1], needed);
			PrintLastError();
			continue;
		}

		printf ("Printer Driver Info Level 2:\n");
		print_driver_info_2 (buffer2);
		free (buffer2);


		/* INFO LEVEL 3 */
		GetPrinterDriver(printer, arch[i], 3, (LPBYTE)buffer3, 0, &needed);
		if (GetLastError() != ERROR_INSUFFICIENT_BUFFER)
		{
			fprintf (stderr, "Error getting printer driver information for [%s] using NULL buffer.\n", argv[1]);
			PrintLastError();
			continue;
		}

		if ((buffer3 = (LPBYTE)malloc(needed)) == NULL)
		{
		fprintf (stderr, "Unable to malloc memory for DRIVER_INFO_3 buffer!\n");
			exit (-1);
		}
		if (!GetPrinterDriver(printer, arch[i], 3, (LPBYTE)buffer3, needed, &returned))
		{
			fprintf (stderr, "Error getting printer driver information for [%s].\nSize of buffer = %d\n", 
				     argv[1], needed);
			PrintLastError();
			continue;
		}

		printf ("Printer Driver Info Level 3:\n");
		print_driver_info_3 (buffer3);
		printf ("\n");
		free (buffer3);

		/* INFO LEVEL 6 */
		GetPrinterDriver(printer, arch[i], 6, (LPBYTE)buffer6, 0, &needed);
		if (GetLastError() != ERROR_INSUFFICIENT_BUFFER)
		{
			fprintf (stderr, "Error getting printer driver information for [%s] using NULL buffer.\n", argv[1]);
			PrintLastError();
			continue;
		}

		if ((buffer6 = (LPBYTE)malloc(needed)) == NULL)
		{
			fprintf (stderr, "Unable to malloc memory for DRIVER_INFO_6 buffer!\n");
			exit (-1);
		}
		if (!GetPrinterDriver(printer, arch[i], 6, (LPBYTE)buffer6, needed, &returned))
		{
			fprintf (stderr, "Error getting printer driver information for [%s].\nSize of buffer = %d\n", 
				     argv[1], needed);
			PrintLastError();
			continue;
		}

		printf ("Printer Driver Info Level 6:\n");
		print_driver_info_6 (buffer6);
		printf ("\n");
		free (buffer6);

	}


	/* close the printer */
	if (!ClosePrinter(printer))
	{
		PrintLastError();
	}
	else
	{
		printf ("Printer [%s] closed successfully.\n", argv[1]);
	}

	return 0;

}