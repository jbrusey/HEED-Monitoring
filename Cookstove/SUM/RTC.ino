//Include the RTC Libraries
#include "ArduinoLowPower.h"


/**
 * Puts the node into its sleep mode
 */
void nodeSleep(){
  LowPower.sleep(CS_SLEEP_TIME);
}

void setupNodeSleep() {
  LowPower.attachInterruptWakeup(RTC_ALARM_WAKEUP, dummy_ISR, CHANGE);
}

void nodeSleepLowBattery(){
  LowPower.sleep(CS_LOW_BATTERY_SLEEP);   //Sleep for 10 minutes
}

/* Function called on waking up by LowPower
 */
void dummy_ISR() {}
