/* -*- mode: c; c-basic-offset: 4; indent-tabs-mode: nil; tab-width: 4 -*-
 * ex: set softtabstop=4 tabstop=8 expandtab shiftwidth=4: *
 * Editor Settings: expandtabs and use 4 spaces for indentation */


#include <sys/types.h>
#include <sys/sysctl.h>
#include <sys/vmmeter.h>
#include <vm/vm_param.h>
#include <errno.h>
#include <string.h>
#include <stdio.h>
#include <unistd.h>

int main(int argc, char *argv[])
{
    int err = 0;
    struct vmtotal vm_stats = { 0 };
    size_t vm_stats_size = sizeof(vm_stats);
    long long real_mem = 0;
    size_t real_mem_size = sizeof(real_mem);
    double loadAvg = 0.0;

    err = sysctlbyname("vm.vmtotal", &vm_stats, &vm_stats_size, NULL, 0);
    if (err)
    {
        fprintf(stderr, "Failed to call sysctl('vm.vmtotal') (%s)\n", strerror(errno));
        return 1;
    }

    err = sysctlbyname("hw.realmem", (void*)&real_mem, &real_mem_size, NULL, 0);
    if (err)
    {
        fprintf(stderr, "Failed to call sysctl('hw.realmem') (%s)\n", strerror(errno));
        return 1;
    }

    printf("Physcial Memory                = %lld\n", real_mem);
    printf("Run queue                      = %d\n", vm_stats.t_rq);
    printf("Jobs in disk qait              = %d\n", vm_stats.t_dw);
    printf("Jobs in page wait              = %d\n", vm_stats.t_pw);
    printf("Jobs sleeping in core          = %d\n", vm_stats.t_sl);
    printf("Swapped out jobs               = %d\n", vm_stats.t_sw);
    printf("Total virtual memory           = %d\n", vm_stats.t_vm);
    printf("Active virtual memory          = %d\n", vm_stats.t_avm);
    printf("Total real memory in use       = %d\n", vm_stats.t_rm);
    printf("Active real memory             = %d\n", vm_stats.t_arm);
    printf("Shared virtual memory          = %d\n", vm_stats.t_vmshr);
    printf("Active shared virtual memory   = %d\n", vm_stats.t_avmshr);
    printf("Share real memory              = %d\n", vm_stats.t_rmshr);
    printf("Active shared real memory      = %d\n", vm_stats.t_armshr);
    printf("Free memory pages              = %d\n", vm_stats.t_free);
    printf("Free memory                    = %dK\n", (vm_stats.t_free * getpagesize()) >> 10);

    if (getloadavg(&loadAvg, 1) != -1)
    {
        printf("Current load = %f\n", loadAvg);
    }


    return 0;
}
