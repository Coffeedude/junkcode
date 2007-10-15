/******************************************************************
 * SPOOLSS regression testing code for Samba print servers
 * 
 *****************************************************************/

#include <windows.h>
#include <stdio.h>
#include "printlib.h"

int main (int argc, char* argv[])
{
	DWORD	needed, returned;
	HANDLE	printer;
	PPRINTER_INFO_1	buffer1 = NULL;
	PPRINTER_INFO_2	buffer2 = NULL;
	PPRINTER_INFO_5	buffer5 = NULL;
	PPRINTER_INFO_7	buffer7 = NULL;
	
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
	needed = returned = 0;
	GetPrinter(printer, 1, (LPBYTE)buffer1, 0, &needed);
	if (GetLastError() != ERROR_INSUFFICIENT_BUFFER)
	{
		if (needed != 0)
			fprintf (stderr, "Error GetPrinter Info Level 1 for [%s] using NULL buffer.\n", argv[1]);
		else
			printf ("Info level 1 returned no information\n");
	}
	else
	{
		if ((buffer1 = (LPBYTE)malloc(needed)) == NULL)
		{
			fprintf (stderr, "Unable to malloc memory for buffer!\n");
			exit (-1);
		}
		if (!GetPrinter(printer, 1, (LPBYTE)buffer1, needed, &returned))
		{
			fprintf (stderr, "Error enumerating printers Info Level 1 for [%s].\nSize of buffer = %d\n", 
					argv[1], needed);
		}

		printf ("Printer Info Level 1:\n");
		print_printer_info_1(buffer1);
		printf ("\n");
		free (buffer1);
	}

	/* INFO LEVEL 2 */
	needed = returned = 0;
	GetPrinter(printer, 2, (LPBYTE)buffer2, 0, &needed);
	if (GetLastError() != ERROR_INSUFFICIENT_BUFFER)
	{
		if (needed != 0)
			fprintf (stderr, "Error GetPrinter Info Level 2 for [%s] using NULL buffer.\n", argv[1]);
		else
			printf ("Info level 2 returned no information\n");
	}
	else
	{
		if ((buffer2 = (LPBYTE)malloc(needed*2)) == NULL)
		{
			fprintf (stderr, "Unable to malloc memory for buffer!\n");
			exit (-1);
		}

		printf("Pass one\n");
		if (!GetPrinter(printer, 2, (LPBYTE)buffer2, needed, &returned))
		{
			fprintf (stderr, "Error getting printers Info Level 2 for [%s].\nSize of buffer = 0x%x [0x%x]\n", 
					argv[1], needed, returned);
			PrintLastError();

			needed *= 2;
			returned = 0;
			printf("Pass Two\n");
			if (!GetPrinter(printer, 2, (LPBYTE)buffer2, needed, &returned))
			{
				fprintf (stderr, "Error getting printers Info Level 2 for [%s].\nSize of buffer = 0x%x [0x%x]\n", 
						argv[1], needed, returned);	
				PrintLastError();
			}
		}

		printf ("Printer Info Level 2:\n");
		printf ("Returned = 0x%x\n", returned);
		print_printer_info_2(buffer2);
		printf ("\n");
		free (buffer2);
	}

	/* INFO LEVEL 5 */
	GetPrinter(printer, 5, (LPBYTE)buffer5, 0, &needed);
	if (GetLastError() != ERROR_INSUFFICIENT_BUFFER)
	{
		if (needed != 0)
			fprintf (stderr, "Error GetPrinter Info Level 5 for [%s] using NULL buffer.\n", argv[1]);
		else
			printf ("Info level 5 returned no information\n");
	}
	else
	{
		if ((buffer5 = (LPBYTE)malloc(needed)) == NULL)
		{
			fprintf (stderr, "Unable to malloc memory for buffer!\n");
			exit (-1);
		}
		if (!GetPrinter(printer, 5, (LPBYTE)buffer5, needed, &needed))
		{
			fprintf (stderr, "Error getting printers Info Level 5 for [%s].\nSize of buffer = %d\n", 
					argv[1], needed);
		}

		printf ("Printer Info Level 5:\n");
		print_printer_info_5(buffer5);
		printf ("\n");
		free (buffer5);
	}

	/* INFO LEVEL 7 */
	GetPrinter(printer, 7, (LPBYTE)buffer7, 0, &needed);
	if (GetLastError() != ERROR_INSUFFICIENT_BUFFER)
	{
		if (needed != 0)
			fprintf (stderr, "Error GetPrinter Info Level 7 for [%s] using NULL buffer.\n", argv[1]);
		else
			printf ("Info level 7 returned no information\n");
	}
	else
	{
		if ((buffer7 = (LPBYTE)malloc(needed)) == NULL)
		{
			fprintf (stderr, "Unable to malloc memory for buffer!\n");
			exit (-1);
		}
		if (!GetPrinter(printer, 7, (LPBYTE)buffer7, needed, &needed))
		{
			fprintf (stderr, "Error getting printers Info Level 7 for [%s].\nSize of buffer = %d\n", 
					argv[1], needed);
		}

		printf ("Printer Info Level 7:\n");
		printf ("flags = %d\n", buffer7->dwAction);
		printf ("\n");
		free (buffer7);
	}

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

