//Import SD card library and define variables
#include <SPI.h>
#include "SdFat.h"
SdFat SD;
#define SD_CS_PIN 4

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
    exit(0);
  }

  debug(fileName);
  if (!file.open(fileName,  O_APPEND | O_CREAT | O_WRITE )) { //CHECK WHAT THIS DOES
    debug("Card failed, or not present");
    exit(0);
  }
  
  debug("card initialized");
}


/**
 * Logs a string to a datafile
 * @param dataString The String to be saved to SD card
 * @return True if the String was saved
 */
bool writeDataToFile(String dataString)
{
  debug("SD Write start");
  file.println(dataString);
  
  // Force data to SD and update the directory entry to avoid data loss.
  if (!file.sync() || file.getWriteError()) {
    debug("write error");
    return false;
  }
  debug("SD Write end");
  return true;
}
