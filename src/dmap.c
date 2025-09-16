#include <ps5kld/dmap.h>


uint64_t get_dmap()
{
    return !kernel_pmap ? 0 : kernel_pmap->pm_pml4 - kernel_pmap->pm_cr3;
}   