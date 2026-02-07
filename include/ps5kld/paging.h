#pragma once

#include <ps5kld/dmap.h>

enum pde_shift {
    PDE_PRESENT = 0,
    PDE_RW,
    PDE_USER,
    PDE_WRITE_THROUGH,
    PDE_CACHE_DISABLE,
    PDE_ACCESSED,
    PDE_DIRTY,
    PDE_PS,
    PDE_GLOBAL,
    PDE_XOTEXT = 58,
    PDE_PROTECTION_KEY = 59,
    PDE_EXECUTE_DISABLE = 63
};

#define PTE_CLEAN_ADDR_MASK 0xffffffffff800ULL
#define PDE_ADDR_MASK PTE_CLEAN_ADDR_MASK
#define PDE_FIELD(pde, field) (*pde & field)

#define PAGE_PRESENT (1 << 0)
#define PTE_PRESENT PAGE_PRESENT
#define PDE_PRESENT_MASK PTE_PRESENT
#define PDE_RW_MASK                     1UL
#define PDE_USER_MASK                   1UL
#define PDE_WRITE_THROUGH_MASK          1UL
#define PDE_CACHE_DISABLE_MASK          1UL
#define PDE_ACCESSED_MASK               1UL
#define PDE_DIRTY_MASK                  1UL
#define PDE_PS_MASK                     1UL
#define PDE_GLOBAL_MASK                 1UL
#define PDE_XOTEXT_MASK                 1UL
#define PDE_PROTECTION_KEY_MASK         0xFUL
#define PDE_EXECUTE_DISABLE_MASK        1UL

#define PAGE_SIZE_BIT (1 << 7)
#define CLEAR_PDE_BIT(pde, name)        (pde &= ~(PDE_##name##_MASK << PDE_##name))
#define GET_PDE_FIELD(pde, name)            (((pde) >> PDE_##name) & PDE_##name##_MASK)
#define SET_PDE_FIELD(pde, name, val)   (pde |= (val << PDE_##name))
#define PDE_ADDR(pde)                   (pde & PDE_ADDR_MASK)
#define SET_PDE_FIELD(pde, name, val)   (pde |= (val << PDE_##name))
#define SET_PDE_BIT(pde, name)          (pde |= (PDE_##name##_MASK << PDE_##name))
#define SET_PDE_ADDR(pde, addr)         do { \
                                            pde &= ~(PDE_ADDR_MASK); \
                                            pde |= (addr & PDE_ADDR_MASK); \
                                        } while (0)

// 2MB pages
#define LARGE_PAGE_OFFSET_MASK  0x1FFFFFULL 
#define PDE_LARGE_ADDR_MASK     0xFFFFFFFE00000ULL
// TODO: check huge pages - 1GB pages

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



int bind_page_pte(uint64_t vaddr1, uint64_t vaddr2, uint64_t pml);
uint64_t virt2phys(uint64_t cr3, uint64_t vaddr);
uint64_t remap_page(uint64_t cr3, uint64_t vaddr_src, uint64_t vaddr_dst);

// page tables 
uint64_t get_plm4e(uint64_t cr3, uint64_t vaddr);
uint64_t get_pdpt(uint64_t cr3, uint64_t vaddr);
uint64_t* get_pd(uint64_t cr3, uint64_t vaddr); // returns a pointer, because here's where we can get the large pages
uint64_t* get_pt(uint64_t cr3, uint64_t vaddr);
