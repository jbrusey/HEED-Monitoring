//Include the RTC Libraries
#include "ArduinoLowPower.h"

void setupNodeSleep() {
  LowPower.attachInterruptWakeup(RTC_ALARM_WAKEUP, dummy_ISR, CHANGE);
}

// TODO consider calling directly from main code
/**
 * Puts the node into its sleep mode
 */
void nodeSleep(){
  LowPower.sleep(LANTERN_SLEEP_TIME);
}

void dummy_ISR() {}
