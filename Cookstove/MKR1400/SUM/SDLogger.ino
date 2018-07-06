//Import SD card library and define variables
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
  file.print(reading->error);
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
    return; //Need to turn LED on or similar
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
  bool csv_write_res = false;
  debug("SD: Write start");
  if (!file.open(fileName, O_APPEND | O_CREAT | O_WRITE )) {
    debug("SD Card failed, or not present");
    reportError(ERR_CSV_OPEN);
    return csv_write_res;
  }
  
  delay(10);
  _write(reading);
  delay(10);

  // Force data to SD and update the directory entry to avoid data loss.
  if (!file.close() || file.getWriteError()) {
    debug("SD: Write error");
    reportError(ERR_CSV_WRITE);
    return csv_write_res;
  }
  csv_write_res = true;
  debug("SD: Write end");
  return csv_write_res;
}
