/** @file struct.h
 *  @brief This contains the packet structure and 
 *  function to print data to file. 
 *  
 *  @author Kriti Bhargava
 *  @author James Brusey
 *  @bug No known bugs.
 */
 
#include <SdFat.h>

struct Data {
  uint32_t unixtime;
  float tempThermocouple;
  float tempSi7021;
  float humidity;
  uint32_t error;
  uint32_t seq;
  uint16_t nodeBatt;

  void print(SdFile *file){
    file->print(this->unixtime);
    file->print(",");
    file->print(this->tempThermocouple);
    file->print(",");
    file->print(this->tempSi7021);
    file->print(",");
    file->print(this->humidity);
    file->print(",");
    file->print(this->nodeBatt);
    file->print(",");
    file->print(this->error);
    file->print(",");
    file->println(this->seq);
  }

};
