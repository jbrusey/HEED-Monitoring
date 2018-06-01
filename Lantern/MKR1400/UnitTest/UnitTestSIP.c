// -*- c -*-

typedef unsigned char byte;

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>
#include "minunit.h"
#include "../Lantern/Lantern.h"
#include "../Lantern/SIP.ino"

int tests_run=0;
bool pktConstructed = false;
bool pktWrote = false;

void constructPkt(float solar_batt, float node_batt, byte interrupt, int seq)
{
  //dummy function, just check it is called
  pktConstructed = true;
}

bool writeDataToFile()
{
  //dummy function, just check it is called
  pktWrote = true;
  return true;
}


static char* test_update_state(void) { 
  float new_state = 3.3;
  updateState(new_state);
  mu_assert("prev state should be set to 3.3", prev_solat_batt == new_state);
  return 0;
}

static char* test_has_no_event(void) { 
  float cv = 3.3;
  byte inter = 0x83;
  prev_solat_batt = 3.3;
  mu_assert("test_has_no_event: should be false", !hasEvent(cv, inter));
  return 0;
}

static char* test_has_batt_event(void) { 
  float cv = 3.3;
  byte inter = 0x83;
  prev_solat_batt = 3.1;
  mu_assert("test_has_batt_event: should be true", hasEvent(cv, inter));
  return 0;
}

static char* test_has_int_event(void) { 
  float cv = 3.3;
  byte inter = 0x9B;
  prev_solat_batt = 3.3;
  mu_assert("test_has_int_event: should be true", hasEvent(cv, inter));
  return 0;
}

void reset_test_state(void){
  pktConstructed = false;
  pktWrote = false;
  prev_solat_batt = -1;
}

static char* all_tests(void) {
  reset_test_state();
  mu_run_test(test_update_state);
  reset_test_state();
  mu_run_test(test_has_no_event);
  reset_test_state();
  mu_run_test(test_has_batt_event);
  reset_test_state();
  mu_run_test(test_has_int_event);
  reset_test_state();
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
