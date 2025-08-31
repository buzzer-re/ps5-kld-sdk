#include <ps5kld/kernel.h>
#include <ps5kld/offsets/offsets.h>

void (*kprintf)(char* fmt, ...) = NULL;

struct flat_pmap* kernel_pmap = NULL;
uint64_t kernel_base;
uint64_t Xfast_syscall = 0;
uint64_t* apic_ops = 0;

#define SET_KERNEL_SYMBOLS(fw) do { \
    Xfast_syscall = Xfast_syscall_##fw; \
    kernel_base   = get_kernel_base(); \
    kprintf       = (void (*)(char *, ...))(kernel_base + kprintf_offset_##fw); \
    apic_ops      = (void *)(kernel_base + apic_ops_offset_##fw); \
    kernel_pmap   = (void *)(kernel_base + kernel_pmap_offset_##fw); \
} while (0)


uint64_t get_kernel_base()
{
    return rdmsr(MSR_LSTAR) - Xfast_syscall;
}

int init_kernel(uint32_t fwver)
{
    kernel_base = get_kernel_base();
    
    switch (fwver)
    {
        case 0x403:
            SET_KERNEL_SYMBOLS(403);
            break;
        case 0x500:
            SET_KERNEL_SYMBOLS(500);
            break;
        default:
            return 0; // unsupported
    }

    return 1;
}

