#include <lib/x86.h>

#include "import.h"

#define VM_USERLO       0x40000000
#define VM_USERHI       0xF0000000
#define VM_USERLO_PI    (VM_USERLO / PAGESIZE)
#define VM_USERHI_PI    (VM_USERHI / PAGESIZE)

unsigned int get_pde(unsigned int vaddr)
{
    return vaddr >> 22;
}

unsigned int get_pte(unsigned int vaddr)
{
    unsigned int mask = (~(0U) >> 22);
    return (vaddr >> 12) & mask;
}

// QUESTION: Do we have to check if vadr is valid?

/**
 * Returns the page table entry corresponding to the virtual address,
 * according to the page structure of process # [proc_index].
 * Returns 0 if the mapping does not exist.
 */


unsigned int get_ptbl_entry_by_va(unsigned int proc_index, unsigned int vaddr)
{
    unsigned int pde_index = get_pde(vaddr);
    unsigned int pte_index = get_pte(vaddr);
    if (get_pdir_entry(proc_index, pde_index)) 
        return get_ptbl_entry(proc_index, pde_index, pte_index);
    else 
        return 0;
}         

// returns the page directory entry corresponding to the given virtual address
unsigned int get_pdir_entry_by_va(unsigned int proc_index, unsigned int vaddr)
{
    unsigned int pde_index = get_pde(vaddr);
    return get_pdir_entry(proc_index, pde_index);
}

// removes the page table entry for the given virtual address
void rmv_ptbl_entry_by_va(unsigned int proc_index, unsigned int vaddr)
{
    unsigned int pde_index = get_pde(vaddr);
    unsigned int pte_index = get_pte(vaddr);
    rmv_ptbl_entry(proc_index, pde_index, pte_index);
}

// removes the page directory entry for the given virtual address
void rmv_pdir_entry_by_va(unsigned int proc_index, unsigned int vaddr)
{
    unsigned int pde_index = get_pde(vaddr);
    rmv_pdir_entry(proc_index, pde_index);
}

// maps the virtual address [vaddr] to the physical page # [page_index] with permission [perm]
// you do not need to worry about the page directory entry. just map the page table entry.
void set_ptbl_entry_by_va(unsigned int proc_index, unsigned int vaddr, unsigned int page_index, unsigned int perm)
{
    unsigned int pde_index = get_pde(vaddr);
    unsigned int pte_index = get_pte(vaddr);
    set_ptbl_entry(proc_index, pde_index, pte_index, page_index, perm);
}

// registers the mapping from [vaddr] to physical page # [page_index] in the page directory
void set_pdir_entry_by_va(unsigned int proc_index, unsigned int vaddr, unsigned int page_index)
{
    unsigned int pde_index = get_pde(vaddr);
    set_pdir_entry(proc_index, pde_index, page_index);
}   

// initializes the identity page table
// the permission for the kernel memory should be PTE_P, PTE_W, and PTE_G,
// while the permission for the rest should be PTE_P and PTE_W.
void idptbl_init(unsigned int mbi_adr)
{
    int i, j;
    int kernelPerm = PTE_P | PTE_W | PTE_G;
    int normalPerm = PTE_P | PTE_W;

    container_init(mbi_adr);

    for (i = 0; i < 1024; ++i) {
        for (j = 0; j < 1024; ++j) {
            unsigned int addr = (i << 22); 
            if (addr < VM_USERLO || addr >= VM_USERHI)    
                set_ptbl_entry_identity(i, j, kernelPerm);
            else
                set_ptbl_entry_identity(i, j, normalPerm);
        }
    }
}
