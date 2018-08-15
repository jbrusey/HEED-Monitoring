// -*- c -*-
#pragma GCC diagnostic ignored "-Wwrite-strings"

typedef unsigned char byte;

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>
#include "minunit.h"
#include "../Lantern/Lantern.h"
#include "../Lantern/HeartBeat.ino"


int tests_run=0;

static char* test_heartbeat_false(void) {
  heartCount=1;
  mu_assert("test_heartbeat_false: should be false", !isHeartbeat());
  return 0;
}

static char* test_heartbeat_true(void) {
  heartCount=1440;
  mu_assert("test_heartbeat_true: should be true", isHeartbeat());
  return 0;
}
static char* all_tests(void) {
  mu_run_test(test_heartbeat_false);
  mu_run_test(test_heartbeat_true);
  return 0;
}


int main()
{
  char *result = all_tests();
  printf("START TESTS\n");
  if (result != 0) {
    printf("%s\n", result);
  }
  else {
    printf("ALL TESTS PASSED\n");
  }
  printf("Tests run: %d\n", tests_run);
  return 0;
}
