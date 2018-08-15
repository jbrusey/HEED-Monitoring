#include <ArduinoJson.h>
#include "Lantern.h"
#include "struct.h"
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
    delay(STARTUP_DELAY); //avoids the node going to sleep straight away (avoids the node hanging during flashing)
    USBDevice.detach();
    setupRTC();
    setRTCAlarm(RTC_SAMPLE_TIME);
  #endif
  
  #ifdef STORE
    setupSD();
  #endif
  
  setupAnalogSensors();
  setupADXL345();
}


/**
 * MAIN LOOP: Calls for the sensing procedure to run before 
 * putting the node to sleep
 */
void loop() {
 if (batteryLow) 
    batteryError();
 else{
    #ifdef DEBUG
      SerialUSB.println("\n-----------------");
    #endif
    debug("Start Sense");
    doSenseCycle();
    debug("End Sense");
  }
   
  #ifdef DEBUG
    delay(5000); // keeps USB connection on instead
  #else
    nodeSleep(); //go back to sleep
  #endif
}





