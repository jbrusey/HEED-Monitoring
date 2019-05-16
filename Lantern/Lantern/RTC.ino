//Include the RTC Libraries
#include "ArduinoLowPower.h"

#include <RTCZero.h>
RTCZero rtc;

// TODO consider calling directly from main code
/**
 * Puts the node into its sleep mode
 */
void nodeSleep(uint32_t sleep_time){
  uint32_t end_time;
 
  end_time = rtc.getEpoch() + sleep_time;

  do {
    LowPower.sleep((end_time - rtc.getEpoch())*1000);   //time in milliseconds
  } while(end_time > rtc.getEpoch());
}

void setupNodeSleep() {
  LowPower.attachInterruptWakeup(RTC_ALARM_WAKEUP, dummy_ISR, CHANGE);
  rtc.begin();
  rtc.setEpoch(1451606400); // Jan 1, 2016
}

void nodeSleepLowBattery(){
  LowPower.sleep(60000*10);   //Sleep for 10 minutes
}

void dummy_ISR() {}
