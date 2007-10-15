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
	DWORD	needed, returned, i, j;
	PPRINTPROCESSOR_INFO_1  buffer1 = NULL;
	
	if (argc < 2)
	{
		fprintf (stderr, "useage: %s <servername>\n", argv[0]);
		exit (-1);
	}

	for (i=0; i<NUM_ARCH; i++)
	{
		printf ("[%s]\n", arch[i]);

		needed = 0;
		EnumPrintProcessors(argv[1], arch[i], 1, (LPBYTE)buffer1, 0, &needed, &returned);
		if (GetLastError() != ERROR_INSUFFICIENT_BUFFER)
		{
			fprintf (stderr, "Error EnumPrintProcessors Info Level 1 for [%s] using NULL buffer.\n", argv[1]);
			continue;
		}

		if ((buffer1 = (LPBYTE)malloc(needed)) == NULL)
		{
			fprintf (stderr, "Unable to malloc memory for buffer!\n");
			exit (-1);
		}
		if (!EnumPrintProcessors(argv[1], arch[i], 1, (LPBYTE)buffer1, needed, &needed, &returned))
		{
			fprintf (stderr, "Error getting print processor information for [%s].\nSize of buffer = %d\n", 
					argv[1], needed);
		}
		printf ("Print Processor Info 1:\n");
		for (j=0; j<returned; j++)
		{
			print_printprocessor_info_1 (&buffer1[j]);
			printf ("\n");
		}
		free (buffer1);
	}


	return 0;

}

