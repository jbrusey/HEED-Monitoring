#include <SdFat.h>

struct Data {
  uint32_t unixtime;
  float solarBatt;
  int steps;
  float nodeBatt;
  uint32_t error;
  uint32_t seq;

  void print(SdFile *file){
    file->print(this->unixtime);
    file->print(",");
    file->print(this->solarBatt);
    file->print(",");
    file->print(this->steps);
    file->print(",");
    file->print(this->nodeBatt);
    file->print(",");
    file->print(this->error);
    file->print(",");
    file->println(this->seq);
  }

};
