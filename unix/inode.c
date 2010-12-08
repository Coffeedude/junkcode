/* -*- mode: c; c-basic-offset: 4; indent-tabs-mode: nil; tab-width: 4 -*-
 * ex: set softtabstop=4 tabstop=8 expandtab shiftwidth=4: *
 * Editor Settings: expandtabs and use 4 spaces for indentation */

#include <stdio.h>
#include <unistd.h>
#include <sys/stat.h>

int main(int argc, char* argv[])
{
    struct stat fileStat = {0};

    printf("sizeof(stat->inode_t == %lu)\n", sizeof(fileStat.st_ino));

    return 0;
}
	 
