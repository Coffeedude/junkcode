/******************************************************************
 * SPOOLSS regression testing code for Samba print servers
 * 
 *****************************************************************/

#include <windows.h>
#include <stdio.h>

#define NUM_ARCH	5
const LPTSTR arch [NUM_ARCH] = {	"Windows 4.0",
							"Windows NT x86",
							"Windows NT R4000",
							"Windows NT PowerPC",
							"Windows NT Alpha_AXP"	};

int main (int argc, char* argv[])
{
	
	DWORD			returned, needed, i;
	LPTSTR			buffer = NULL;

	if (argc < 2)
	{
		fprintf (stderr, "useage: %s <servername>\n", argv[0]);
		exit (-1);
	}

	printf ("Printer Driver Directory Info Level 1:\n");
	for (i=0; i<NUM_ARCH; i++)
	{
		GetPrinterDriverDirectory(argv[1], arch[i], 1, (LPBYTE)buffer, 0, &needed);
		if (GetLastError() != ERROR_INSUFFICIENT_BUFFER)
		{
			fprintf (stderr, "Error getting printer driver directory for [%s] using NULL buffer.\n", argv[1]);
			exit (-1);
		}

		if ((buffer = (LPBYTE)malloc(needed)) == NULL)
		{
			fprintf (stderr, "Unable to malloc memory for directory path!\n");
			exit (-1);
		}
		if (!GetPrinterDriverDirectory(argv[1], arch[i], 1, (LPBYTE)buffer, needed, &returned))
		{
			fprintf (stderr, "Error getting printer driver directory for [%s].\nSize of buffer = %d\n", 
				     argv[1], needed);
			exit (-1);
		}

		printf ("\t%s Path\t= %s\n\n", arch[i], buffer);
		free (buffer);
	}

	return 0;

}