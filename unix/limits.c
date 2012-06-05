#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <sys/time.h>
#include <sys/resource.h>

// int getrlimit(int resource, struct rlimit *rlim);
// int setrlimit(int resource, const struct rlimit *rlim);

int main(int argc, char *argv[])
{
    struct rlimit rlim = {0};

    if (getrlimit(RLIMIT_NOFILE, &rlim) < 0)
    {
        fprintf(stderr, "Error getting fd limit!\n");
        exit(1);
    }

    printf("Fd limits (soft == %d, hard == %d)\n",
           rlim.rlim_cur,
           rlim.rlim_max);

    rlim.rlim_cur = 4096;
    rlim.rlim_max = 8192;

    if (setrlimit(RLIMIT_NOFILE, &rlim) < 0)
    {
        fprintf(stderr, "Failed to set fd limits!\n");
        exit(1);
    }

    if (getrlimit(RLIMIT_NOFILE, &rlim) < 0)
    {
        fprintf(stderr, "Error getting fd limit!\n");
        exit(1);
    }

    printf("Fd limits (soft == %d, hard == %d)\n",
           rlim.rlim_cur,
           rlim.rlim_max);

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
