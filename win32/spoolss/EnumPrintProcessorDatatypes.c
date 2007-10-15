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
	DWORD	needed, returned, j;
	PDATATYPES_INFO_1	buffer1 = NULL;
	
	if (argc < 3)
	{
		fprintf (stderr, "useage: %s <servername> <print processor>\n", argv[0]);
		exit (-1);
	}

	/* INFO LEVEL 1 */
	needed = returned = 0;
	EnumPrintProcessorDatatypes(argv[1], argv[2], 1, (LPBYTE)buffer1, 0, &needed, &returned);
	if (GetLastError() != ERROR_INSUFFICIENT_BUFFER)
	{
		if (needed != 0)
			fprintf (stderr, "Error EnumPrintProcessorDatetypes Info Level 1 for [%s] using NULL buffer.\n", argv[1]);
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
		if (!EnumPrintProcessorDatatypes(argv[1], argv[2], 1, (LPBYTE)buffer1, needed, &needed, &returned))
		{
			fprintf (stderr, "Error enumerating print processor Info Level 1 for [%s].\nSize of buffer = %d\n", 
					argv[1], needed);
		}
			printf ("Datatypes Info Level 1\n");
		for (j=0; j<returned; j++)
		{
			printf ("\tDatatype \t= %s\n", buffer1[j].pName);
		}
		free (buffer1);
	}



	return 0;

}

