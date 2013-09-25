/* -*- mode: c; c-basic-offset: 4; indent-tabs-mode: nil; tab-width: 4 -*-
 * ex: set softtabstop=4 tabstop=8 expandtab shiftwidth=4: *
 * Editor Settings: expandtabs and use 4 spaces for indentation */

#include <sys/time.h>
#include <sys/resource.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <assert.h>
#include <stdlib.h>
#include <kvm.h>
#include <sys/param.h>
#include <sys/sysctl.h>
#include <sys/user.h>
#include <unistd.h>
#include <sys/types.h>

int main (
    int argc,
    char *argv[]
    )
{
    int err = 0;
    int i = 0;
    size_t alloc_size = 1024*1024;
    struct rusage stats = { 0 };
    void *buffer = NULL;
    kvm_t *kd = NULL;
    struct kinfo_proc *procs = NULL;
    int proc_count = 0;

    kd = kvm_open(NULL, NULL, NULL, 0, NULL);
    if (kd == NULL)
    {
        fprintf(stderr, "Failed to open kvm!\n");
        return 1;
    }

    printf("Process statistics:\n");
    for (i=0; i<100; i++)
    {
        buffer = malloc(alloc_size * (i+1));
        assert(buffer);

        memset(&stats, 0x0, sizeof(stats));

#if 0
        err = getrusage(RUSAGE_SELF, &stats);
        if (err)
        {
            fprintf(stderr, "getrusage() failed!  (%s)\n", strerror(err));
            return err;
        }
        printf("  Max RSS (%d) = %ld\n", i, stats.ru_maxrss);
#endif

        proc_count = 0;
        procs = kvm_getprocs(kd, KERN_PROC_PID, (int)getpid(), &proc_count);
        if (procs == NULL || proc_count == 0)
        {
            fprintf(stderr, "Failed to get procinfo!!\n");
            return 2;
        }

        printf("  Max RSS (%d) = %ldK\n", i, procs[0].ki_size>>10);

        sleep (2);

        free(buffer);
    }

    return 0;
}
