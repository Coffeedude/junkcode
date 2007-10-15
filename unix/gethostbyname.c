#include <unistd.h>
#include <netdb.h>
#include <stdio.h>
#include <string.h>

int main( int argc, char *argv[] )
{
        char ac[80];
        struct hostent *phe = NULL;
        int i = 0;
	struct in_addr addr;

        if ( gethostname( ac, sizeof( ac ) ) == -1 ) {
                return -1;
        }

        if ( ( phe = gethostbyname( ac ) ) == NULL ) {
                return -1;
        }

        /* Return the first valid IP Address */

        for ( i = 0; phe->h_addr_list[i]; i++ ) {
		memcpy( &addr, phe->h_addr_list[i], sizeof( struct in_addr ) );
		printf( "%s\n", inet_ntoa(addr));
        }

        return 0;
}

