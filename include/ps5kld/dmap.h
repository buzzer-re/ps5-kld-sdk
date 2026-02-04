#pragma once

#include <ps5kld/kernel.h>

#define PD_CLEAN_ADDR_MASK 0xffffffffff800ULL
#define PDE_FIELD(pde, field) (*pde & field)

#define PAGE_PRESENT (1 << 0)
#define PAGE_SIZE_BIT (1 << 7)

// 2MB pages
#define LARGE_PAGE_OFFSET_MASK  0x1FFFFFULL 
#define PDE_LARGE_ADDR_MASK     0xFFFFFFFE00000ULL
// TODO: check huge pages - 1GB pages


#define PHYSTODMAP(phys_addr) (get_dmap() + phys_addr)

typedef struct __vaddr_page
{
    union 
    {
        uint64_t vaddr;
        struct
        {
            uint64_t offset: 12;
            uint64_t pt_index: 9;
            uint64_t pd_index: 9;
            uint64_t pdpt_index: 9;
            uint64_t pml4_index: 9;
            uint64_t sign_ext: 16;
        } levels;
    };
} vaddr_page;

uint64_t get_dmap();
uint64_t virt2phys(uint64_t cr3, uint64_t vaddr);