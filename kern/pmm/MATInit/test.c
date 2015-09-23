#include <lib/debug.h>
#include <pmm/MATIntro/export.h>

int MATInit_test1()
{
  int i;
  int nps = get_nps();
  if (nps <= 1000) {
    dprintf("test 1 failed.\n");
    return 1;
  }
  for(i = 0; i < nps; i ++) {
    if (at_is_allocated(i) != 0) {
      dprintf("allocated: %d: %d\n", i, at_is_allocated(i));
      dprintf("test 1 failed.\n");
      return 1;
    }
    if ((i < 0x40000000/4096 || i >= 0xF0000000/4096) && at_is_norm(i) != 0) {
      dprintf("norm: %d: %d\n", i, at_is_norm(i));
      dprintf("test 1 failed.\n");
      return 1;
    }
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

 // The test I've included checks if the nps is 2^20 when the maximum
 // address is 2^32 - 1. This checks two things:
 // 1. Whether the nps is correct in general
 // 2. Whether overflow can be handled correctly in nps calculation
 // To expand on 2, if nps is calculated as:
 // nps = (max_address + 1)/PAGESIZE, then there will be overflow
 // as the  max unsigned int is 2_32 - 1 on most compilers.

int MATInit_test_own()
{
  return 0;
}

int test_MATInit()
{
  return MATInit_test1() + MATInit_test_own();
}
