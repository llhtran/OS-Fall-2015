#include <lib/x86.h>

#include "import.h"

#define VM_USERLO       0x40000000
#define VM_USERHI       0xF0000000
#define VM_USERLO_PI    (VM_USERLO / PAGESIZE)
#define VM_USERHI_PI    (VM_USERHI / PAGESIZE)

/**
 * For each process from id 0 to NUM_IDS -1,
 * set the page directory entries so that the kernel portion of the map as identity map,
 * and the rest of page directories are unmmaped.
 */
void pdir_init(unsigned int mbi_adr)
{
    int i, j;

    idptbl_init(mbi_adr);

    for (i = 0; i < NUM_IDS; ++i) {
    	for (j = 0; j < 1024; ++j) {
            unsigned int addr = (j << 22);
        	if (addr < VM_USERLO || addr >= VM_USERHI)  {  
            	set_pdir_entry_identity(i, j);
            } else        
        		rmv_pdir_entry(i, j);
        }
    }

}

/**
 * Allocates a page (with container_alloc) for the page table,
 * and registers it in page directory for the given virtual address,
 * and clears (set to 0) the whole page table entries for this newly mapped page table.
 * It returns the page index of the newly allocated physical page.
 * In the case when there's no physical page available, it returns 0.
 */
unsigned int alloc_ptbl(unsigned int proc_index, unsigned int vadr)
{
/*	unsigned int page_index = container_alloc(proc_index);
	if (page_index) {
		unsigned int pde_index = vadr >> 22;
		set_pdir_entry(proc_index, pde_index, page_index);
		int i;
		for (i = 0; i < 1024; ++i) {
			rmv_ptbl_entry(proc_index, pde_index, i);
		}
		return page_index;
	} */
	return 0;
}

// Reverse operation of alloc_ptbl.
// Removes corresponding page directory entry,
// and frees the page for the page table entries (with container_free).
void free_ptbl(unsigned int proc_index, unsigned int vadr)
{
/*	unsigned int pde_index = vadr >> 22;
	unsigned int pde = get_pdir_entry(proc_index, pde_index);
	unsigned int page_index = (pde - 7) / PAGESIZE;
	int i;
	for (i = 0; i < 1024; ++i) {
		rmv_ptbl_entry(proc_index, pde_index, i);
	} */
}
