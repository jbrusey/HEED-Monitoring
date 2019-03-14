//Include the RTC Libraries
#include "ArduinoLowPower.h"


// TODO consider calling directly from main code
/**
 * Puts the node into its sleep mode
 */
void nodeSleep(){
  LowPower.sleep(LANTERN_SLEEP_TIME);
}

