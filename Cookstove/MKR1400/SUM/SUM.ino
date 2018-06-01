#include "SUM.h" //Header file with consts/config



/**
 * Setups the program by
 * 1. Starting USB serial if in debug mode
 * 2. Setting up the RTC
 * 4. Setting up all sensors
 * 5, Setting up the RTC to trigger every minute
 */
void setup() {
#ifdef PRINTF
  startSerial();
#else
  USBDevice.detach();
#endif

  setupRTC();
  setupAnalogSensors();
  setRTCAlarm(RTC_SAMPLE_TIME);
  delay(STARTUP_DELAY); //avoids the node going to sleep straight away (avoids the node hanging during flashing)
}

/**
 * MAIN LOOP: Calls for the sensing procedure to run before 
 * putting the node to sleep
 */
void loop() {
  debug("Start Sense");
  doSenseCycle(); //sense and send
  debug("End Sense");
  nodeSleep(); //go back to sleep
}




