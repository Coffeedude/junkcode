/******************************************************************
 * SPOOLSS regression testing code for Samba print servers
 * 
 *****************************************************************/

#include <windows.h>
#include <stdio.h>


int main (int argc, char* argv[])
{
	
	HANDLE	printer;
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

	if (!DeletePrinter(printer))
	{
		FormatMessage( FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | 
			FORMAT_MESSAGE_IGNORE_INSERTS, NULL, GetLastError(),
			MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), // Default language
			(LPTSTR) &lpMsgBuf, 0, NULL);
		printf ("ERROR : %s\n", (char*)lpMsgBuf);
		LocalFree (lpMsgBuf);

		if (!ClosePrinter(printer))
		{
			fprintf (stderr, "Error closing printer!\n");
			exit (-1);
		}
		else
		{
			printf ("Printer [%s] closed successfully.\n", argv[1]);
		}

	}
	else
	{
		printf ("Printer [%s] deleted successfully.\n", argv[1]);
	}

	return 0;

}