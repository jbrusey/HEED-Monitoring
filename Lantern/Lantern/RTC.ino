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
    watchdog_clear();
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

/* Set up the WDT to perform a system reset if the loop() blocks for more than 16 seconds
 * https://forum.arduino.cc/index.php?topic=366836.msg3683023#msg3683023
 */
void watchdog_setup(void){
  // Set up the generic clock (GCLK2) used to clock the watchdog timer at 1.024kHz
  REG_GCLK_GENDIV = GCLK_GENDIV_DIV(4) |            // Divide the 32.768kHz clock source by divisor 32, where 2^(4 + 1): 32.768kHz/32=1.024kHz
                    GCLK_GENDIV_ID(2);              // Select Generic Clock (GCLK) 2
  while (GCLK->STATUS.bit.SYNCBUSY);                // Wait for synchronization

  REG_GCLK_GENCTRL = GCLK_GENCTRL_DIVSEL |          // Set to divide by 2^(GCLK_GENDIV_DIV(4) + 1)
                     GCLK_GENCTRL_IDC |             // Set the duty cycle to 50/50 HIGH/LOW
                     GCLK_GENCTRL_GENEN |           // Enable GCLK2
                     GCLK_GENCTRL_SRC_OSCULP32K |   // Set the clock source to the ultra low power oscillator (OSCULP32K)
                     GCLK_GENCTRL_ID(2);            // Select GCLK2         
  while (GCLK->STATUS.bit.SYNCBUSY);                // Wait for synchronization

  // Feed GCLK2 to WDT (Watchdog Timer)
  REG_GCLK_CLKCTRL = GCLK_CLKCTRL_CLKEN |           // Enable GCLK2 to the WDT
                     GCLK_CLKCTRL_GEN_GCLK2 |       // Select GCLK2
                     GCLK_CLKCTRL_ID_WDT;           // Feed the GCLK2 to the WDT
  while (GCLK->STATUS.bit.SYNCBUSY);                // Wait for synchronization

  REG_WDT_CONFIG = WDT_CONFIG_PER_16K ;             // Set the WDT reset timeout to 16 seconds
  while(WDT->STATUS.bit.SYNCBUSY);                  // Wait for synchronization
  REG_WDT_CTRL = WDT_CTRL_ENABLE;                   // Enable the WDT in normal mode
  while(WDT->STATUS.bit.SYNCBUSY);                  // Wait for synchronization
}

void watchdog_clear(void){
  if (!WDT->STATUS.bit.SYNCBUSY)                // Check if the WDT registers are synchronized
  {
    REG_WDT_CLEAR = WDT_CLEAR_CLEAR_KEY;        // Clear the watchdog timer
  }
}

void dummy_ISR() {}
