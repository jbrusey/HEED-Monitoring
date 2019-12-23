/** @file struct.h
 *  @brief This contains the packet structure and 
 *  function to print data to file. 
 *  
 *  @author Kriti Bhargava
 *  @author James Brusey
 *  @bug No known bugs.
 */
 
#pragma once
#include <SdFat.h>
#include "sip.h"


struct Packet {
  unsigned long rtcTime;
  int rawSteps;
  State steps;
  float solarBatt;
  float nodeBatt;
  uint32_t error;
  seq_t seq;

  void print(SdFile *file){
    file->print(rtcTime);
    file->print(",");
    file->print(rawSteps);
    file->print(",");
    file->print(steps.x[0]);
    file->print(",");
    file->print(steps.x[1]);
    file->print(",");
    file->print(solarBatt);
    file->print(",");
    file->print(nodeBatt);
    file->print(",");
    file->print(error);
    file->print(",");
    file->println(seq);
  }

};
