#include <ps5kld/dmap.h>


uint64_t get_dmap()
{
    return !kernel_pmap ? 0 : kernel_pmap->pm_pml4 - kernel_pmap->pm_cr3;
}   


uint64_t virt2phys(uint64_t cr3, uint64_t vaddr)
{
    kprintf("[virt2phys] cr3=0x%llx vaddr=0x%llx\n", cr3, vaddr);

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

    kprintf("[virt2phys] pml4_idx=%llu pdpt_idx=%llu pd_idx=%llu pt_idx=%llu offset=%llu\n",
        addr.levels.pml4_index, addr.levels.pdpt_index,
        addr.levels.pd_index, addr.levels.pt_index, addr.levels.offset);

    pml4 = (uint64_t*) PHYSTODMAP(cr3);
    kprintf("[virt2phys] pml4=0x%llx (dmap+cr3)\n", (uint64_t)pml4);

    pml4_entry = pml4 + addr.levels.pml4_index;
    kprintf("[virt2phys] pml4_entry @ 0x%llx = 0x%llx\n", (uint64_t)pml4_entry, *pml4_entry);
    if (!PDE_FIELD(pml4_entry, PAGE_PRESENT))
    {
        kprintf("Error reading PML4, page not present!\n");
        return 0;
    }

    pdpt_entry = (uint64_t*) PHYSTODMAP((*pml4_entry & PD_CLEAN_ADDR_MASK)) + addr.levels.pdpt_index;
    kprintf("[virt2phys] pdpt_entry @ 0x%llx = 0x%llx\n", (uint64_t)pdpt_entry, *pdpt_entry);

    if (!PDE_FIELD(pdpt_entry, PAGE_PRESENT))
    {
        kprintf("Error reading pdpt entry, page not present!\n");
        return 0;
    }

    pd_entry = (uint64_t*) PHYSTODMAP((*pdpt_entry & PD_CLEAN_ADDR_MASK)) + addr.levels.pd_index;
    kprintf("[virt2phys] pd_entry @ 0x%llx = 0x%llx\n", (uint64_t)pd_entry, *pd_entry);

    if (!PDE_FIELD(pd_entry, PAGE_PRESENT))
    {
        kprintf("Error reading pd entry, page not present!\n");
        return 0;
    }

    if (PDE_FIELD(pd_entry, PAGE_SIZE_BIT))
    {
        uint64_t phys = (*pd_entry & PDE_LARGE_ADDR_MASK) | (vaddr & LARGE_PAGE_OFFSET_MASK);
        kprintf("[virt2phys] 2MB page detected! phys=0x%llx\n", phys);
        return phys;
    }

    pt_entry = (uint64_t*) PHYSTODMAP((*pd_entry & PD_CLEAN_ADDR_MASK)) + addr.levels.pt_index;
    kprintf("[virt2phys] pt_entry @ 0x%llx = 0x%llx\n", (uint64_t)pt_entry, *pt_entry);

    if (!PDE_FIELD(pt_entry, PAGE_PRESENT))
    {
        kprintf("Error reading pt entry, page not present!\n");
        return 0;
    }

    uint64_t phys = (*pt_entry & PD_CLEAN_ADDR_MASK) | addr.levels.offset;
    kprintf("[virt2phys] 4KB page, phys=0x%llx\n", phys);
    return phys;
}



