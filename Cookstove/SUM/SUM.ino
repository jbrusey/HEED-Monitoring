#include <ArduinoJson.h>
#include "SUM.h" // Header file with consts/config
#include "struct.h" // Header file with data struct fetched in sense cycle
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
  #endif

  setupRTC();
  setRTCAlarm(RTC_SAMPLE_TIME);
  
  #ifdef STORE
    setupSD();
  #endif
  
  setupAnalogSensors();
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
    
    doSenseCycle();
  }
   
  #ifdef DEBUG
    delay(5000); // keeps USB connection on instead
  #else
    nodeSleep(); //go back to sleep
  #endif
}




