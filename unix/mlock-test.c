#include <sys/mman.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main( int argc, char *argv[] ) 
{
	int psize = getpagesize();
	char *bytes = NULL;
	char *p;
	int ret;

	/* allocate 2 page sizes since we will have to offset from the first */

	if ((bytes = malloc( psize )) == NULL ) {
		fprintf(stderr, "malloc() failed!\n");
		exit(1);
	}

	p = bytes + (psize - ((unsigned int)bytes % psize));
	ret = mlock ( p, 1);
	if ( ret != 0 ) {
		perror( "mlock() failed!" );
		exit(1);
	}

	munlock( bytes, psize );

	return 0;
}

