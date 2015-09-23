#include <lib/debug.h>
#include "import.h"

#define PAGESIZE	4096
#define VM_USERLO	0x40000000
#define VM_USERHI	0xF0000000
#define VM_USERLO_PI	(VM_USERLO / PAGESIZE)
#define VM_USERHI_PI	(VM_USERHI / PAGESIZE)

/**
 * The initialization function for the allocation table AT.
 * It contains two major parts:
 * 1. Calculate the actual physical memory of the machine, and sets the number of physical pages (NUM_PAGES).
 * 2. Initializes the physical allocation table (AT) implemented in MATIntro layer, based on the
 *    information available in the physical memory map table.
 *    Review import.h in the current directory for the list of avaiable getter and setter functions.
 */
void
pmem_init(unsigned int mbi_addr)
{
  unsigned int nps;

  // TODO: Define your local variables here.

  //Calls the lower layer initializatin primitives.
  //The parameter mbi_addr shell not be used in the further code.
	devinit(mbi_addr);

  /**
   * Calculate the number of actual number of avaiable physical pages and store it into the local varaible nps.
   * Hint: Think of it as the highest address possible in the ranges of the memory map table,
   *       divided by the page size.
   */
  // TODO
	unsigned int maxAddr = 0;
	int i, j, k;
	int startRg, endRg, len;
	int rSize = get_size();

	for (k = 0; k < rSize; ++k) {
		endRg = get_mms(k) + get_mml(k);
		if (endRg > maxAddr)
			maxAddr = endRg;
	}

	int remainder = maxAddr % PAGESIZE;
	if (remainder == PAGESIZE-1)
		nps = (maxAddr / PAGESIZE) + 1;
	else
		nps = maxAddr / PAGESIZE; 
	set_nps(nps); // Setting the value computed above to NUM_PAGES.
	
	int startPg, endPg;
	int set; 

	for (i = 0; i < nps; ++i)
	{
		startPg = PAGESIZE*i;
		endPg = (i+1)*PAGESIZE - 1;
		
		// if entire page is in kernel, set to 1
		if (startPg >= VM_USERHI || endPg < VM_USERLO)
			at_set_perm(i, 1);
		// if entire page is in user space
		else if (startPg >= VM_USERLO && endPg < VM_USERHI)
		{
			set = 0;
			// look for the range in which page belongs
			for (j = 0; j < rSize; ++j)
			{
				startRg = get_mms(j);
				len = get_mml(j);
				endRg = startRg + len - 1;

				// if page lies entirely in range
				if (startPg >= startRg && endPg <= endRg)
				{
					if (is_usable(j))
						at_set_perm(i, 2);
					else
						at_set_perm(i, 0);
					set = 1;
					break;
				}
				// page is half in this range, half in another
				// else if ((startPg < startRg && endPg <= endRg) || 
				// 	 (startPg >= startRg && endPg > endRg))
				// {
				// 	at_set_perm(i, 0);
				//	break;
				// }
			}
			if (!set) at_set_perm(i, 0);
		}
		// if page is half in kernel, half in user space
		else
			at_set_perm(i, 0);
	}	


  /**
   * Initialization of the physical allocation table (AT).
   *
   * In CertiKOS, the entire addresses < VM_USERLO or >= VM_USERHI are reserved by the kernel.
   * That corresponds to the physical pages from 0 to VM_USERLO_PI-1, and from VM_USERHI_PI to NUM_PAGES-1.
   * The rest of pages that correspond to addresses [VM_USERLO, VM_USERHI), can be used freely ONLY IF
   * the entire page falls into one of the ranges in the memory map table with the permission marked as usable.
   *
   * Hint:
   * 1. You have to initialize AT for all the page indices from 0 to NPS - 1.
   * 2. For the pages that are reserved by the kernel, simply set its permission to 1.
   *    Recall that the setter at_set_perm also marks the page as unallocated. 
   *    Thus, you don't have to call another function to set the allocation flag.
   * 3. For the rest of the pages, explore the memory map table to set its permission accordingly.
   *    The permission should be set to 2 only if there is a range containing the entire page that
   *    is marked as available in the memory map table. O.w., it should be set to 0.
   *    Note that the ranges in the memory map are not aligned by pages.
   *    So it may be possible that for some pages, only part of the addresses are in the ranges.
   *    Currently, we do not utilize partial pages, so in that case, you should consider those pages as unavailble.
   * 4. Every page in the allocation table shold be initialized.
   *    But the ranges in the momory map table may not cover the entire available address space.
   *    That means there may be some gaps between the ranges.
   *    You should still set the permission of those pages in allocation table to 0.
   */
  // TODO
}


