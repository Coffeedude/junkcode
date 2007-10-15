#include <windows.h>
#include <stdio.h>
#include "printlib.h"

#define DetectNull(p) (p?p:"NULL")

void print_devmode (DEVMODE *pDevModeIn)
{
	if (pDevModeIn == NULL)
	{
		printf ("\tDevice Mode\t= (null)\n");
		return;
	}

	printf( "\tDEVMODE:----------\n" );
	printf( "\tDeviceName:      [%s]\n", DetectNull( pDevModeIn->dmDeviceName ) );
	printf( "\tSpecVersion:     %d\n",   pDevModeIn->dmSpecVersion );
	printf( "\tDriverVersion:   %d\n",   pDevModeIn->dmDriverVersion );
	printf( "\tSize:            %d\n",   pDevModeIn->dmSize );
	printf( "\tDriverExtra:     %d\n",   pDevModeIn->dmDriverExtra );
	printf( "\tFields:          0x%x\n", pDevModeIn->dmFields );

	if ( pDevModeIn->dmFields & DM_ORIENTATION )
 		printf( "\tOrientation:     %d\n", pDevModeIn->dmOrientation );
	if ( pDevModeIn->dmFields & DM_PAPERSIZE )
 		printf( "\tPaperSize:       %d\n", pDevModeIn->dmPaperSize);
	if ( pDevModeIn->dmFields & DM_PAPERLENGTH )
 		printf( "\tPaperLength:     %d\n", pDevModeIn->dmPaperLength );
	if ( pDevModeIn->dmFields & DM_PAPERWIDTH )
 		printf( "\tPaperWidth:      %d\n", pDevModeIn->dmPaperWidth );
//	if ( pDevModeIn->dmFields & DM_POSITION )
//		printf( "\tPosition:        %d\n", pDevModeIn->dmPosition );
	if ( pDevModeIn->dmFields & DM_SCALE )
 		printf( "\tScale:           %d\n", pDevModeIn->dmScale );
	if ( pDevModeIn->dmFields & DM_COPIES )
 		printf( "\tCopies:          %d\n", pDevModeIn->dmCopies );
	if ( pDevModeIn->dmFields & DM_DEFAULTSOURCE )
 		printf( "\tDefaultSource:   %d\n", pDevModeIn->dmDefaultSource );
	if ( pDevModeIn->dmFields & DM_PRINTQUALITY )
 		printf( "\tPrintQuality:    %d\n", pDevModeIn->dmPrintQuality );
	if ( pDevModeIn->dmFields & DM_COLOR )
 		printf( "\tColor:           %d\n", pDevModeIn->dmColor );
	if ( pDevModeIn->dmFields & DM_DUPLEX )
 		printf( "\tDuplex:          %d\n", pDevModeIn->dmDuplex );
	if ( pDevModeIn->dmFields & DM_YRESOLUTION )
 		printf( "\tYResolution:     %d\n", pDevModeIn->dmYResolution );
	if ( pDevModeIn->dmFields & DM_TTOPTION )
 		printf( "\tTTOption:        %d\n", pDevModeIn->dmTTOption );
	if ( pDevModeIn->dmFields & DM_COLLATE )
 		printf( "\tCollate:         %d\n", pDevModeIn->dmCollate );
	if ( pDevModeIn->dmFields & DM_FORMNAME )
 		printf( "\tForm:            [%s]\n", pDevModeIn->dmFormName );
	if ( pDevModeIn->dmFields & DM_LOGPIXELS )
 		printf( "\tLogPixels:       %d\n", pDevModeIn->dmLogPixels );
	if ( pDevModeIn->dmFields & DM_BITSPERPEL )
 		printf( "\tBitsPerPel:      %d\n", pDevModeIn->dmBitsPerPel );
	if ( pDevModeIn->dmFields & DM_PELSWIDTH )
 		printf( "\tPelsWidth:       %d\n", pDevModeIn->dmPelsWidth );
	if ( pDevModeIn->dmFields & DM_PELSHEIGHT )
 		printf( "\tPelsHeight:      %d\n", pDevModeIn->dmPelsHeight );
	if ( pDevModeIn->dmFields & DM_DISPLAYFLAGS )
 		printf( "\tDisplayFlags:    %d\n", pDevModeIn->dmDisplayFlags );
//	if ( pDevModeIn->dmFields & DM_NUP )
//		printf( "\tNup:             %d\n", pDevModeIn->dmNup );
	if ( pDevModeIn->dmFields & DM_DISPLAYFREQUENCY )
 		printf( "\tDisplayFrequency:%d\n", pDevModeIn->dmDisplayFrequency );
	if ( pDevModeIn->dmFields & DM_ICMMETHOD )
 		printf( "\tICMMethod:       %d\n", pDevModeIn->dmICMMethod );
	if ( pDevModeIn->dmFields & DM_ICMINTENT )
 		printf( "\tICMIntent:       %d\n", pDevModeIn->dmICMIntent );
	if ( pDevModeIn->dmFields & DM_MEDIATYPE )
 		printf( "\tMediaType:       %d\n", pDevModeIn->dmMediaType );
	if ( pDevModeIn->dmFields & DM_DITHERTYPE )
 		printf( "\tDitherType:      %d\n", pDevModeIn->dmDitherType );
//	if ( pDevModeIn->dmFields & DM_PANNINGWIDTH )
//		printf( "\tPanningWidth:    %d\n", pDevModeIn->dmPanningWidth );
//	if ( pDevModeIn->dmFields & DM_PANNINGHEIGHT )
//		printf( "\tPanningHeight:   %d\n", pDevModeIn->dmPanningHeight );

#if 0
	if ( bForceIn )
	{
		printf( "  DEVMODE\n" );
		Dump( (BYTE*)pDevModeIn, sizeof(DEVMODE), LEADER );

		if ( pDevModeIn->dmDriverExtra )
		{
			printf( "  DriverExtra\n" );
			Dump( (BYTE*)pDevModeIn + sizeof(DEVMODE), pDevModeIn->dmDriverExtra, LEADER );
		}
	}
#endif

	return;
}

void print_acl (const char* str, ACL *acl)
{
	printf ("%s\n", str);
	if (acl == NULL)
		return;

	printf ("\t\tACL Revision \t\t 0x%x\n", acl->AclRevision);
	printf ("\t\tSbz1\t\t 0x%x\n", acl->Sbz1);
	printf ("\t\tSbz2\t\t 0x%x\n", acl->Sbz2);
	printf ("\t\tACL Size\t\t 0x%x\n", acl->AclSize);
	printf ("\t\tACL Count\t\t 0x%x\n", acl->AceCount);


	return;
}

void print_sid (const char* str, SID *sid)
{
	DWORD	i;

	printf ("%s\n", str);
	printf ("0x%x\n", sid);
	if (sid == NULL)
		return;
	printf ("\t\tRevision\t\t0x%x\n", sid->Revision);
	printf ("\t\tSubAuthorityCount\t0x%x\n", sid->SubAuthorityCount);
	printf ("\t\tSubAuthority\n\t");
	while (i < sid->SubAuthorityCount)
	{
		printf ("\t0x%x", sid->SubAuthority[i]);
		if (i%4 == 3)
			printf ("\n\t");
		i++;
	}

	return;
}

void print_secdesc (SECURITY_DESCRIPTOR *secdesc)
{
	if (secdesc == NULL)
	{
		printf ("\tSecurity Descriptor\t= (null)\n");
		return;
	}

	printf ("\tRevision\t= 0x%x\n", secdesc->Revision);
	printf ("\tSbz1\t\t= 0x%x\n", secdesc->Sbz1);
#if 0
	print_sid ("\tOwner\t\t= ", secdesc->Owner);
	print_sid ("\tGroup\t\t= ",secdesc->Group);
	print_acl ("\tSacl\t\t= ", secdesc->Sacl);
	print_acl ("\tDacl\t\t= ", secdesc->Dacl);
#endif
	return;
}

void PrintLastError()
{
	LPVOID		lpMsgBuf;
	DWORD		status;

	status = GetLastError();

	FormatMessage( FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | 
		FORMAT_MESSAGE_IGNORE_INSERTS, NULL, GetLastError(),
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), // Default language
		(LPTSTR) &lpMsgBuf, 0, NULL);
	printf ("ERROR  [0x%x] : %s\n", status, (char*)lpMsgBuf);
	LocalFree (lpMsgBuf);

		return;
}


void print_job_info_1(PJOB_INFO_1 info)
{
	printf ("\tJob ID\t\t= %d\n",		info->JobId);
	printf ("\tPrinter Name\t= %s\n",	info->pPrinterName);
	printf ("\tMachine Name\t= %s\n",	info->pMachineName);
	printf ("\tUser Name\t= %s\n",		info->pUserName);
	printf ("\tDocument\t= %s\n",		info->pDocument);
	printf ("\tDatatype\t= %s\n",		info->pDatatype);
	printf ("\tStatus\t\t= %s\n",			info->pStatus);
	printf ("\tStatus\t= %d\n",		info->Status);
	printf ("\tPriority\t= %d\n",		info->Priority);
	printf ("\tPosition\t= %d\n",		info->Position);
	printf ("\tTotal Pages\t= %d\n",	info->TotalPages);
	printf ("\tPages Printed\t= %d\n",	info->PagesPrinted);
	printf ("\tSubmitted (DD:MM:YY HH:MM:SS)\t= %d:%d:%d %d:%d:%d UTC\n",
			info->Submitted.wDay, info->Submitted.wMonth,
			info->Submitted.wYear, info->Submitted.wHour,
			info->Submitted.wMinute, info->Submitted.wSecond);

	return;
}

void print_job_info_2(PJOB_INFO_2 info)
{
	printf ("\tJob ID\t\t= %d\n",		info->JobId);
	printf ("\tPrinter Name\t= %s\n",	info->pPrinterName);
	printf ("\tMachine Name\t= %s\n",	info->pMachineName);
	printf ("\tUser Name\t= %s\n",		info->pUserName);
	printf ("\tDocument\t= %s\n",		info->pDocument);
	printf ("\tDatatype\t= %s\n",		info->pDatatype);
	printf ("\tNotify Name\t= %s\n",	info->pNotifyName);
	printf ("\tPrint Processor\t= %s\n",	info->pPrintProcessor);
	printf ("\tParameters\t= %s\n",		info->pParameters);
	printf ("\tDriver Name\t= %s\n",		info->pDriverName);
	printf ("\tStatus\t\t= %s\n",		info->pStatus);
	printf ("\tStatus\t\t= %d\n",		info->Status);
	printf ("\tPriority\t= %d\n",		info->Priority);
	printf ("\tPosition\t= %d\n",		info->Position);
	printf ("\tTotal Pages\t= %d\n",	info->TotalPages);
	printf ("\tPages Printed\t= %d\n",	info->PagesPrinted);
	printf ("\tStart Time\t= %d\n",		info->StartTime);
	printf ("\tUntil Time\t= %d\n",		info->UntilTime);
	printf ("\tTime\t\t= %d\n",		info->Time);
	printf ("\tSize\t\t= %d\n",			info->Size);
	printf ("\tSubmitted (DD:MM:YY HH:MM:SS)\t= %d:%d:%d %d:%d:%d UTC\n",
			info->Submitted.wDay, info->Submitted.wMonth,
			info->Submitted.wYear, info->Submitted.wHour,
			info->Submitted.wMinute, info->Submitted.wSecond);
	printf ("\tDevice Mode Information\n");
	printf ("\t-----------------------\n");
	print_devmode (info->pDevMode);
	printf ("\tSecurity Descriptor Information\n");
	printf ("\t-------------------------------\n");
	print_secdesc (info->pSecurityDescriptor);

	return;
}

void print_job_info_3(PJOB_INFO_3 info)
{
	printf ("\tJob ID\t\t= %d\n",		info->JobId);
	printf ("\tJob ID Next Job\t= %d\n",	info->NextJobId);
	printf ("\tReserved (must be 0)\t= %d\n",	info->Reserved);

	return;
}

void print_monitor_info_1 (PMONITOR_INFO_1 info)
{
		printf ("\tMonitor Name\t= %s\n",	info->pName);

		return;
}

void print_monitor_info_2 (PMONITOR_INFO_2 info)
{
		printf ("\tMonitor Name\t= %s\n",	info->pName);
		printf ("\tEnvironment\t= %s\n",	info->pEnvironment);
		printf ("\tDLL Name\t= %s\n",	info->pDLLName);

		return;
}

void print_form_info_1 (PFORM_INFO_1 info)
{

		printf ("\tForm Name\t= %s\n",	info->pName);
		printf ("\tFlags\t\t= 0x%x\n",		info->Flags);
		printf ("\tSize\t\t= %d x %d\n", info->Size.cx, info->Size.cy);
		printf ("\tRectangle\t= [left]%d [right]%d [top]%d [bottom]%d\n", 
				info->ImageableArea.left, info->ImageableArea.right,
				info->ImageableArea.top,	info->ImageableArea.bottom);

		return;
}

void print_printer_info_1 (PPRINTER_INFO_1 info)
{
	
			printf ("\tPrinter Name\t= %s\n", info->pName);
			printf ("\tDescription\t= %s\n", info->pDescription);
			printf ("\tComment\t\t= %s\n", info->pComment);
			printf ("\tFlags\t\t= 0x%x\n", info->Flags);

			return;
}

void print_printer_info_2 (PPRINTER_INFO_2 info)
{
			printf ("\tServer Name\t\t= %s\n",	info->pServerName);
			printf ("\tPrinter Name\t\t= %s\n",	info->pPrinterName);
			printf ("\tPort Name\t\t= %s\n",		info->pPortName);
			printf ("\tShare Name\t\t= %s\n",		info->pShareName);
			printf ("\tDriver Name\t\t= %s\n",	info->pDriverName);
			printf ("\tComment\t\t\t= %s\n",		info->pComment);
			printf ("\tLocation\t\t= %s\n",		info->pLocation);
			printf ("\tSeparator File\t\t= %s\n",		info->pSepFile);
			printf ("\tDefault Data Type\t= %s\n",	info->pDatatype);
			printf ("\tPrint Processor\t\t= %s\n",	info->pPrintProcessor);
			printf ("\tParameters\t\t= %s\n",			info->pParameters);

			printf ("\tAttributes\t\t= 0x%x\n",		info->Attributes);
			printf ("\tPriority\t\t= 0x%x\n",		info->Priority);
			printf ("\tDefault Priority\t= 0x%x\n",		info->DefaultPriority);
			printf ("\tStart Time\t\t= 0x%x\n",		info->StartTime);
			printf ("\tUntil Time\t\t= 0x%x\n",		info->UntilTime);
			printf ("\tStatus\t\t\t= 0x%x\n",			info->Status);
			printf ("\tcJobs\t\t\t= 0x%x\n",			info->cJobs);
			printf ("\tAverage PPM\t\t= 0x%x\n",		info->AveragePPM);

			printf ("\tDevice Mode Information\n");
			printf ("\t-----------------------\n");
			print_devmode (info->pDevMode);
#if 0
			printf ("\tSecurity Descriptor Information\n");
			printf ("\t-------------------------------\n");
			print_secdesc (info->pSecurityDescriptor);
#endif 
			return;
}

void print_printer_info_5 (PPRINTER_INFO_5 info)
{

			printf ("\tPrinter Name\t\t\t= %s\n",	info->pPrinterName);
			printf ("\tPort Name\t\t\t= %s\n",		info->pPortName);
			printf ("\tAttributes\t\t\t= 0x%x\n",		info->Attributes);
			printf ("\tDev NotSelect Timeout\t= 0x%x\n",	info->DeviceNotSelectedTimeout);
			printf ("\tTX RetryTimeout\t\t= 0x%x\n",		info->TransmissionRetryTimeout);

			return;

}

void print_printprocessor_info_1 (PPRINTPROCESSOR_INFO_1 info)
{

	printf ("\tPrint Processor Name\t= %s\n", info->pName);

	return;
}

void print_driver_info_1 (PDRIVER_INFO_1 info)
{
	printf ("\tDriver Name\t= %s\n\n", info->pName);

	return;
}

void print_driver_info_2 (PDRIVER_INFO_2 info)
{
		printf ("\tDriver Name\t= %s\n",	info->pName);
		printf ("\tEnvironment\t= %s\n",	info->pEnvironment);
		printf ("\tVersion\t\t= %d\n",		info->cVersion);
		printf ("\tDriver Path\t= %s\n",	info->pDriverPath);
		printf ("\tData File\t= %s\n",		info->pDataFile);
		printf ("\tConfig File\t= %s\n\n",	info->pConfigFile);

		return;
}

void print_driver_info_3 (PDRIVER_INFO_3 info)
{
		char		*ptr = NULL;

		printf ("\tDriver Name\t= %s\n",	info->pName);
		printf ("\tEnvironment\t= %s\n",	info->pEnvironment);
		printf ("\tVersion\t\t= %d\n",		info->cVersion);
		printf ("\tDriver Path\t= %s\n",	info->pDriverPath);
		printf ("\tData File\t= %s\n",		info->pDataFile);
		printf ("\tConfig File\t= %s\n",	info->pConfigFile);
		printf ("\tHelp Path\t= %s\n",		info->pHelpFile);
		printf ("\tMonitor Name\t= %s\n",	info->pMonitorName);
		printf ("\tData Type\t= %s\n",		info->pDefaultDataType);
		ptr = (char*)info->pDependentFiles;
		while ((ptr != NULL) && (*ptr != '\0'))
		{
			printf ("\tDependent Files\t= %s\n", ptr);
			for (;*ptr != '\0'; ptr++)
				/* printf ("%s\n", ptr); */
				;
			ptr++;
		}

		return;
}

void print_driver_info_4 (PDRIVER_INFO_4 info)
{
		char		*ptr = NULL;

		printf ("\tDriver Name\t= %s\n",	info->pName);
		printf ("\tEnvironment\t= %s\n",	info->pEnvironment);
		printf ("\tVersion\t\t= %d\n",		info->cVersion);
		printf ("\tDriver Path\t= %s\n",	info->pDriverPath);
		printf ("\tData File\t= %s\n",		info->pDataFile);
		printf ("\tConfig File\t= %s\n",	info->pConfigFile);
		printf ("\tHelp Path\t= %s\n",		info->pHelpFile);
		printf ("\tMonitor Name\t= %s\n",	info->pMonitorName);
		printf ("\tData Type\t= %s\n",		info->pDefaultDataType);
		printf ("\tPrevious Names\t= %s\n",	info->pszzPreviousNames);
		ptr = (char*)info->pDependentFiles;
		while ((ptr != NULL) && (*ptr != '\0'))
		{
			printf ("\tDependent Files\t= %s\n", ptr);
			for (;*ptr != '\0'; ptr++)
				/* printf ("%s\n", ptr); */
				;
			ptr++;
		}

		return;
}

void print_driver_info_6 (PDRIVER_INFO_6 info)
{
		char		*ptr = NULL;

		printf ("\tDriver Name\t= %s\n",	info->pName);
		printf ("\tEnvironment\t= %s\n",	info->pEnvironment);
		printf ("\tVersion\t\t= %d\n",		info->cVersion);
		printf ("\tDriver Path\t= %s\n",	info->pDriverPath);
		printf ("\tData File\t= %s\n",		info->pDataFile);
		printf ("\tConfig File\t= %s\n",	info->pConfigFile);
		printf ("\tHelp Path\t= %s\n",		info->pHelpFile);
		printf ("\tMonitor Name\t= %s\n",	info->pMonitorName);
		printf ("\tData Type\t= %s\n",		info->pDefaultDataType);
		printf ("\tPrevious Names\t= %s\n",	info->pszzPreviousNames);
		ptr = (char*)info->pDependentFiles;
		if (ptr != NULL)
		{
			while (*ptr != '\0')
			{
				printf ("\tDependent Files\t= %s\n", ptr);
				for (;*ptr != '\0'; ptr++)
					/* printf ("%s\n", ptr); */
					;
				ptr++;
			}
		}
		else
		{
			printf ("\tPrevious Names\t= (null)\n");
		}


		ptr = (char*)info->pszzPreviousNames;
		if (ptr != NULL)
		{
			while (*ptr != '\0')
			{
				printf ("\tPrevious Names\t= %s\n", ptr);
				for (;*ptr != '\0'; ptr++)
					/* printf ("%s\n", ptr); */
					;
				ptr++;
			}
		}
		else
		{
			printf ("\tPrevious Names\t= (null)\n");
		}

		printf ("\tDriver Date\t= %d\n", info->ftDriverDate);
		printf ("\tDriver Version\t= %d\n", info->dwlDriverVersion);
		printf ("\tManufacture Name = %s\n", info->pszMfgName);
		printf ("\tOEM URL\t\t= %s\n", info->pszOEMUrl);
		printf ("\tHardware ID\t= %s\n", info->pszHardwareID);
		printf ("\tProvider\t= %s\n", info->pszProvider);

		return;
}

void print_doc_info_1 (PDOC_INFO_1 info)
{
	printf ("\tDocument Name\t= %s\n", info->pDocName);
	printf ("\tOutput Filename\t= %s\n", info->pOutputFile);
	printf ("\tDatatype\t= %s\n", info->pDatatype);

	return;
}

void print_printer_enum_values (PRINTER_ENUM_VALUES *info)
{
	DWORD	i = 0;;

	printf ("\tValue Name\t= %s [0x%x]\n", info->pValueName, info->cbValueName);
	printf ("\tType\t\t= 0x%x\n", info->dwType);
	printf ("\tSize\t\t= 0x%x\n", info->cbData);

	while (i < info->cbData)
	{
		printf ("\t0x%x", *(info->pData++));
		if (i%4 == 3)
			printf ("\n");
		i++;
	}
	printf ("\n");

	return;
}