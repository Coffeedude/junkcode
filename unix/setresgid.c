/*
 * Test the setresuid() system call
 * copyright Gerald Carter <jerry@samba.org>  2006
 */

#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <grp.h>
#include <errno.h>
#include <string.h>

int main( int argc, char *argv[] )
{
	struct group *grp;

	if ( argc != 2 ) {
		printf("Usage: %s <user>\n", argv[0]);
		return 1;
	}

 	if ( (grp = getgrnam(argv[1])) == NULL ) {
		fprintf( stderr, "Unable to locate group %s\n", argv[1] );
		return 2;
	}

	printf("Setting egid to %d\n", (int) grp->gr_gid);

	if ( setresgid( -1, grp->gr_gid, -1 ) ) {
		fprintf( stderr, "setresgid( -1, %d, -1) failed (%s)\n",
			(int)grp->gr_gid, strerror(errno));
		return 3;
	}

	printf("Process gid now (%d, %d)\n", getgid(), getegid());
	
	return 0;
}

