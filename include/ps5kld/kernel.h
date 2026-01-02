#pragma once

#define _KERNEL
#include <sys/param.h>
#include <sys/conf.h>
#include <sys/cpuset.h>
#include <machine/specialreg.h>
#include <stdint.h>

#define M_ZERO 0
#define M_NODUMP 1
#define M_NOWAIT 2
#define M_WAITOK 3
#define M_USE_RESERVE 4

struct flat_pmap {                                                                                                   
    uint64_t mtx_name_ptr;
    uint64_t mtx_flags;
    uint64_t mtx_data;
    uint64_t mtx_lock;
    uint64_t pm_pml4;
    uint64_t pm_cr3;
}; 

uint64_t get_kernel_base();
int init_kernel(uint32_t fwver);


// Kernel functions
extern void(*kprintf)(char* fmt, ...);
extern uint64_t(*kmalloc)(size_t size, uint64_t *type, int flags);

// Kernel variables
extern struct flat_pmap* kernel_pmap;
extern uint64_t* apic_ops;
extern uint64_t* KM_TEMP; // malloc_type