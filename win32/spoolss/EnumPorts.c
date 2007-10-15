/******************************************************************
 * SPOOLSS regression testing code for Samba print servers
 * 
 *****************************************************************/

#include <windows.h>
#include <stdio.h>


int main (int argc, char* argv[])
{
	
	DWORD			returned, needed, i;
	PPORT_INFO_1	buffer1 = NULL;
	PPORT_INFO_2	buffer2 = NULL;

	if (argc < 2)
	{
		fprintf (stderr, "useage: %s <servername>\n", argv[0]);
		exit (-1);
	}

	/* INFO LEVEL 1 */
	needed = returned = 0;
	EnumPorts(argv[1], 1, (LPBYTE)buffer1, 0, &needed, &returned);
	if (GetLastError() != ERROR_INSUFFICIENT_BUFFER)
	{
		fprintf (stderr, "Error getting port information for [%s] using NULL buffer.\n", argv[1]);
		exit (-1);
	}

	if ((buffer1 = (LPBYTE)malloc(needed)) == NULL)
	{
		fprintf (stderr, "Unable to malloc memory for PORT_INFO_1 buffer!\n");
		exit (-1);
	}
	if (!EnumPorts(argv[1], 1, (LPBYTE)buffer1, needed, &needed, &returned))
	{
		fprintf (stderr, "Error getting port information for [%s].\nSize of buffer = %d\n", 
			     argv[1], needed);
		exit (-1);
	}

	printf ("Port Info Level 1:\n");
	for (i=0; i<returned; i++)
	{
		printf ("\tPort Name\t= %s\n", buffer1[i].pName);
		printf ("\n");
	}
	free (buffer1);

	/* INFO LEVEL 2 */
	needed = returned = 0;
	EnumPorts(argv[1], 2, (LPBYTE)buffer2, 0, &needed, &returned);
	if (GetLastError() != ERROR_INSUFFICIENT_BUFFER)
	{
		fprintf (stderr, "Error getting port information for [%s] using NULL buffer.\n", argv[1], argv[2]);
		exit (-1);
	}

	if ((buffer2 = (LPBYTE)malloc(needed)) == NULL)
	{
		fprintf (stderr, "Unable to malloc memory for PORT_INFO_2 buffer!\n");
		exit (-1);
	}
	if (!EnumPorts(argv[1], 2, (LPBYTE)buffer2, needed, &needed, &returned))
	{
		fprintf (stderr, "Error getting port information for [%s].\nSize of buffer = %d\n", 
			     argv[1], needed);
		exit (-1);
	}

	printf ("Port Info Level 2:\n");
	for (i=0; i<returned; i++)
	{
		printf ("\tPort Name\t= %s\n",		buffer2[i].pPortName);
		printf ("\tMonitor Name\t= %s\n",	buffer2[i].pMonitorName);
		printf ("\tDescription\t= %s\n",	buffer2[i].pDescription);
		printf ("\tType\t\t= 0x%x\n",		buffer2[i].fPortType);
		printf ("\tReserved\t= %d\n",		buffer2[i].Reserved);
		printf ("\n");
	}
	free (buffer2);


	return 0;

}