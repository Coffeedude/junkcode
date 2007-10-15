/******************************************************************
 * SPOOLSS regression testing code for Samba print servers
 * 
 *****************************************************************/

#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include "printlib.h"

#define START_JOB	0
#define MAX_JOBS	100

int main (int argc, char* argv[])
{
	
	HANDLE			printer;
	DWORD			returned, needed, i;
	PJOB_INFO_1		buffer1 = NULL;
	PJOB_INFO_2		buffer2 = NULL;
	DWORD			dJob = 0;


	if (argc < 3)
	{
		fprintf (stderr, "useage: %s <printername> <job id>\n", argv[0]);
		exit (-1);
	}
	dJob = atoi(argv[2]);

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

	/* INFO LEVEL 1 */
	EnumJobs(printer, START_JOB, MAX_JOBS, 1, (LPBYTE)buffer1, 0, &needed, &returned);
	if (GetLastError() != ERROR_INSUFFICIENT_BUFFER)
	{
		if (needed != 0)
		{
			fprintf (stderr, "Error getting job information for [%s] using NULL buffer.\n", argv[2]);
			exit (-1);
		}
		else
		{
			printf ("No jobs returned for [%s]\n", argv[1]);
			ClosePrinter (printer);
			exit (0);
		}
	}

	if ((buffer1 = (LPBYTE)malloc(needed)) == NULL)
	{
		fprintf (stderr, "Unable to malloc memory for JOB_INFO_1 buffer!\n");
		exit (-1);
	}
	if (!EnumJobs(printer, START_JOB, MAX_JOBS, 1, (LPBYTE)buffer1, needed, &needed, &returned))
	{
		fprintf (stderr, "Error getting form information for [%s].\nSize of buffer = %d\n", 
			     argv[2], needed);
		exit (-1);
	}

	printf ("Job Info Level 1:\n");
	for (i=0; i<returned; i++)
	{
		print_job_info_1 (&buffer1[i]);
	}
	printf ("\n");
	free (buffer1);


	/* INFO LEVEL 2 */
	GetJob(printer, dJob, 2, (LPBYTE)buffer2, 0, &needed);
	if (GetLastError() != ERROR_INSUFFICIENT_BUFFER)
	{
		if (needed != 0)
		{
			fprintf (stderr, "Error getting job information for [%s] using NULL buffer.\n", argv[2]);
			exit (-1);
		}
		else
		{
			printf ("No jobs returned for [%s]\n", argv[1]);
			ClosePrinter (printer);
			exit (0);
		}
	}

	if ((buffer2 = (LPBYTE)malloc(needed)) == NULL)
	{
		fprintf (stderr, "Unable to malloc memory for JOB_INFO_2 buffer!\n");
		exit (-1);
	}
	if (!GetJob(printer, dJob, 2, (LPBYTE)buffer2, needed, &needed))
	{
		fprintf (stderr, "Error getting form information for [%s].\nSize of buffer = %d\n", 
			     argv[2], needed);
		exit (-1);
	}

	printf ("Job Info Level 2:\n");
	for (i=0; i<returned; i++)
	{
		print_job_info_2 (&buffer2[i]);
	}
	printf ("\n");
	free (buffer2);


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
