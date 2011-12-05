#include <errno.h>
#include <string.h>
#include <stdio.h>

int main(int argc, char *argv[])
{
    int i = 0;
    

    for (i=0; i<256; i++)
    {
        printf("%d\t=%s\n", i, strerror(i));
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

