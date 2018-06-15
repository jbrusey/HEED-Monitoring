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
  float new_temp_thermocouple = 25.1;
  float new_temp_Si7021 = 23.5;
  float new_humidity = 60;
  updateState(new_temp_thermocouple, new_temp_Si7021, new_humidity);
  mu_assert("prev state should be set to 3.3", prev_temp_thermocouple == new_temp_thermocouple);
  return 0;
}

static char* test_has_no_event(void) {
  float current_temp_thermocouple = 29.2;
  float current_temp_Si7021 = 0;
  float current_humidity = 0;
  prev_temp_thermocouple = 29;
  prev_temp_Si7021 = 0;
  prev_humidity = 0;
  mu_assert("test_has_no_event: should be false", !hasEvent(current_temp_thermocouple, current_temp_Si7021, current_humidity));
  return 0;
}

static char* test_has_event(void) {
  float current_temp_thermocouple = 25;
  float current_temp_Si7021 = 0;
  float current_humidity = 0;
  prev_temp_thermocouple = 25.6;
  prev_temp_Si7021 = 0;
  prev_humidity = 0;
  mu_assert("test_has_batt_event: should be true", hasEvent(current_temp_thermocouple, current_temp_Si7021, current_humidity));
  return 0;
}

void reset_test_state(void){
  prev_temp_thermocouple = -1;
  prev_temp_Si7021 = -1;
  prev_humidity = -1;
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
