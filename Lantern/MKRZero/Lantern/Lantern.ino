#include "Lantern.h"
#include "Arduino.h"
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
  delay(STARTUP_DELAY); //avoids the node going to sleep straight away (avoids the node hanging during flashing)

  #ifdef PRINTF
    startSerial();
  #else
    USBDevice.detach();
  #endif

  pinMode(LED_BUILTIN, OUTPUT);
  
  setupRTC();
  setupSD();
  setupAnalogSensors();
  setupADXL345();
  //setRTCAlarm(RTC_SAMPLE_TIME);
}


/**
 * MAIN LOOP: Calls for the sensing procedure to run before 
 * putting the node to sleep
 */
void loop() {
 if (batteryLow) batteryError();
  else{
    #ifdef PRINTF
      SerialUSB.println("\n-----------------");
    #endif
    //digitalWrite(LED_BUILTIN, HIGH);    //comment out to remove LED-based debugging
    debug("Start Sense");
    doSenseCycle();
    debug("End Sense");
    //digitalWrite(LED_BUILTIN, LOW);     //comment out to remove LED-based debugging
  }
  
  #ifdef PRINTF
    delay(5000); // for debugging purposes only - keeps USB connection on
  #else
    nodeSleep(); //uncomment in final product deployment
  #endif
 }




