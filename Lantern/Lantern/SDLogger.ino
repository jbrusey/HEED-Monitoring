/** @file SDLogger.ino
 *  @brief Set up SD card and write to file.  
 *
 *  This contains the functions to set up the SD card
 *  connections and write data to file.
 *  
 *  @author Kriti Bhargava
 *  @author James Brusey
 *  @bug No known bugs.
 */
 
//Import SD card library and define variables
#include <SdFat.h>
SdFat SD;

char fileName[] = "ML_" NODE_ID ".csv";

SdFile file;


/**
 * Configure the SD card for use. This function checks to
 * see if the card is present and can be initialized:
 */
void setupSD() {
  dbg("Setting up SD");

  if (!SD.begin(SD_CS_PIN, SPI_HALF_SPEED)) {
    dbg("SD card failed, or not present");
    return; //Need to turn LED on or similar
  }

  dbg(fileName);

  dbg("SD card initialized");
}

/**
 * Logs a string to a datafile
 * @param pointer to a data struct
 * @return True if the String was saved
 */
bool writeDataToFile(Packet* reading)
{
  dbg("SD: Write start");
  if (!file.open(fileName, O_APPEND | O_CREAT | O_WRITE )) {
    dbg("SD card failed, or not present");
    reportError(ERR_CSV_OPEN);
    return false;
  }

  reading->print(&file);

  // Force data to SD and update the directory entry to avoid data loss.
  if (!file.close() || file.getWriteError()) {
    dbg("SD: Write error");
    reportError(ERR_CSV_WRITE);
    return false;
  }
  dbg("SD: Write end");
  return true;
}
