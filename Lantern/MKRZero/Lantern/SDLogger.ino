//Import SD card library and define variables
#include <SPI.h>
#include "SdFat.h"
SdFat SD;
#define SD_CS_PIN SDCARD_SS_PIN

char fileName[9] = "ML_" NODE_ID ".csv";

SdFile file;


/**
 * Configure the RTC for use. This function checks to
 * see if the card is present and can be initialized:
 */
void setupSD() {
  debug("Setting up SD");
  
  if (!SD.begin(SD_CS_PIN)) {
    debug("Card failed, or not present");
    exit(0); //NEed to turn LED on or similar
  }
 
  debug("card initialized");
}


void _write(Data* reading){
  String sep = ","; //CSV seperator
  
  file.print(reading->unixtime);
  file.print(sep);
  file.print(NODE_ID);
  file.print(sep);
  file.print(reading->solarBatt);
  file.print(sep);
  file.print(reading->usage);
  file.print(sep);
  file.print(reading->charging);
  file.print(sep);
  file.print(reading->interrupt, HEX);
  file.print(sep);
  file.print(reading->inactivity);
  file.print(sep);
  file.print(reading->activity);
  file.print(sep);
  file.print(reading->nodeBatt);
  file.print(sep);
  file.println(reading->seq);
}

/**
 * Logs a string to a datafile
 * @param dataString The String to be saved to SD card
 * @return True if the String was saved
 */
bool writeDataToFile(Data* reading)
{
  debug("SD Write start");
  if (!file.open(fileName, O_APPEND | O_CREAT | O_WRITE )) {
    debug("Card failed, or not present");
    return false;
  }
  delay(10);

  _write(reading);
  
  //file.println(dataString);
  delay(10);

  // Force data to SD and update the directory entry to avoid data loss.
  if (!file.close()) {
    debug("write error");
    return false;
  }
 
  debug("SD Write end");
  return true;
}
