//Include the RTC Libraries
#include <RTCZero.h> //include the Arduino Zero's Real Time Clock library
RTCZero rtc;

/**
 * Configure the RTC for use the method:
 * 1. Starts the RTC
 * 2. Sets the date and time based on compile time
 */
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

  debug("RTC On");
}

/**
 * ISR FUNCTION: Doesn't need to do anything, but needs to be defined
 */
void RTC_ISR() {}

/**
 * Setup the RTC alarm to trigger an interrupt every minute
 * @param seconds The second of a minute to trigger an interruot
 */
void setRTCAlarm(int seconds) {
  // Set RTC alarm to trigger an interrupt on every --:--:xx
  rtc.setAlarmSeconds(seconds);

  // Match only seconds (Periodic alarm every minute)
  rtc.enableAlarm(RTCZero::MATCH_SS);

  // Attach ISR
  rtc.attachInterrupt(RTC_ISR);
  debug("RTC Alarm Set");
}

/**
 * Puts the node into its sleep mode
 */
void nodeSleep(){
  debug("Sleep");
  rtc.standbyMode();
}

void getTime(Data* readings){
  readings->unixtime = rtc.getEpoch();
}

