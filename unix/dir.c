#include <sys/stat.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <dirent.h>

int main(
	int argc,
	char *argv[]
	)
{
	int fd = -1;
	DIR *d = NULL;	
	

	if ((d = opendir(argv[1])) == NULL) {
		fprintf(stderr, "opendir: %s\n", strerror(errno));
		exit(1);
	}

	if ((fd = dirfd(d)) == -1) {		
		fprintf(stderr, "dirfd: %s\n", strerror(errno));
		exit(1);
	}

	if (closedir(d) == -1) {
		fprintf(stderr, "closedir: %s\n", strerror(errno));
		exit(1);
	}		

	if (close(fd) == -1) {
		fprintf(stderr, "close: %s\n", strerror(errno));
		exit(1);
	}

	return 0;
}
