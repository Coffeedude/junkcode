/******************************************************************
 * SPOOLSS regression testing code for Samba print servers
 * 
 *****************************************************************/

#include <windows.h>
#include <stdio.h>
#include "printlib.h"


int main (int argc, char* argv[])
{
	
	DWORD			returned, needed, i;
	PMONITOR_INFO_1	buffer1 = NULL;
	PMONITOR_INFO_2	buffer2 = NULL;

	if (argc < 2)
	{
		fprintf (stderr, "useage: %s <servername>\n", argv[0]);
		exit (-1);
	}

	/* INFO LEVEL 1 */
	needed = returned = 0;
	EnumMonitors(argv[1], 1, (LPBYTE)buffer1, 0, &needed, &returned);
	if (GetLastError() != ERROR_INSUFFICIENT_BUFFER)
	{
		fprintf (stderr, "Error getting monitor information for [%s] using NULL buffer.\n", argv[1]);
		exit (-1);
	}

	if ((buffer1 = (LPBYTE)malloc(needed)) == NULL)
	{
		fprintf (stderr, "Unable to malloc memory for MONITOR_INFO_1 buffer!\n");
		exit (-1);
	}
	if (!EnumMonitors(argv[1], 1, (LPBYTE)buffer1, needed, &needed, &returned))
	{
		fprintf (stderr, "Error getting monitor information for [%s].\nSize of buffer = %d\n", 
			     argv[1], needed);
		exit (-1);
	}

	printf ("Monitor Info Level 1:\n");
	for (i=0; i<returned; i++)
	{
		print_monitor_info_1(&buffer1[i]);
	}
	free (buffer1);

	/* INFO LEVEL 2 */
	needed = returned = 0;
	EnumMonitors(argv[1], 2, (LPBYTE)buffer2, 0, &needed, &returned);
	if (GetLastError() != ERROR_INSUFFICIENT_BUFFER)
	{
		fprintf (stderr, "Error getting monitor information for [%s] using NULL buffer.\n", argv[1], argv[2]);
		exit (-1);
	}

	if ((buffer2 = (LPBYTE)malloc(needed)) == NULL)
	{
		fprintf (stderr, "Unable to malloc memory for MONITOR_INFO_2 buffer!\n");
		exit (-1);
	}
	if (!EnumMonitors(argv[1], 2, (LPBYTE)buffer2, needed, &needed, &returned))
	{
		fprintf (stderr, "Error getting monitor information for [%s].\nSize of buffer = %d\n", 
			     argv[1], needed);
		exit (-1);
	}

	printf ("Monitor Info Level 2:\n");
	for (i=0; i<returned; i++)
	{
		print_monitor_info_2(&buffer2[i]);
	}
	free (buffer2);


	return 0;

}