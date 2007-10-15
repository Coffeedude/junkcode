#if 1	/* unicode supprot */
#  define UNICODE
#  define _UNICODE
#  include <tchar.h>
#  include <wchar.h>
#  define printf wprintf
#  define MAIN wmain
#  define WINCHAR WCHAR
#else	/* ansi characters */
#  include <stdio.h>
#  define WINCHAR char
#  define MAIN main
#endif

#include <windows.h>
/* #include <strsafe.h> */
#include <stdlib.h>



/********************************************************************
********************************************************************/

static void PrintLastError( void )
{
	LPVOID		lpMsgBuf;
	DWORD		status;

	status = GetLastError();

	FormatMessage( FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | 
		FORMAT_MESSAGE_IGNORE_INSERTS, NULL, GetLastError(),
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), // Default language
		(LPTSTR) &lpMsgBuf, 0, NULL);
	printf(TEXT("ERROR  [0x%x] : %s\n"), status, (LPTSTR)lpMsgBuf);
	LocalFree (lpMsgBuf);

		return;
}

/********************************************************************
********************************************************************/

#define BUFFER_SIZE 8192

int MAIN( int argc, WINCHAR *argv[] )
{
    HANDLE h;
    EVENTLOGRECORD *pevlr; 
    BYTE bBuffer[BUFFER_SIZE]; 
    DWORD dwRead, dwNeeded, cRecords, dwThisRecord; 
 
    // Open the Application event log. 
 
	printf(TEXT("Opening the %s log on on %s..."), (LPTSTR)argv[2], (LPTSTR)argv[1]);
    if ( !(h = OpenEventLog( argv[1], argv[2])) ) {
		PrintLastError();
		exit( 1 );
	}
	printf(TEXT("ok\n"));

 
    pevlr = (EVENTLOGRECORD *) &bBuffer; 
 
	printf(TEXT("Oldest record number...."));
    GetOldestEventLogRecord(h, &dwThisRecord);
	printf(TEXT("%d\n"), dwThisRecord);

    while (ReadEventLog(h,                // event log handle 
                EVENTLOG_FORWARDS_READ |  // reads forward 
                EVENTLOG_SEQUENTIAL_READ, // sequential read 
                0,            // ignored for sequential reads 
                pevlr,        // pointer to buffer 
                BUFFER_SIZE,  // size of buffer 
                &dwRead,      // number of bytes read 
                &dwNeeded))   // bytes in next record 
    {
        while (dwRead > 0) 
        { 
            // Print the record number, event identifier, type, 
            // and source name. 
 
            printf(TEXT("%02d  Event ID: 0x%08X "), 
                dwThisRecord++, pevlr->EventID); 
            printf(TEXT("EventType: %d Source: %s\n"), pevlr->EventType, 
				(LPSTR)((LPBYTE) pevlr + sizeof(EVENTLOGRECORD))); 
 
            dwRead -= pevlr->Length; 
            pevlr = (EVENTLOGRECORD *)((LPBYTE) pevlr + pevlr->Length); 
        } 
 
        pevlr = (EVENTLOGRECORD *) &bBuffer; 
    } 
 
    CloseEventLog(h); 
}

