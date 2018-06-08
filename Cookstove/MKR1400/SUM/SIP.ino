//GLOBALS
float prev_temp = 0;

/**
 * Checks if the temperature reading is "interesting" and needs to be sent. The functions defines data 
 * as being eventful if:
 * The current temperature differs from the previous saved temperature by more than a defined
 * threshold
 * @param current_temp The currently sensed temperature
 * @return True if the data is eventful, else false
 */
bool hasEvent(float current_temp)
{
  return fabs(current_temp - prev_temp) >= SIP_TEMP_THRESH;
}


//Update previous temperature state
void updateState(float temp){
  prev_temp = temp;
}

