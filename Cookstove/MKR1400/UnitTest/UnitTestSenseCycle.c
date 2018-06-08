// -*- c -*-

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

//function prototypes
String constructPkt(float temp, float node_batt, int seq);
//bool writeDataToFile(String pkt);
float getTemperature();
float getBatteryVoltage();
void connectGSM();
void disconnectGSM();
void connectMQTT();
void disconnectMQTT();
bool transmit(String topic, String dataString);

//Bring in minunit and the code to be tested
#include "minunit.h"
#include "../SUM/SUM.h"
#include "../SUM/SIP.ino"
#include "../SUM/SenseCycle.ino"

int tests_run=0;
bool pktConstructed = false;
//bool pktWrote = false;
bool pktTx = false;


void connectGSM(){}
void disconnectGSM(){}
void connectMQTT(){}
void disconnectMQTT(){}

bool transmit(String topic, String dataString) {
  pktTx = true;
}


String constructPkt(float temp, float node_batt, int seq)
{
  //dummy function, just check it is called
  pktConstructed = true;
  return (char)pktConstructed;
}

//bool writeDataToFile(String pkt)
//{
//  //dummy function, just check it is called
//  pktWrote = true;
//  return true;
//}

//initial value for solarBattery
float temp = 25;
//dummy method to retrun the solar battery voltage
float getTemperature()
{
  return temp;
}

//initial value for the nodes battery
float nodeBattery = 3.3;
//dummy method to return the node battery voltage
float getBatteryVoltage()
{
  return nodeBattery;
}

//dummy method to set the sensor values, simulates the physical values
void setSensorValues(float t, float nb){
  temp = t;
  nodeBattery = nb;
  }

void reset_test_state(void){
  pktConstructed = false;
//pktWrote = false;
  prev_temp = -1;
}

static char* test_sense(void) {

  setSensorValues(28, 3.3);
  doSenseCycle();
  mu_assert("Cycle 1: Transmit seq should be 1", seq==1);
  mu_assert("Cycle 1: Pkt constructed", pktConstructed);
//mu_assert("Cycle 1: Pkt wrote", pktWrote);
  mu_assert("Cycle 1: Pkt TX", pktTx);

  
  //reset unit test params
  pktConstructed = false;
//pktWrote = false;
  pktTx = false;


  doSenseCycle();
  mu_assert("Cycle 2: Transmit seq should be 1", seq==1);
  mu_assert("Cycle 2: Pkt not constructed", !pktConstructed);
//mu_assert("Cycle 2: Pkt not wrote", !pktWrote);
  mu_assert("Cycle 2: Pkt not TX", !pktTx);
  
  //reset unit test params
  pktConstructed = false;
//pktWrote = false;
  pktTx = false;

  setSensorValues(27, 3.3);
  doSenseCycle();
  mu_assert("Cycle 3: Transmit seq should be 2", seq==2);
  mu_assert("Cycle 3: Pkt constructed", pktConstructed);
//mu_assert("Cycle 3: Pkt wrote", pktWrote);
  mu_assert("Cycle 3: Pkt TX", pktTx);

  //reset unit test params
  pktConstructed = false;
// pktWrote = false;
  pktTx = false;

  doSenseCycle();
  mu_assert("Cycle 4: Transmit seq should be 2", seq==2);
  mu_assert("Cycle 4: Pkt not constructed", !pktConstructed);
//mu_assert("Cycle 4: Pkt not wrote", !pktWrote);
  mu_assert("Cycle 4: Pkt not TX", !pktTx);

  
  //reset unit test params
  pktConstructed = false;
//pktWrote = false;
  pktTx = false;

  setSensorValues(40, 3.3);
  doSenseCycle();
  mu_assert("Cycle 5: Transmit seq should be 3", seq==3);
  mu_assert("Cycle 5: Pkt constructed", pktConstructed);
//mu_assert("Cycle 5: Pkt wrote", pktWrote);
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
