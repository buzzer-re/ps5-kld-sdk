#include <ps5kld/dmap.h>


uint64_t get_dmap()
{
    return !kernel_pmap ? 0 : kernel_pmap->pm_pml4 - kernel_pmap->pm_cr3;
}   


uint64_t virt2phys(uint64_t cr3, uint64_t vaddr)
{
    if (!cr3 || !vaddr) 
        return 0;
    
    if (cr3 & 0xFFF) // not aligned CR3 
        return 0;
    
    uint64_t* pml4; 
    uint64_t* pml4_entry;
    uint64_t* pdpt_entry;
    uint64_t* pd_entry;
    uint64_t* pt_entry;
    vaddr_page addr;
    addr.vaddr = vaddr;
    
    pml4 = (uint64_t*) PHYSTODMAP(cr3);
    
    pml4_entry = pml4 + addr.levels.pml4_index;
    if (!PDE_FIELD(pml4_entry, PAGE_PRESENT))
    {
        kprintf("Error reading PML4, page not present!\n");
        return 0;
    }

    pdpt_entry = (uint64_t*) PHYSTODMAP((*pml4_entry & PD_CLEAN_ADDR_MASK)) + addr.levels.pdpt_index;
    
    if (!PDE_FIELD(pdpt_entry, 1))
    {
        kprintf("Error reading pdpt entry, page not present!\n");
        return 0;
    }

    pd_entry = (uint64_t*) PHYSTODMAP((*pdpt_entry & PD_CLEAN_ADDR_MASK)) + addr.levels.pd_index;

    if (!PDE_FIELD(pd_entry, PAGE_PRESENT)) 
    {
        kprintf("Error reading pd entry, page not present!\n");
        return 0;
    }

    pt_entry = (uint64_t*) PHYSTODMAP((*pd_entry & PD_CLEAN_ADDR_MASK)) + addr.levels.pt_index;

    if (!PDE_FIELD(pt_entry, 1))
    {
        kprintf("Error reading pt entry, page not present!\n");
        return 0;
    }

    return (*pt_entry & PD_CLEAN_ADDR_MASK) | addr.levels.offset;;
}



