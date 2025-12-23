#include <ps5kld/kernel.h>
#include <ps5kld/intrin.h>
#include <ps5kld/dmap.h>
#include <ps5kld/machine/idt.h>

int module_start(kproc_args* args)
{
    kprintf("Hello from kernel SDK, running on fw: %x\n", args->fwver);
    kprintf("Kernel base: %#02lx\n", args->kdata_base);

    uint8_t idt[10];
    __sidt((uint64_t*)idt);

    IDTR* idtr = (IDTR*) idt;

    kprintf("CR3: %x\nCR0: %x\nIDT base: %#02lx\n",
        __readcr3(),
        __readcr0(),
        idtr->base
    );


    return 0;
}

