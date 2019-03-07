#include "SUM.h"
int heartCount = 0;

bool isHeartbeat(){
  heartCount++; //TODO predicate should not have side-effects
  if (heartCount > HEART_LIMIT){
    heartCount=0;
    return true;
  }
  return false;
}

