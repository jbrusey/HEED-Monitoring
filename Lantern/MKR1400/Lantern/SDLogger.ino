////Import SD card library and define variables
//#include <SPI.h>
//#include "SdFat.h"
//SdFat SD;
//#define SD_CS_PIN SDCARD_SS_PIN
//
//char fileName[9] = "ML_" NODE_ID ".csv";
//
//SdFile file;
//
//
///**
// * Configure the RTC for use. This function checks to
// * see if the card is present and can be initialized:
// */
//void setupSD() {
//  debug("Setting up SD");
//  
//  if (!SD.begin(SD_CS_PIN)) {
//    debug("Card failed, or not present");
//    exit(0);
//  }
//
//  debug(fileName);
//  if (!file.open(fileName,  O_APPEND | O_CREAT | O_WRITE )) { //CHECK WHAT THIS DOES
//    debug("Card failed, or not present");
//    exit(0);
//  }
//  
//  debug("card initialized");
//}
//
///**
// * Constructs a data string from the sensed values
// * @param solar_batt The currently sensed solar lantern voltage
// * @param node_batt The currently sensed node battery voltage
// * @param interrupt The INT_SOURCE register from an ADXL345 sensor.
// * @param seq The packet sequence number
// * @return A string of all the sensor values as a csv
// */
//String constructPkt(float solar_batt, float node_batt, byte interrupt, int seq)
//{
//  debug("Construct packet");
//
//  float unixtime = rtc.getEpoch();                     
//  bool inactivity = adxl345IsActivity(interrupt);      
//  bool activity = adxl345IsInactivity(interrupt);      
//  
//  String sep = ","; //CSV seperator
//  String dataString = String(unixtime) + sep + solar_batt + sep + 
//                      String(interrupt, HEX) + sep + inactivity + sep + 
//                      activity + sep + node_batt + sep + seq;
//  debug(dataString);
//  
//  return dataString;
//}
//
//
///**
// * Logs a string to a datafile
// * @param dataString The String to be saved to SD card
// * @return True if the String was saved
// */
//bool writeDataToFile(String dataString)
//{
//  debug("SD Write start");
//  file.println(dataString);
//  
//  // Force data to SD and update the directory entry to avoid data loss.
//  if (!file.sync() || file.getWriteError()) {
//    debug("write error");
//    return false;
//  }
//  debug("SD Write end");
//  return true;
//}
