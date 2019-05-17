#pragma once
#include <SdFat.h>
#include "sip.h"

//Create JSON buffer and object

const size_t bufferSize = JSON_OBJECT_SIZE(NUM_JSON_FIELDS);
DynamicJsonBuffer jsonBuffer(bufferSize);
JsonObject& payload = jsonBuffer.createObject();

struct Packet {
  unsigned long gsmTime;
  int rawSteps;
  State steps;
  float solarBatt;
  float nodeBatt;
  uint32_t error;
  seq_t seq;

  void print(SdFile *file){
    file->print(gsmTime);
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


  /**
   * Constructs a json packet from the packet
   * @return A string of all the sensor values as json
   */
  String json()
  {
    String dataString = "";

    payload["unixtime"] = gsmTime;
    payload["NODE_ID"] = NODE_ID;
    payload["rawSteps"] = rawSteps;
    payload["steps"] =  steps.x[0];
    payload["stepsRate"] =  steps.x[1];
    payload["solarBatt"] =  solarBatt;
    payload["nodeBatt"] = nodeBatt;
    payload["error"] =  error;
    payload["seq"] =  seq;

    payload.printTo(dataString);
    return dataString;
  }

};
