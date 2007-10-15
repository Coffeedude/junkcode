/******************************************************************
 * SPOOLSS regression testing code for Samba print servers
 * 
 *****************************************************************/

#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "printlib.h"


int main (int argc, char* argv[])
{
	PRINTER_INFO_2		buffer;
	HANDLE				printer;
	LPVOID				lpMsgBuf;
	
	if (argc < 5)
	{
		fprintf (stderr, "useage: %s <servername> <printername> <sharename> <port name>\n", argv[0]);
		exit (-1);
	}

	memset(&buffer, 0 , sizeof(buffer));

	buffer.pServerName	= strdup (argv[1]);
	buffer.pPrinterName = strdup (argv[2]);
	buffer.pShareName	= strdup (argv[3]);
	buffer.pPortName	= strdup (argv[4]);
	buffer.pDriverName	= strdup ("Apple LaserWriter II NTX v51.8");
	buffer.pPrintProcessor	= strdup ("winprint");
	buffer.pDatatype	= strdup ("RAW");
	buffer.Attributes	= PRINTER_ATTRIBUTE_SHARED;
	buffer.pSecurityDescriptor = NULL;


	printf ("Attempting to add printer [%s] on server [%s]\n", 
		buffer.pPrinterName, buffer.pServerName);
	printf ("\nPrinter Info 1:\n");
	print_printer_info_2 (&buffer);
	printf ("\n");

	printer = AddPrinter (buffer.pServerName, 2, (LPBYTE)(&buffer));
	if ( printer == NULL)
	{
		FormatMessage( FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | 
			FORMAT_MESSAGE_IGNORE_INSERTS, NULL, GetLastError(),
			MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), // Default language
			(LPTSTR) &lpMsgBuf, 0, NULL);
		printf ("ERROR : %s\n", (char*)lpMsgBuf);
		LocalFree (lpMsgBuf);
	}
	else
	{
		printf ("Printer [%s] added successfully on server [%s]!\n",
			buffer.pPrinterName, buffer.pServerName);
		if (!ClosePrinter (printer))
			fprintf (stderr, "Error closing printer handle!\n");
		else
			printf ("Printer handle closed successfully.\n");
	}

	return 0;

}

