/******************************************************************
 * SPOOLSS regression testing code for Samba print servers
 * 
 *****************************************************************/

#include <windows.h>
#include <stdio.h>
#include "printlib.h"


void TestEnumPrinters (LPTSTR servername, DWORD flags)
{
	DWORD	needed, returned, i;
	PPRINTER_INFO_1	buffer1 = NULL;
	PPRINTER_INFO_2	buffer2 = NULL;
	PPRINTER_INFO_4	buffer4 = NULL;
	PPRINTER_INFO_5	buffer5 = NULL;

	/* INFO LEVEL 1 */
	needed = returned = 0;
	EnumPrinters(flags, servername, 1, (LPBYTE)buffer1, 0, &needed, &returned);
	if (GetLastError() != ERROR_INSUFFICIENT_BUFFER)
	{
		if (needed != 0)
			fprintf (stderr, "Error EnumPrinters Info Level 1 for [%s] using NULL buffer.\n", servername);
		else
			printf ("Info level 1 returned no information\n");
	}
	else
	{
		if ((buffer1 = (LPBYTE)malloc(needed)) == NULL)
		{
			fprintf (stderr, "Unable to malloc memory for buffer!\n");
			return;
		}
		if (!EnumPrinters(flags, servername, 1, (LPBYTE)buffer1, needed, &needed, &returned))
		{
			fprintf (stderr, "Error enumerating printers Info Level 1 for [%s].\nSize of buffer = %d\n", 
					servername, needed);
		}

		printf ("Printer Info Level 1:\n");
		for (i=0; i<returned; i++)
		{
			print_printer_info_1 (&buffer1[i]);
			printf ("\n");
		}
		free (buffer1);
	}

	/* the following flags are only valid for info level 1 */
	if ((flags == PRINTER_ENUM_REMOTE) || (flags == PRINTER_ENUM_NETWORK))
		return;

	/* INFO LEVEL 2 */
	needed = returned = 0;
	EnumPrinters(flags, servername, 2, (LPBYTE)buffer2, 0, &needed, &returned);
	if (GetLastError() != ERROR_INSUFFICIENT_BUFFER)
	{
		if (needed != 0)
			fprintf (stderr, "Error EnumPrinters Info Level 2 for [%s] using NULL buffer.\n", servername);
		else
			printf ("Info level 2 returned no information\n");
	}
	else
	{
		if ((buffer2 = (LPBYTE)malloc(needed)) == NULL)
		{
			fprintf (stderr, "Unable to malloc memory for buffer!\n");
			return;
		}
		if (!EnumPrinters(flags, servername, 2, (LPBYTE)buffer2, needed, &needed, &returned))
		{
			fprintf (stderr, "Error enumerating printers Info Level 2 for [%s].\nSize of buffer = %d\n", 
					servername, needed);
		}

		printf ("Printer Info Level 2:\n");
		for (i=0; i<returned; i++)
		{
			print_printer_info_2 (&buffer2[i]);
			printf ("\n");
		}
		free (buffer2);
	}

	/* INFO LEVEL 5 */
	needed = returned = 0;
	EnumPrinters(flags, servername, 5, (LPBYTE)buffer5, 0, &needed, &returned);
	if (GetLastError() != ERROR_INSUFFICIENT_BUFFER)
	{
		if (needed != 0)
			fprintf (stderr, "Error EnumPrinters Info Level 5 for [%s] using NULL buffer.\n", servername);
		else
			printf ("Info level 5 returned no information\n");
	}
	else
	{
		if ((buffer5 = (LPBYTE)malloc(needed)) == NULL)
		{
			fprintf (stderr, "Unable to malloc memory for buffer!\n");
			return;
		}
		if (!EnumPrinters(flags, servername, 5, (LPBYTE)buffer5, needed, &needed, &returned))
		{
			fprintf (stderr, "Error enumerating printers Info Level 5 for [%s].\nSize of buffer = %d\n", 
					servername, needed);
		}

		printf ("Printer Info Level 5:\n");
		for (i=0; i<returned; i++)
		{
			print_printer_info_5 (&buffer5[i]);
			printf ("\n");
		}
		free (buffer5);
	}

}


int main (int argc, char* argv[])
{
	
	if (argc < 2)
	{
		fprintf (stderr, "useage: %s <servername>\n", argv[0]);
		exit (-1);
	}
#if 0
	printf ("\n\nFlags = PRINTER_ENUM_NAME\n");
	TestEnumPrinters (argv[1], PRINTER_ENUM_NAME);
	printf ("\n");

	printf ("\n\nFlags = PRINTER_ENUM_NAME | PRINTER_ENUM_SHARED\n");
	TestEnumPrinters (argv[1], PRINTER_ENUM_NAME | PRINTER_ENUM_SHARED);
	printf ("\n");
#endif

	printf ("\n\nFlags = PRINTER_ENUM_CONNECTIONS\n");
	TestEnumPrinters (argv[1], PRINTER_ENUM_CONNECTIONS);
	printf ("\n");

	printf ("\n\nFlags = PRINTER_ENUM_NETWORK\n");
	TestEnumPrinters (NULL, PRINTER_ENUM_NETWORK);
	printf ("\n");

	printf ("\n\nFlags = PRINTER_ENUM_REMOTE\n");
	TestEnumPrinters (NULL, PRINTER_ENUM_REMOTE);
	printf ("\n");


	return 0;

}

