// -*- c -*-
#pragma GCC diagnostic ignored "-Wwrite-strings"

typedef unsigned char byte;

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>
#include "minunit.h"
#include "../Lantern/struct.h"
#include "../Lantern/Lantern.h"

#undef SIP_SOLAR_BATTERY_THRESH
#undef SIP_STATE_THRESH
#define SIP_SOLAR_BATTERY_THRESH 0.1
#define SIP_STATE_THRESH 50

#include "../Lantern/SIP.ino"

int tests_run=0;
Data* readings = new Data();

static char* test_update_state(void) {
  readings->solarBatt=3.3;
  readings->usage=400.2;
  readings->charging=100.9;
  updateState(readings);
  mu_assert("prev solarBatt should be set to 3.3", prev_solat_batt == readings->solarBatt);
  mu_assert("prev usage should be set to 400.2", prev_usage == readings->usage);
  mu_assert("prev charging should be set to 100.9", prev_charging == readings->charging);
  return 0;
}

static char* test_has_no_event(void) {
  readings->solarBatt = 3.3;
  readings->interrupt = 0x83;
  readings->usage = 400.2;
  readings->charging = 100.9;

  prev_solat_batt = 3.3;
  prev_usage = 400.2;
  prev_charging = 100.9;
  mu_assert("test_has_no_event: should be false", !hasEvent(readings));
  return 0;
}

static char* test_has_batt_event(void) {
  readings->solarBatt = 3.3;
  readings->interrupt = 0x83;
  readings->usage = 400.2;
  readings->charging = 100.9;

  prev_solat_batt = 3.1;
  prev_usage = 400.2;
  prev_charging = 100.9;
  
  mu_assert("test_has_batt_event: should be true", hasEvent(readings));
  return 0;
}

static char* test_has_int_event(void) {
  readings->solarBatt = 3.3;
  readings->interrupt = 0x9B;
  readings->usage = 400.2;
  readings->charging = 100.9;

  prev_solat_batt = 3.3;
  prev_usage = 400.2;
  prev_charging = 100.9;
  mu_assert("test_has_int_event: should be true", hasEvent(readings));
  return 0;
  }

static char* test_has_usage_event(void) {
  readings->solarBatt = 3.3;
  readings->interrupt = 0x83;
  readings->usage = 400.2;
  readings->charging = 100.9;

  prev_solat_batt = 3.3;
  prev_usage = 550.2;
  prev_charging = 100.9;
  mu_assert("test_has_usage_event: should be true", hasEvent(readings));
  return 0;
}


static char* test_has_charging_event(void) {
  readings->solarBatt = 3.3;
  readings->interrupt = 0x83;
  readings->usage = 400.2;
  readings->charging = 100.9;

  prev_solat_batt = 3.3;
  prev_usage = 400.2;
  prev_charging = 350.9;
  mu_assert("test_has_charging_event: should be true", hasEvent(readings));
  return 0;
}
void reset_test_state(void){
  prev_solat_batt = -1;
  prev_usage = -1;
  prev_charging = -1;
  readings = new Data();
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
  mu_run_test(test_has_usage_event);
  reset_test_state();
  mu_run_test(test_has_charging_event);
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
