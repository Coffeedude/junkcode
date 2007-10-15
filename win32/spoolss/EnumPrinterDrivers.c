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
	DWORD	needed, returned, i, j;
	PDRIVER_INFO_1	buffer1 = NULL;
	PDRIVER_INFO_2	buffer2 = NULL;
	PDRIVER_INFO_3	buffer3 = NULL;
	
	if (argc < 2)
	{
		fprintf (stderr, "useage: %s <servername>\n", argv[0]);
		exit (-1);
	}

	for (i=0; i<NUM_ARCH; i++)
	{
		printf ("\n\n[%s]\n", arch[i]);

		/* INFO LEVEL 1 */
		needed = returned = 0;
		EnumPrinterDrivers(argv[1], arch[i], 1, (LPBYTE)buffer1, 0, &needed, &returned);
		if (GetLastError() != ERROR_INSUFFICIENT_BUFFER)
		{
			if (needed != 0)
				fprintf (stderr, "Error EnumPrinterDrivers Info Level 1 for [%s] using NULL buffer.\n", argv[1]);
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
			if (!EnumPrinterDrivers(argv[1], arch[i], 1, (LPBYTE)buffer1, needed, &needed, &returned))
			{
				fprintf (stderr, "Error enumerating printer drivers Info Level 1 for [%s].\nSize of buffer = %d\n", 
						argv[1], needed);
			}

			printf ("Driver Info Level 1\n");
			for (j=0; j<returned; j++)
			{
				printf ("\tDriver Name\t= %s\n\n", buffer1[j].pName);
				printf ("\n");
			}
			free (buffer1);
		}

		/* INFO LEVEL 2 */
		needed = returned = 0;
		EnumPrinterDrivers(argv[1], arch[i], 2, (LPBYTE)buffer2, 0, &needed, &returned);
		if (GetLastError() != ERROR_INSUFFICIENT_BUFFER)
		{
			if (needed != 0)
				fprintf (stderr, "Error EnumPrinterDrivers Info Level 2 for [%s] using NULL buffer.\n", argv[1]);
			else
				printf ("Info level 2 returned no information\n");
		}
		else
		{
			if ((buffer2 = (LPBYTE)malloc(needed)) == NULL)
			{
				fprintf (stderr, "Unable to malloc memory for buffer!\n");
				exit (-1);
			}
			if (!EnumPrinterDrivers(argv[1], arch[i], 2, (LPBYTE)buffer2, needed, &needed, &returned))
			{
				fprintf (stderr, "Error enumerating printer drivers Info Level 2 for [%s].\nSize of buffer = %d\n", 
						argv[1], needed);
			}

			printf ("\nDriver Info Level 2\n");
			for (j=0; j<returned; j++)
			{
				printf ("\tDriver Name\t= %s\n",	buffer2[j].pName);
				printf ("\tEnvironment\t= %s\n",	buffer2[j].pEnvironment);
				printf ("\tVersion\t\t= %d\n",		buffer2[j].cVersion);
				printf ("\tDriver Path\t= %s\n",	buffer2[j].pDriverPath);
				printf ("\tData File\t= %s\n",		buffer2[j].pDataFile);
				printf ("\tConfig File\t= %s\n\n",	buffer2[j].pConfigFile);
				printf ("\n");
			}
			free (buffer2);
		}

		/* INFO LEVEL 3 */
		needed = returned = 0;
		EnumPrinterDrivers(argv[1], arch[i], 3, (LPBYTE)buffer3, 0, &needed, &returned);
		if (GetLastError() != ERROR_INSUFFICIENT_BUFFER)
		{
			if (needed != 0)
				fprintf (stderr, "Error EnumPrinterDrivers Info Level 3 for [%s] using NULL buffer.\n", argv[1]);
			else
				printf ("Info level 3 returned no information\n");
		}
		else
		{
			if ((buffer3 = (LPBYTE)malloc(needed)) == NULL)
			{
				fprintf (stderr, "Unable to malloc memory for buffer!\n");
				exit (-1);
			}
			if (!EnumPrinterDrivers(argv[1], arch[i], 3, (LPBYTE)buffer3, needed, &needed, &returned))
			{
				fprintf (stderr, "Error enumerating printer drivers Info Level 3 for [%s].\nSize of buffer = %d\n", 
						argv[1], needed);
			}

			printf ("\nDriver Info Level 3\n");
			for (j=0; j<returned; j++)
			{
				char*	ptr;

				printf ("\tDriver Name\t= %s\n",	buffer3[j].pName);
				printf ("\tEnvironment\t= %s\n",	buffer3[j].pEnvironment);
				printf ("\tVersion\t\t= %d\n",		buffer3[j].cVersion);
				printf ("\tDriver Path\t= %s\n",	buffer3[j].pDriverPath);
				printf ("\tData File\t= %s\n",		buffer3[j].pDataFile);
				printf ("\tConfig File\t= %s\n",	buffer3[j].pConfigFile);
				printf ("\tHelp Path\t= %s\n",		buffer3[j].pHelpFile);
				printf ("\tMonitor Name\t= %s\n",	buffer3[j].pMonitorName);
				printf ("\tData Type\t= %s\n",		buffer3[j].pDefaultDataType);
				ptr = (char*)buffer3->pDependentFiles;
				while (*ptr != '\0')
				{
					printf ("\tDependent Files\t= %s\n", ptr);
					for (;*ptr != '\0'; ptr++)
						/* printf ("%s\n", ptr); */
						;
					ptr++;
				}

				printf ("\n");
			}
			free (buffer3);
		}

	}


	return 0;

}

