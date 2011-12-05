#include <sys/types.h>
#include <attr/xattr.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <sys/stat.h>
#include <fcntl.h>



/***********************************************************
 **********************************************************/

int main(
    int argc,
    char *argv[]
    )
{
    int fd = -1;
    char *data = "testing data";    
    int ret = 0;    
    
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <filename>\n", argv[0]);
        exit(1);
    }
    
    if ((fd = open(argv[1], O_RDWR, 0)) == -1) {
        int err = errno;
        
        fprintf(stderr, "open(): %s\n", strerror(err));
        exit(err);
    }

    ret = fsetxattr(fd,
                    "user.test0",
                    (const void*)data,
                    strlen(data) + 1,
                    0);
    if (ret == -1) {
        int err = errno;
        
        fprintf(stderr, "fsetxattr(): %s\n", strerror(err));
        exit(err);
    }

    return 0;    
}



/*
local variables:
mode: c
c-basic-offset: 4
indent-tabs-mode: nil
tab-width: 4
end:
*/
