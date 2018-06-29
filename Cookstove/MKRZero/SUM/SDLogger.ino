//Import SD card library and define variables
#include <SPI.h>
#include "SdFat.h"
SdFat SD;
#define SD_CS_PIN 4

char fileName[10] = "SUM_" NODE_ID ".csv";

SdFile file;


void _write(Data* reading){ 
  file.print(reading->unixtime);
  file.print(",");
  file.print(reading->tempThermocouple);
  file.print(",");
  file.print(reading->tempSi7021);
  file.print(",");
  file.print(reading->humidity);
  file.print(",");
  file.print(reading->nodeBatt);
  file.print(",");
  file.println(reading->seq);
}

/**
 * Configure the RTC for use. This function checks to
 * see if the card is present and can be initialized:
 */
void setupSD() {
  debug("Setting up SD");
  
  if (!SD.begin(SD_CS_PIN, SPI_HALF_SPEED)) {
    debug("SD card failed, or not present");
    return;
  }

  debug(fileName);

  debug("SD card initialized");
}


/**
 * Logs a string to a datafile
 * @param pointer to a data struct
 * @return True if the String was saved
 */
bool writeDataToFile(Data* reading)
{
  debug("SD: Write start");
  if (!file.open(fileName, O_APPEND | O_CREAT | O_WRITE )) {
    debug("SD: card failed, or not present");
    return false;
  }
  delay(10);
  _write(reading);
  
  //file.println(dataString);
  delay(10);

  if (!file.close() || file.getWriteError()) {
    debug("SD: write error");
    return false;
  }
 
  debug("SD: Write end");
  return true;
}
