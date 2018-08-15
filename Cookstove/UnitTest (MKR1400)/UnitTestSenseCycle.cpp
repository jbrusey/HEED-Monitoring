// -*- c -*-
#pragma GCC diagnostic ignored "-Wwrite-strings"

//bring in required standard libraries
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>

#include "../SUM/struct.h"
#include "../SUM/error.h"

//define missing types
typedef unsigned char byte;
typedef unsigned char String;

//Dummy topic, doesn't matter what it is set to
const String MQTT_TOPIC = 1;

//function prototypes
String constructPkt(Data* readings);
bool writeDataToFile(Data* readings);
void getTemperatureThermocouple(Data* readings);
void getSi7021Data(Data* readings);
void getBatteryVoltage(Data* readings);
void getGSMTime(Data* readings);
void nodeFunctional();

void connectGSM();
void disconnectGSM();
void connectMQTT();
void disconnectMQTT();
void _DO_NOTHING();
bool transmit(String topic, String dataString);

//Bring in minunit and the code to be tested
#include "minunit.h"
#include "../SUM/SUM.h"
#include "../SUM/SIP.ino"
#include "../SUM/SenseCycle.ino"

int tests_run=0;
bool pktConstructed = false;
bool pktWrote = false;
bool pktTx = false;


void connectGSM(){}
void disconnectGSM(){}
void connectMQTT(){}
void disconnectMQTT(){}
void nodeFunctional(){}

void _DO_NOTHING(){}

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

//initial values
float temp_thermocouple = 25;
float temp_Si7021 = 24;
float humidity = 50;
float nodeBattery = 3.3;
float GSMTime = 0;

//dummy method to return the temperature of Thermocouple
void getTemperatureThermocouple(Data* readings)
{
  readings->tempThermocouple = temp_thermocouple;
}

//dummy method to return the Si7021 data
void getSi7021Data(Data* readings)
{
  readings->tempSi7021 = temp_Si7021;
  readings->humidity = humidity;
}

//dummy method to return the node battery voltage
void getBatteryVoltage(Data* readings)
{
  readings->nodeBatt = nodeBattery;
}

//dummy method to return the node battery voltage
void getGSMTime(Data* readings)
{
  readings->unixtime = GSMTime;
}

//dummy method to set the sensor values, simulates the physical values
void setSensorValues(float tt, float ts, float h, float nb){
  temp_thermocouple = tt;
  temp_Si7021 = ts;
  humidity = h;
  nodeBattery = nb;
  }

void reset_test_state(void){
  pktConstructed = false;
  pktWrote = false;
  prev_temp_thermocouple = -1;
  prev_temp_Si7021 = -1;
  prev_humidity = -1;
}

static char* test_sense(void) {

  setSensorValues(28, 26, 50, 3.3);
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

  setSensorValues(27, 24, 34, 3.3);
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

  setSensorValues(40, 56, 34, 3.3);
  doSenseCycle();
  mu_assert("Cycle 5: Transmit seq should be 3", seq==3);
  mu_assert("Cycle 5: Pkt constructed", pktConstructed);
  mu_assert("Cycle 5: Pkt wrote", pktWrote);
  mu_assert("Cycle 5: Pkt TX", pktTx);


  return 0;
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
