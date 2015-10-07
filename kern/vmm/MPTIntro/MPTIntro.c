#include <lib/gcc.h>
#include <lib/x86.h>
#include <lib/debug.h>

#include "import.h"

#define PT_PERM_UP 0
#define PT_PERM_PTU (PTE_P | PTE_W | PTE_U)

/**
 * Page directory pool for NUM_IDS processes.
 * mCertiKOS maintains one page structure for each process.
 * Each PDirPool[index] represents the page directory of the page structure for the process # [index].
 * In mCertiKOS, we statically allocate page directories, and maintain the second level page tables dynamically.
 */
char * PDirPool[NUM_IDS][1024] gcc_aligned(PAGESIZE);

/**
 * In mCertiKOS, we use identity page table mappings for the kernel memory.
 * IDPTbl is a statically allocated, identity page table that will be reused for
 * all the kernel memory.
 * Every page directory entry of kernel memory links to corresponding entry in IDPTbl.
 */
unsigned int IDPTbl[1024][1024] gcc_aligned(PAGESIZE);


// sets the CR3 register with the start address of the page structure for process # [index]
void set_pdir_base(unsigned int index)
{
    // STILL: is this correct? lol I can't pointers
    set_cr3(PDirPool[index]);
}

// returns the page directory entry # [pde_index] of the process # [proc_index]
// this can be used to test whether the page directory entry is mapped
unsigned int get_pdir_entry(unsigned int proc_index, unsigned int pde_index)
{
    // assuming if it's not mapped, it's 0
    return (unsigned int) PDirPool[proc_index][pde_index];
}   

// sets specified page directory entry with the start address of physical page # [page_index].
// you should also set the permissions PTE_P, PTE_W, and PTE_U
void set_pdir_entry(unsigned int proc_index, unsigned int pde_index, unsigned int page_index)
{
    PDirPool[proc_index][pde_index] = (char *) (PAGESIZE * page_index | PT_PERM_PTU);
}   

// sets the page directory entry # [pde_index] for the process # [proc_index]
// with the initial address of page directory # [pde_index] in IDPTbl
// you should also set the permissions PTE_P, PTE_W, and PTE_U
// this will be used to map the page directory entry to identity page table.
void set_pdir_entry_identity(unsigned int proc_index, unsigned int pde_index)
{   
    // QUESTION: Does this give the actual address of where IDPTbl is in hardware
    PDirPool[proc_index][pde_index] = (char *) ((unsigned int) IDPTbl[pde_index] | PT_PERM_PTU);
}   

// removes specified page directory entry (set the page directory entry to 0).
// don't forget to cast the value to (char *).
void rmv_pdir_entry(unsigned int proc_index, unsigned int pde_index)
{
    PDirPool[proc_index][pde_index] = (char *) 0;
}   

// returns the specified page table entry.
// do not forget that the permission info is also stored in the page directory entries.
unsigned int get_ptbl_entry(unsigned int proc_index, unsigned int pde_index, unsigned int pte_index)
{   
    // eliminating the permission bits
    unsigned int pte = (unsigned int) PDirPool[proc_index][pde_index];
    // unsigned int temp = pte & (unsigned int) (~(0U) << 3);
    unsigned int temp = pte - (pte % PAGESIZE);
    unsigned int * addr = (unsigned int *) temp;
    return addr[pte_index];
}

// sets specified page table entry with the start address of physical page # [page_index]
// you should also set the given permission
void set_ptbl_entry(unsigned int proc_index, unsigned int pde_index, unsigned int pte_index, unsigned int page_index, unsigned int perm)
{   
    unsigned int pte = (unsigned int) PDirPool[proc_index][pde_index];
    // unsigned int temp = pte & (unsigned int) (~(0U) << 3);
    unsigned int temp = pte - (pte % PAGESIZE);
    unsigned int * addr = (unsigned int *) temp;
    addr[pte_index] = (unsigned int) PAGESIZE * page_index + perm; 
}   

// sets the specified page table entry in IDPTbl as the identity map.
// you should also set the given permission
void set_ptbl_entry_identity(unsigned int pde_index, unsigned int pte_index, unsigned int perm)
{
    unsigned int idAddr = (pde_index << 22) | (pte_index << 12) | perm;
    IDPTbl[pde_index][pte_index] = idAddr;
}

// sets the specified page table entry to 0
void rmv_ptbl_entry(unsigned int proc_index, unsigned int pde_index, unsigned int pte_index)
{
    unsigned int pte = (unsigned int) PDirPool[proc_index][pde_index];
    // unsigned int temp = pte & (unsigned int) (~(0U) << 3);
    unsigned int temp = pte - (pte % PAGESIZE);
    unsigned int * addr = (unsigned int *) temp;
    addr[pte_index] = (unsigned int) 0;
}
