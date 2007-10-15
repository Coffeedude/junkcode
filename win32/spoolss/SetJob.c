/******************************************************************
 * SPOOLSS regression testing code for Samba print servers
 * 
 *****************************************************************/

#include <windows.h>
#include <stdio.h>
#include "printlib.h"


int main (int argc, char* argv[])
{
	
	HANDLE			printer;
	DWORD			dwJobId = 0;
	DOCINFO			docinfo;
	LPVOID			lpMsgBuf;


	if (argc < 3)
	{
		fprintf (stderr, "useage: %s <printername> <Job ID>\n", argv[0]);
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

#if 0
	docinfo.lpszDocName = strdup (argv[2]);
	docinfo.lpszOutput  = NULL;
	docinfo.lpszDatatype = strdup ("RAW");
	docinfo.fwType = 0;
	docinfo.cbSize = sizeof (docinfo);

	dwJobId = StartDoc (NULL, &docinfo);
	printf ("JobId = %d\n", dwJobId);
	if (dwJobId < 1)
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
		printf ("StartDoc called successfully.  Job ID = %d\n", dwJobId);
	}
#endif

	dwJobId = atoi (argv[2]);

	printf ("command = JOB_CONTROL_PAUSE\n");
	if (SetJob(printer, dwJobId, 0, NULL, JOB_CONTROL_PAUSE))
	{
		printf ("Job Id [%d] paused successfully\n", dwJobId);
	}
	else
	{
		FormatMessage( FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | 
			FORMAT_MESSAGE_IGNORE_INSERTS, NULL, GetLastError(),
			MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), // Default language
			(LPTSTR) &lpMsgBuf, 0, NULL);
		printf ("ERROR : %s\n", (char*)lpMsgBuf);
		LocalFree (lpMsgBuf);
	}
	printf ("\n");

	printf ("command = JOB_CONTROL_RESUME\n");
	if (SetJob(printer, dwJobId, 0, NULL, JOB_CONTROL_RESUME))
	{
		printf ("Job Id [%d] resumed successfully\n", dwJobId);
	}
	else
	{
		FormatMessage( FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | 
			FORMAT_MESSAGE_IGNORE_INSERTS, NULL, GetLastError(),
			MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), // Default language
			(LPTSTR) &lpMsgBuf, 0, NULL);
		printf ("ERROR : %s\n", (char*)lpMsgBuf);
		LocalFree (lpMsgBuf);
	}
	printf ("\n");

	printf ("command = JOB_CONTROL_RESTART\n");
	if (SetJob(printer, dwJobId, 0, NULL, JOB_CONTROL_RESTART))
	{
		printf ("Job Id [%d] restarted successfully\n", dwJobId);
	}
	else
	{
		FormatMessage( FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | 
			FORMAT_MESSAGE_IGNORE_INSERTS, NULL, GetLastError(),
			MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), // Default language
			(LPTSTR) &lpMsgBuf, 0, NULL);
		printf ("ERROR : %s\n", (char*)lpMsgBuf);
		LocalFree (lpMsgBuf);
	}
	printf ("\n");


	printf ("command = JOB_CONTROL_DELETE\n");
	if (SetJob(printer, dwJobId, 0, NULL, JOB_CONTROL_DELETE))
	{
		printf ("Job Id [%d] deleted successfully\n", dwJobId);
	}
	else
	{
		FormatMessage( FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | 
			FORMAT_MESSAGE_IGNORE_INSERTS, NULL, GetLastError(),
			MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), // Default language
			(LPTSTR) &lpMsgBuf, 0, NULL);
		printf ("ERROR : %s\n", (char*)lpMsgBuf);
		LocalFree (lpMsgBuf);
	}
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
