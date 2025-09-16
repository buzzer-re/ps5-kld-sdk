#pragma once

#define _KERNEL
#include <sys/param.h>
#include <sys/conf.h>
#include <sys/cpuset.h>
#include <machine/specialreg.h>
#include <stdint.h>


struct flat_pmap {                                                                                                   
    uint64_t mtx_name_ptr;
    uint64_t mtx_flags;
    uint64_t mtx_data;
    uint64_t mtx_lock;
    uint64_t pm_pml4;
    uint64_t pm_cr3;
}; 

typedef struct __kproc_args
{
    uint64_t kdata_base;
    uint32_t fwver;
} kproc_args;


uint64_t get_kernel_base();
int init_kernel(uint32_t fwver);


// Kernel functions
extern void(*kprintf)(char* fmt, ...);


// Kernel variables
extern struct flat_pmap* kernel_pmap;
extern uint64_t* apic_ops;