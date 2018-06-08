// -*- c -*-

typedef unsigned char byte;

#include <stdio.h>
#include <stdlib.h>

int tests_run=0;

typedef unsigned char String;

#include <stdbool.h>
#include <math.h>
#include "minunit.h"
#include "../SUM/SUM.h"
#include "../SUM/SIP.ino"


static char* test_update_state(void) { 
  float new_state = 25.1;
  updateState(new_state);
  mu_assert("prev state should be set to 3.3", prev_temp == new_state);
  return 0;
}

static char* test_has_no_event(void) { 
  float cv = 29.2;
  prev_temp = 29;
  mu_assert("test_has_no_event: should be false", !hasEvent(cv));
  return 0;
}

static char* test_has_event(void) { 
  float cv = 25;
  prev_temp = 25.6;
  mu_assert("test_has_batt_event: should be true", hasEvent(cv));
  return 0;
}

void reset_test_state(void){
  prev_temp = -1;
}

static char* all_tests(void) {
  reset_test_state();
  mu_run_test(test_update_state);
  reset_test_state();
  mu_run_test(test_has_no_event);
  reset_test_state();
  mu_run_test(test_has_event);
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
