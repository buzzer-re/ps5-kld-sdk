#include <ps5kld/paging.h>


uint64_t get_plm4e(uint64_t cr3, uint64_t vaddr)
{
    if (!cr3 || !vaddr)
        return 0;

    // get pml4e using the vaddr struct that contains the bitfields
    vaddr_page addr;
    addr.vaddr = vaddr;
    // read from dmap
    uint64_t* pml4e = (uint64_t*)PHYSTODMAP(cr3) + addr.levels.pml4_index;
    
    return (*pml4e & PTE_PRESENT) ? *pml4e : 0;
}

uint64_t get_pdpt(uint64_t cr3, uint64_t vaddr)
{
    if (!cr3 || !vaddr)
        return 0;

    vaddr_page addr;
    addr.vaddr = vaddr;

    uint64_t pml4 = get_plm4e(cr3, vaddr);
    uint64_t pdpt_phys = pml4 & PTE_CLEAN_ADDR_MASK;
    // read from dmap
    uint64_t* pdpte = (uint64_t*)PHYSTODMAP(pdpt_phys) + addr.levels.pdpt_index;
    return (*pdpte & PTE_PRESENT) ? *pdpte : 0;
}

uint64_t* get_pd(uint64_t cr3, uint64_t vaddr)
{
    if (!cr3 || !vaddr)
        return 0;

    vaddr_page addr;
    addr.vaddr = vaddr;

    uint64_t pdpt = get_pdpt(cr3, vaddr);
    uint64_t pde_phys = pdpt & PTE_CLEAN_ADDR_MASK;
    uint64_t* pde = (uint64_t*) PHYSTODMAP(pde_phys) + addr.levels.pd_index;

    return pde;
}

uint64_t* get_pt(uint64_t cr3, uint64_t vaddr)
{
    if (!cr3 || !vaddr)
        return 0;

    vaddr_page addr;
    addr.vaddr = vaddr;

    uint64_t* pde = get_pd(cr3, vaddr);
    if (PDE_FIELD(pde, PAGE_SIZE_BIT))
    {
        // large page
        return 0;
    }

    uint64_t* pt_entry = (uint64_t*) PHYSTODMAP((*pde & PTE_CLEAN_ADDR_MASK)) + addr.levels.pt_index;
    
    return pt_entry;
}

uint64_t virt2phys(uint64_t cr3, uint64_t vaddr)
{
    if (!cr3 || !vaddr)
        return 0;

    uint64_t* pt = get_pt(cr3, vaddr);
    vaddr_page addr;
    addr.vaddr = vaddr;

    if (!pt)
    {
        //
        // Check superpages (2MB)
        //
        uint64_t* pde = get_pd(cr3, vaddr);
        if (PDE_FIELD(pde, PAGE_SIZE_BIT))
        {
            // is a superpage (2MB)
            kprintf("superpage\n");
            return (*pde & PDE_LARGE_ADDR_MASK) | (vaddr & LARGE_PAGE_OFFSET_MASK);
        }

        return 0;
    }

    if (!PDE_FIELD(pt, PAGE_PRESENT))
    {
        return 0;
    }

    uint64_t phys = (*pt & PTE_CLEAN_ADDR_MASK) | addr.levels.offset;
    return phys;
}


uint64_t* get_pte_for_remap(uint64_t cr3, uint64_t addr)
{
    //
    // First we need to check if this is a large page, 
    // if this is a large page we need to downgrade it
    // code below is based on Specter's implementation on byepervisor 
    // https://github.com/PS5Dev/Byepervisor/blob/57204cbd7bd26ed4623634d52b0f60f40d630087/src/paging.cpp#L200C5-L200C32 
    //
    uint64_t new_pte = 0;
    uint64_t* pde = get_pd(cr3, addr);
    // check
    if (PDE_FIELD(pde, PAGE_SIZE_BIT))
    {
        // large page, downgrade it
        kprintf("Large page detected, downgrading it to 512 chunks of 4k each");
        uint64_t pde_pa = *pde & PTE_CLEAN_ADDR_MASK;
        uint64_t pde_value = *pde;
        // this is our new page table which will hold the downgraded superpage
        uint64_t* new_pt = (uint64_t*) kmalloc(0x1000, KM_TEMP, M_ZERO | M_WAITOK);
        // fill page table
        for (int i = 0; i < 512; ++i)
        {
            new_pte = 0;
            SET_PDE_FIELD(new_pte, RW, GET_PDE_FIELD(pde_value, RW));
            SET_PDE_FIELD(new_pte, USER, GET_PDE_FIELD(pde_value, USER));
            SET_PDE_FIELD(new_pte, DIRTY, GET_PDE_FIELD(pde_value, DIRTY));
            SET_PDE_FIELD(new_pte, GLOBAL, GET_PDE_FIELD(pde_value, GLOBAL));
            SET_PDE_FIELD(new_pte, XOTEXT, GET_PDE_FIELD(pde_value, XOTEXT));
            SET_PDE_FIELD(new_pte, EXECUTE_DISABLE, GET_PDE_FIELD(pde_value, EXECUTE_DISABLE));
            SET_PDE_ADDR(new_pte, pde_pa);
            SET_PDE_BIT(new_pte, PRESENT);

            new_pt[i] = new_pte;
            
            pde_pa += 0x1000; // important
        }
        // update PDE fields to the new pt created
        SET_PDE_BIT(pde_value, RW);
        CLEAR_PDE_BIT(pde_value, PS);
        CLEAR_PDE_BIT(pde_value, GLOBAL);
        CLEAR_PDE_BIT(pde_value, XOTEXT);
        SET_PDE_ADDR(pde_value, virt2phys(cr3, (uint64_t) new_pt));
        
        // pde is already from the dmap mapping, safe to write here
        *pde = pde_value;

        return new_pt;
    }

    return get_pt(cr3, addr);
}

// TODO: Fix it
uint64_t remap_page(uint64_t cr3, uint64_t vaddr_src, uint64_t vaddr_dst)
{
    if (!cr3 || !vaddr_src || !vaddr_dst)
    {
        return 0;
    }

    kprintf("vaddr1: %#02lx physaddr: %#02lx\n", vaddr_src, virt2phys(cr3, vaddr_src));
    // uint64_t* pte1 = get_pte_for_remap(cr3, vaddr_src);
    uint64_t* pt2 = get_pte_for_remap(cr3, vaddr_dst);

    uint64_t orig_pa = PDE_ADDR(*pt2);
    SET_PDE_ADDR(*pt2, virt2phys(cr3, vaddr_dst));
    
    return orig_pa;
}