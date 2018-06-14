#include "Lantern.h"
#include "Arduino.h"
#include "struct.h"


/**
 * Setups the program by
 * 1. Starting USB serial if in debug mode
 * 2. Setting up the RTC
 * 3. Setting up a connection to the SD Card
 * 4. Setting up all sensors
 * 5, Setting up the RTC to trigger every minute
 */
void setup() {
  delay(STARTUP_DELAY);

#ifdef PRINTF
  startSerial();
#else
  USBDevice.detach();
#endif
  setupRTC();
  setupSD();
  setupAnalogSensors();
  setupADXL345();
  setRTCAlarm(RTC_SAMPLE_TIME);
}


/**
 * MAIN LOOP: Calls for the sensing procedure to run before 
 * putting the node to sleep
 */
void loop() {
  SerialUSB.println("\n-----------------");
  debug("Start Sense");
  doSenseCycle();
  debug("End Sense");
  debugln();
  //nodeSleep();
  delay(5000);
 }




