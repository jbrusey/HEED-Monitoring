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
  mu_assert("prev state should be set to 25.1", prev_temp_thermocouple == new_temp_thermocouple);
  mu_assert("prev state should be set to 23.5", prev_temp_Si7021 == new_temp_Si7021);
  mu_assert("prev state should be set to 60", prev_humidity == new_humidity);
  return 0;
}

static char* test_has_no_event(void) {

  float current_temp_thermocouple = 25;
  prev_temp_thermocouple = 25.3;

  float current_temp_Si7021 = 24;
  prev_temp_Si7021 = 24.1;

  float current_humidity = 50;
  prev_humidity = 50;

  mu_assert("test_has_no_event: should be false", !hasEvent(current_temp_thermocouple, current_temp_Si7021, current_humidity));
  return 0;
}

static char* test_has_event_from_thermocouple(void) {

  float current_temp_thermocouple = 25;
  prev_temp_thermocouple = 25.6;

  float current_temp_Si7021 = 24;
  prev_temp_Si7021 = 24.1;

  float current_humidity = 50;
  prev_humidity = 50;

  mu_assert("test_has_event_from_thermocouple: should be true", hasEvent(current_temp_thermocouple, current_temp_Si7021, current_humidity));
  return 0;
}

static char* test_has_event_from_Si7021_temp(void) {

  float current_temp_thermocouple = 25;
  prev_temp_thermocouple = 25.3;

  float current_temp_Si7021 = 24;
  prev_temp_Si7021 = 24.7;

  float current_humidity = 50;
  prev_humidity = 50;

  mu_assert("test_has_event_from_Si7021_temp: should be true", hasEvent(current_temp_thermocouple, current_temp_Si7021, current_humidity));
  return 0;
}

static char* test_has_event_from_humidity(void) {

  float current_temp_thermocouple = 25;
  prev_temp_thermocouple = 25.3;

  float current_temp_Si7021 = 24;
  prev_temp_Si7021 = 24.1;

  float current_humidity = 50;
  prev_humidity = 52.1;

  mu_assert("test_has_event_from_humidity: should be true", hasEvent(current_temp_thermocouple, current_temp_Si7021, current_humidity));
  return 0;
}

static char* test_has_event_from_all(void) {

  float current_temp_thermocouple = 25;
  prev_temp_thermocouple = 25.6;

  float current_temp_Si7021 = 24;
  prev_temp_Si7021 = 24.7;

  float current_humidity = 50;
  prev_humidity = 52.1;

  mu_assert("test_has_event_from_all: should be true", hasEvent(current_temp_thermocouple, current_temp_Si7021, current_humidity));
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
  mu_run_test(test_has_event_from_thermocouple);
  reset_test_state();
  mu_run_test(test_has_event_from_Si7021_temp);
  reset_test_state();
  mu_run_test(test_has_event_from_humidity);
  reset_test_state();
  mu_run_test(test_has_event_from_all);
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
