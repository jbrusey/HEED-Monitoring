//Include the RTC Libraries
#include <RTCZero.h> //include the Arduino Zero's Real Time Clock library
RTCZero rtc;

/**
 * Configure the RTC for use. This function:
 * 1. Starts the RTC
 * 2. Sets the date and time based on compile time
 */
// TODO: this seems decidedly fishy! why set at all?
void setupRTC() {
  //Taken from https://www.hackster.io/jkoger/simple-watch-using-rtc-59e635
  char s_month[5];
  int tmonth, tday, tyear, thour, tminute, tsecond;
  byte seconds, minutes, hours, days, months, years;
  static const char month_names[] = "JanFebMarAprMayJunJulAugSepOctNovDec";

  //Start the RTC
  rtc.begin();
  
  // Let's let the compiler set the current date and time for us.
  // This will be a few seconds off due to the time it takes to compile the
  // .ino file and upload the app. But pretty close.
  sscanf(__DATE__, "%s %d %d", s_month, &tday, &tyear);
  sscanf(__TIME__, "%d:%d:%d", &thour, &tminute, &tsecond);
  tmonth = (strstr(month_names, s_month) - month_names) / 3;

  months = tmonth + 1;  // The RTC library expects months to be 1 - 12.
  days = tday;
  years = tyear - 2000; // The RTC library expects years to be from 2000.
  hours = thour;
  minutes = tminute;
  seconds = tsecond;

  rtc.setTime(hours, minutes, seconds);
  rtc.setDate(days, months, years);

  dbg("RTC On");
}

/**
 * ISR FUNCTION: Doesn't need to do anything, but needs to be defined
 */
void RTC_ISR() {}

/**
 * Setup the RTC alarm to trigger an interrupt every minute
 * @param seconds The second of a minute to trigger an interrupt
 */
// TODO replace with library call from LowPower
void setRTCAlarm(int seconds) {
  // Set RTC alarm to trigger an interrupt on every --:--:xx
  rtc.setAlarmSeconds(seconds);

  // Match only seconds (Periodic alarm every minute)
  rtc.enableAlarm(RTCZero::MATCH_SS);

  // Attach ISR
  rtc.attachInterrupt(RTC_ISR);
  dbg("RTC Alarm Set");
}

/**
 * Puts the node into its sleep mode
 */
// TODO remove as this duplicates LowPower.sleep()
void nodeSleep(){
  dbg("Sleep");
  
  // Equivalent of rtc.standbyMode(); from RTCZero.h library
  SCB->SCR |= SCB_SCR_SLEEPDEEP_Msk;
  __DSB(); // Executes DSB (Data Synchronization Barrier) to ensure all ongoing memory accesses have completed
  __WFI(); // Executes WFI (Wait For Interrupt) to place the device into the sleep mode specified by \ref system_set_sleepmode until woken by an interrupt
}

void getTime(Data* readings){
  readings->unixtime = rtc.getEpoch();
}
