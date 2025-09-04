#include <ps5kld/kernel.h>


int module_start(void* kproc_args)
{
    kprintf("Hello from kernel toolchain v2\n");


    return 0;
}