//GLOBALS
float prev_temp_thermocouple = 0;
float prev_temp_Si7021 = 0;
float prev_humidity = 0;

/**
 * Checks if the temperature/humidity reading is "interesting" and needs to be sent. The functions defines data 
 * as being eventful if:
 * The current temperature/humidity differs from the previous saved values by more than a defined
 * threshold
 * @param current_temp_thermocouple The currently sensed temperature by thermocouple
 * @param current_temp_Si7021 The currently sensed temperature by Si7021 sensor
 * @param current_humidity The currently sensed humidity by Si7021 sensor
 * @return True if the data is eventful, else false
 */
bool hasEvent(float current_temp_thermocouple, float current_temp_Si7021, float current_humidity)
{
  if ((fabs(current_temp_thermocouple - prev_temp_thermocouple) >= SIP_TEMP_THERMOCOUPLE_THRESH) 
  || (fabs(current_temp_Si7021 - prev_temp_Si7021) >= SIP_TEMP_SI7021_THRESH) 
  || (fabs(current_humidity - prev_humidity) >= SIP_HUMIDITY_THRESH)) 
    return true;

  return false;
}


//Update previous temperature state
void updateState(float temp_thermocouple, float temp_Si7021, float humidity)
{
  prev_temp_thermocouple = temp_thermocouple;
  prev_temp_Si7021 = temp_Si7021;
  prev_humidity = humidity;
}

