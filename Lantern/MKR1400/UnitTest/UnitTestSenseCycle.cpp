// -*- c -*-
#pragma GCC diagnostic ignored "-Wwrite-strings"

//bring in required standard libraries
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>

//define missing types
typedef unsigned char byte;
typedef unsigned char String;

#include "../Lantern/struct.h"

//function prototypes
String constructPkt(Data* readings);
bool writeDataToFile(String pkt);
void getSolarBatteryVoltage(Data* readings);
void getBatteryVoltage(Data* readings);
void adxl345GetInterrupt(Data* readings);
void getLanternState(Data* readings);
void getTime(Data* readingreadings);

//Bring in minunit and the code to be tested
#include "minunit.h"
#include "../Lantern/Lantern.h"
#include "../Lantern/SIP.ino"
#include "../Lantern/SenseCycle.ino"

int tests_run=0;
bool pktConstructed = false;
bool pktWrote = false;

String constructPkt(Data* readings)
{
  //dummy function, just check it is called
  pktConstructed = true;
  return (char)pktConstructed;
}

bool writeDataToFile(String pkt)
{
  //dummy function, just check it is called
  pktWrote = true;
  return true;
}

//initial value for solarBattery
float solarBattery = 3.3;
//dummy method to retrun the solar battery voltage
void getSolarBatteryVoltage(Data* readings)
{
  readings->solarBatt = solarBattery;
}

//initial value for the nodes battery
float nodeBattery = 3.3;
//dummy method to return the node battery voltage
void getBatteryVoltage(Data* readings)
{
  readings->nodeBatt = solarBattery;
}

void getLanternState(Data* readings)
{
}

void getTime(Data* readings)
{
}

//initial value for the adxl345 interrupt
float adxl345Int = 0x83;
//dummy method to return the adxl345 interrupt register
void adxl345GetInterrupt(Data* readings)
{
  readings->interrupt = adxl345Int;
}

//dummy method to set the sensor values, simulates the physical values
void setSensorValues(float sb, float nb, float acclInt){
  solarBattery = sb;
  nodeBattery = nb;
  adxl345Int = acclInt;
  }


static char* test_sense(void) {

  setSensorValues(3.3, 3.3, 0x83);
  doSenseCycle();
  mu_assert("Cycle 1: Transmit seq should be 1", seq==1);
  mu_assert("Cycle 1: Pkt constructed", pktConstructed);
  mu_assert("Cycle 1: Pkt wrote", pktWrote);
  
  //reset unit test params
  pktConstructed = false;
  pktWrote = false;

  doSenseCycle();
  mu_assert("Cycle 2: Transmit seq should be 1", seq==1);
  mu_assert("Cycle 2: Pkt not constructed", !pktConstructed);
  mu_assert("Cycle 2: Pkt not wrote", !pktWrote);
  
  //reset unit test params
  pktConstructed = false;
  pktWrote = false;

  setSensorValues(3.1, 3.3, 0x83);
  doSenseCycle();
  mu_assert("Cycle 3: Transmit seq should be 2", seq==2);
  mu_assert("Cycle 3: Pkt constructed", pktConstructed);
  mu_assert("Cycle 3: Pkt wrote", pktWrote);
  
  //reset unit test params
  pktConstructed = false;
  pktWrote = false;

  doSenseCycle();
  mu_assert("Cycle 4: Transmit seq should be 2", seq==2);
  mu_assert("Cycle 4: Pkt not constructed", !pktConstructed);
  mu_assert("Cycle 4: Pkt not wrote", !pktWrote);
  
  //reset unit test params
  pktConstructed = false;
  pktWrote = false;

  setSensorValues(3.2, 3.3, 0x9A);
  doSenseCycle();
  mu_assert("Cycle 5: Transmit seq should be 3", seq==3);
  mu_assert("Cycle 5: Pkt constructed", pktConstructed);
  mu_assert("Cycle 5: Pkt wrote", pktWrote);
  
  return 0;
}



void reset_test_state(void){
  pktConstructed = false;
  pktWrote = false;
  prev_solat_batt = -1;
}

static char* all_tests(void) {
  reset_test_state();
  mu_run_test(test_sense);

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
