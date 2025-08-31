#include <crt.h>


void _start(kproc_args* args)
{
    if (init_kernel(args->fwver))
    {
        module_start(args);
    }
}