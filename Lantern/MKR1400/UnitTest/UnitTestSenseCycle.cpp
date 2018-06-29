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

//Dummy topic, doesn't matter what it is set to
const String MQTT_TOPIC = 1;

#include "../Lantern/struct.h"
#include "../Lantern/error.h"

//function prototypes
String constructPkt(Data* readings);
bool writeDataToFile(Data* readings);
void getSolarBatteryVoltage(Data* readings);
void getBatteryVoltage(Data* readings);
void adxl345GetInterrupt(Data* readings);
void getLanternState(Data* readings);
void getTime(Data* readingreadings);
void getLanternState(Data* readings);
void nodeFunctional();

bool connectGSM();
void disconnectGSM();
bool connectMQTT();
void disconnectMQTT();
bool transmit(String topic, String dataString);

//Bring in minunit and the code to be tested
#include "minunit.h"
#include "../Lantern/Lantern.h"

#undef SIP_SOLAR_BATTERY_THRESH
#undef SIP_STATE_THRESH
#define SIP_SOLAR_BATTERY_THRESH 0.1
#define SIP_STATE_THRESH 50
#undef LEDS

#include "../Lantern/SIP.ino"
#include "../Lantern/SenseCycle.ino"

int tests_run=0;
bool pktConstructed = false;
bool pktWrote = false;
bool pktTx = false;

void nodeFunctional(){}
bool connectGSM(){}
void disconnectGSM(){}
bool connectMQTT(){}
void disconnectMQTT(){}

bool transmit(String topic, String dataString)
{
  //dummy function, just check it is called
  pktTx = true;
  return pktTx;
}

String constructPkt(Data* readings)
{
  //dummy function, just check it is called
  pktConstructed = true;
  return (char)pktConstructed;
}

bool writeDataToFile(Data* readings)
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

float usage=-1;
float charging=-1;
void getLanternState(Data* readings)
{
  readings->usage = usage;
  readings->charging = charging;
}

void getTime(Data* readings){}

//initial value for the adxl345 interrupt
float adxl345Int = 0x83;
//dummy method to return the adxl345 interrupt register
void adxl345GetInterrupt(Data* readings)
{
  readings->interrupt = adxl345Int;
}

//dummy method to set the sensor values, simulates the physical values
void setSensorValues(float sb, float nb, float acclInt, float ug, float cg){
  solarBattery = sb;
  nodeBattery = nb;
  adxl345Int = acclInt;
  usage = ug;
  charging = cg;
  }


static char* test_sense(void) {

  setSensorValues(3.3, 3.3, 0x83, 400.9, 100.2);
  doSenseCycle();
  mu_assert("Cycle 1: Transmit seq should be 1", seq==1);
  mu_assert("Cycle 1: Pkt constructed", pktConstructed);
  mu_assert("Cycle 1: Pkt wrote", pktWrote);
  mu_assert("Cycle 1: Pkt TX", pktTx);
  
  //reset unit test params
  pktConstructed = false;
  pktWrote = false;
  pktTx = false;

  doSenseCycle();
  mu_assert("Cycle 2: Transmit seq should be 1", seq==1);
  mu_assert("Cycle 2: Pkt not constructed", !pktConstructed);
  mu_assert("Cycle 2: Pkt not wrote", !pktWrote);
  mu_assert("Cycle 2: Pkt not TX", !pktTx);
  
  //reset unit test params
  pktConstructed = false;
  pktWrote = false;
  pktTx = false;

  setSensorValues(3.1, 3.3, 0x83, 400.9, 100.2);
  doSenseCycle();
  mu_assert("Cycle 3: Transmit seq should be 2", seq==2);
  mu_assert("Cycle 3: Pkt constructed", pktConstructed);
  mu_assert("Cycle 3: Pkt wrote", pktWrote);
  mu_assert("Cycle 3: Pkt TX", pktTx);

    
  //reset unit test params
  pktConstructed = false;
  pktWrote = false;
  pktTx = false;

  doSenseCycle();
  mu_assert("Cycle 4: Transmit seq should be 2", seq==2);
  mu_assert("Cycle 4: Pkt not constructed", !pktConstructed);
  mu_assert("Cycle 4: Pkt not wrote", !pktWrote);
  mu_assert("Cycle 4: Pkt not TX", !pktTx);
    
  //reset unit test params
  pktConstructed = false;
  pktWrote = false;
  pktTx = false;
  
  setSensorValues(3.2, 3.3, 0x9A, 400.9, 100.2);
  doSenseCycle();
  mu_assert("Cycle 5: Transmit seq should be 3", seq==3);
  mu_assert("Cycle 5: Pkt constructed", pktConstructed);
  mu_assert("Cycle 5: Pkt wrote", pktWrote);
  mu_assert("Cycle 5: Pkt TX", pktTx);

  //reset unit test params
  pktConstructed = false;
  pktWrote = false;
  pktTx = false;

  setSensorValues(3.2, 3.3, 0x9A, 200.9, 100.2);
  doSenseCycle();
  mu_assert("Cycle 6: Transmit seq should be 4", seq==4);
  mu_assert("Cycle 6: Pkt constructed", pktConstructed);
  mu_assert("Cycle 6: Pkt wrote", pktWrote);
  mu_assert("Cycle 6: Pkt TX", pktTx);

  //reset unit test params
  pktConstructed = false;
  pktWrote = false;
  pktTx = false;

  setSensorValues(3.2, 3.3, 0x9A, 200.9, 600.2);
  doSenseCycle();
  mu_assert("Cycle 7: Transmit seq should be 5", seq==5);
  mu_assert("Cycle 7: Pkt constructed", pktConstructed);
  mu_assert("Cycle 7: Pkt wrote", pktWrote);
  mu_assert("Cycle 7: Pkt TX", pktTx);

  
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
