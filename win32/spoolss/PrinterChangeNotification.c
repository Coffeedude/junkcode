/******************************************************************
 * SPOOLSS regression testing code for Samba print servers
 * 
 *****************************************************************/

#include <windows.h>
#include <stdio.h>
#include "printlib.h"

void PrintNotifyInfoData(PRINTER_NOTIFY_INFO_DATA *p)
{
	printf("\tType\t=0x%x\n", p->Type);
	printf("\tField\t=0x%x\n", p->Field);
	printf("\tId\t=0x%x\n", p->Id);
	printf("\tdata\t= %s\n", p->NotifyData.Data.pBuf);

	return;

}


int main (int argc, char* argv[])
{
	
	HANDLE	server, change;
	PRINTER_NOTIFY_INFO	*change_info = NULL;
	PRINTER_NOTIFY_OPTIONS watch, *pWatch = NULL;
	PRINTER_NOTIFY_OPTIONS_TYPE data;
	DWORD option = PRINTER_CHANGE_PRINTER_DRIVER;
	BOOL use_notify_options = TRUE;
	DWORD	condition;
	DWORD flags = PRINTER_NOTIFY_OPTIONS_REFRESH;



	if ((argc < 3) || (strcmp(argv[1], "-l") && strcmp(argv[1], "-t")))
	{
		fprintf (stderr, "usage: -l|-t %s <servername>\n", argv[0]);
		exit (-1);
	}

	if (strcmp(argv[1], "-l") == 0)
		use_notify_options = FALSE;


	if (!OpenPrinter (argv[2], &server, NULL))
	{
		PrintLastError();
		exit (-1);
	}
	else
	{
		printf ("Printer [%s] opened successfully.\n\n", argv[2]);
	}

	printf ("flags = PRINTER_CHANGE_PRINTER\n");
	watch.Version = 2;
	watch.Count = 1;
	watch.pTypes = &data;
	data.Type = PRINTER_NOTIFY_TYPE;
	data.Count = 1;
	data.pFields = &option;

	if (use_notify_options) {
		pWatch = &watch;
		option = PRINTER_NOTIFY_FIELD_DRIVER_NAME;
	}

	if ((change=FindFirstPrinterChangeNotification(server, option, 0, pWatch)) == INVALID_HANDLE_VALUE)
	{
		printf ("FindFirstPrinterChangeNotification failed : ");
		PrintLastError();
		ClosePrinter(server);
		exit (-1);
	}
	else
		printf ("FindFirstPrinterChangeNotification succeeded with flags [PRINTER_CHANGE_PRINTER_DRIVER]\n\n");

	if (pWatch) {
		printf ("Calling FindNextPrinterCangeNotification() with PRINTER_NOTIFY_OPTIONS_REFRESH\n");
		memset(&watch, 0x0, sizeof(watch));
		watch.Flags = PRINTER_NOTIFY_OPTIONS_REFRESH;
		watch.Version = 2;
		if (FindNextPrinterChangeNotification(change, &condition, pWatch, &change_info))
			printf ("Received change notification of [0x%x]\n", condition);
		else
			PrintLastError();

		if (change_info) {
			int i;

			printf("\tVersion\t=0x%x\n", change_info->Version);
			printf("\tFlags\t=0x%x\n", change_info->Flags);
			printf("\tCount\t=%d\n", change_info->Count);

			for (i=0; i<(int)change_info->Count; i++) {
				PrintNotifyInfoData(&change_info->aData[i]);
				printf("\n");
			}
		}

		FreePrinterNotifyInfo(change_info);
		printf ("\n");
	}

	do 
	{

		printf ("Waiting for Printer Change Notification...\n");

		if (WaitForSingleObject(change, INFINITE) != WAIT_OBJECT_0)
			continue;

		if (FindNextPrinterChangeNotification(change, &condition, NULL, &change_info))
			printf ("Received change notification of [0x%x]\n", condition);
		else
			PrintLastError();

		if (change_info) {
			int i;

			printf("\tVersion\t=0x%x\n", change_info->Version);
			printf("\tFlags\t=0x%x\n", change_info->Flags);
			printf("\tCount\t=%d\n", change_info->Count);

			for (i=0; i<(int)change_info->Count; i++) {
				PrintNotifyInfoData(&change_info->aData[i]);
				printf("\n");
			}
		}

		FreePrinterNotifyInfo(change_info);
		printf ("\n");
	} while (1);
	

	if (FindClosePrinterChangeNotification(change))
		printf ("FindClosePrinterChangeNotification call succeeded.\n");
	else
	{
		printf ("FindClosePrinterChangeNotification failed : ");
		PrintLastError();
	}
	printf ("\n");

	if (!ClosePrinter(server))
		PrintLastError();
	else
		printf ("Printer [%s] closed successfully.\n", argv[2]);

	return 0;

}