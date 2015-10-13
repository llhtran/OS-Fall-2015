#include <lib/debug.h>
#include "export.h"

int MContainer_test1()
{
  if (container_get_quota(0) <= 10000) {
    dprintf("test 1 failed.\n");
    return 1;
  }
  if (container_can_consume(0, 10000) != 1) {
    dprintf("test 1 failed.\n");
    return 1;
  }
  if (container_can_consume(0, 10000000) != 0) {
    dprintf("test 1 failed.\n");
    return 1;
  }
  dprintf("test 1 passed.\n");
  return 0;
}


int MContainer_test2()
{
  unsigned int old_usage = container_get_usage(0);
  unsigned int old_nchildren = container_get_nchildren(0);
  unsigned int chid = container_split(0, 100);
  if (container_get_quota(chid) != 100 || container_get_parent(chid) != 0 ||
      container_get_usage(chid) != 0 || container_get_nchildren(chid) != 0 ||
      container_get_usage(0) != old_usage + 100 || container_get_nchildren(0) != old_nchildren + 1) {
		dprintf("old_usage %d old_nchildrend %d chid %d\n", old_usage, old_nchildren, chid);
		dprintf("child quota %d parent %d chid usage %d chid children %d\n", container_get_quota(chid), container_get_parent(chid), container_get_usage(chid), container_get_nchildren(chid));
		dprintf("test 2.1 failed.\n");
    return 1;
  }
  container_alloc(chid);
  if (container_get_usage(chid) != 1) {
    dprintf("test 2.2 failed.\n");
    return 1;
  }
  dprintf("test 2 passed.\n");
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
int MContainer_test_own()
{
  // TODO (optional)
  // dprintf("own test passed.\n");
  return 0;
}

int test_MContainer()
{
  return MContainer_test1() + MContainer_test2() + MContainer_test_own();
}
