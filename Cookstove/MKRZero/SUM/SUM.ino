#include "SUM.h" //Header file with consts/config
#include "struct.h" //Header file with data struct fetched in sense cycle
#include "error.h"

/**
 * Setups the program by
 * 1. Starting USB serial if in debug mode
 * 2. Setting up the RTC
 * 3. Setting up a connection to the SD Card
 * 4. Setting up all sensors
 * 5, Setting up the RTC to trigger every minute
 */
void setup() {
  #ifdef DEBUG
    startSerial();
  #else
    USBDevice.detach();
  #endif

  setupRTC();
  setupSD();
  setupAnalogSensors();
  setRTCAlarm(RTC_SAMPLE_TIME);

  delay(STARTUP_DELAY); //avoids the node going to sleep straight away (avoids the node hanging during flashing)
}

/**
 * MAIN LOOP: Calls for the sensing procedure to run before 
 * putting the node to sleep
 */
void loop() {
 if (batteryLow) batteryError();
  else{
    SerialUSB.println("\n-----------------");
    debug("Start Sense");
    doSenseCycle();
    debug("End Sense");
  }
   
  #ifdef DEBUG
    nodeSleep(); //go back to sleep
  #else
    delay(5000); // for debugging purposes only - keeps USB connection on
  #endif
}
