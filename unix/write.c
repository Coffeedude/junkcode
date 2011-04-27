/* -*- mode: c; c-basic-offset: 4; indent-tabs-mode: nil; tab-width: 4 -*-
 * ex: set softtabstop=4 tabstop=8 expandtab shiftwidth=4: *
 * Editor Settings: expandtabs and use 4 spaces for indentation */

#include <unistd.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>

int main(int argc, char* argv[])
{
	int fd = 0;
    size_t bytesWritten = 0;
    char buffer[20] = { 0 };
	
	fd = open(argv[1], O_RDWR, 0);
	if (fd == -1)
	{
        printf("ERROR: %s\n", strerror(errno));
        exit (1);
	}


    bytesWritten = pwrite(fd, buffer, 0, 10);

    close (fd);


    return 0;
}
