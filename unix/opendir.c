#include <stdlib.h>
#include <unistd.h>
#include <dirent.h>
#include <stdio.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include <errno.h>
#include <string.h>

int
main(
	int argc,
	char *argv[]
	)
{
	int fd = -1;
	DIR *dPtr = NULL;
	struct dirent *d = NULL;	

	if ((fd = open(argv[1], 0, 0)) == -1) {
		fprintf(stderr, "Unable to open %s (%s)\n",
			argv[1], strerror(errno));
		exit(1);
	}

	if ((dPtr = opendir(argv[1])) == NULL) {
		fprintf(stderr, "Unable to opendir %s (%s)\n",
			argv[1], strerror(errno));
		exit(2);
	}	

	while ((d = readdir(dPtr)) != NULL)
	{
		printf("%20s  %ld  %ld\n",
		       d->d_name,
		       d->d_ino,
		       d->d_off);		
	}

	if (errno != 0) {
		fprintf(stderr, "Failure during readdir()! (%s)\n",
			strerror(errno));
		exit(3);
	}
	
	closedir(dPtr);	

	return 0;	
}
