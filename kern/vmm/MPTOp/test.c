#include <lib/debug.h>
#include "export.h"

int MPTOp_test1()
{
  unsigned int vaddr = 4096*1024*300;
  if (get_ptbl_entry_by_va(10, vaddr) != 0) {
    dprintf("test 1 failed.\n");
    return 1;
  }
  if (get_pdir_entry_by_va(10, vaddr) != 0) {
    dprintf("test 1 failed.\n");
    return 1;
  }
  set_pdir_entry_by_va(10, vaddr, 100);
  set_ptbl_entry_by_va(10, vaddr, 100, 259);
  if (get_ptbl_entry_by_va(10, vaddr) == 0) {
    dprintf("test 1 failed.\n");
    return 1;
  }
  if (get_pdir_entry_by_va(10, vaddr) == 0) {
    dprintf("test 1 failed.\n");
    return 1;
  }
  rmv_ptbl_entry_by_va(10, vaddr);
  rmv_pdir_entry_by_va(10, vaddr);
  if (get_ptbl_entry_by_va(10, vaddr) != 0) {
    dprintf("test 1 failed.\n");
    return 1;
  }
  if (get_pdir_entry_by_va(10, vaddr) != 0) {
    dprintf("test 1 failed.\n");
    return 1;
  }
  dprintf("test 1 passed.\n");
  return 0;
}

/**
 * Write Your Own Test Script (optional)
 *
 * Come up with your own interesting test cases to challenge your classmates!
 * In addition to the provided simple tests, selected (correct and interesting) test functions
 * will be used in the actual grading of the lab!
 * Your test function itself will not be graded. So don't be afraid of submitting a wrong script.
 *
 * The test function should return 0 for passing the test and a non-zero code for failing the test.
 * Be extra careful to make sure that if you overwrite some of the kernel data, they are set back to
 * the original value. O.w., it may make the future test scripts to fail even if you implement all
 * the functions correctly.
 */
int MPTOp_test_own()
{
  // TODO (optional)
  // dprintf("own test passed.\n");
  return 0;
}

int test_MPTOp()
{
  return MPTOp_test1() + MPTOp_test_own();
}
