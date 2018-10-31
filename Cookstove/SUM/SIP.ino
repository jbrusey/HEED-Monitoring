//GLOBALS
float prev_temp_thermocouple = -1;
float prev_temp_Si7021 = -1;
float prev_humidity = -1;

/**
 * Checks if the temperature/humidity reading is "interesting" and needs to be sent. The functions defines data 
 * as being eventful if:
 * The current temperature/humidity differs from the previous saved values by more than a defined
 * threshold
 * @param readings Contains all data fetched from sensors during sense cycle (such as temp, humidity etc.)
 * @return True if the data is eventful, else false
 */
bool hasEvent(Data* readings)
{
  if (fabs(readings->tempThermocouple - prev_temp_thermocouple) >= SIP_TEMP_THERMOCOUPLE_THRESH)
  {
    debug("Threshold reached! (" + String(readings->tempThermocouple - prev_temp_thermocouple) + "c from thermocouple)");
    debug("Thermocouple temp: " + String(readings->tempThermocouple)); 
    return true;
  }
  else if (fabs(readings->tempSi7021 - prev_temp_Si7021) >= SIP_TEMP_SI7021_THRESH)
  {
    debug("Threshold reached! (" + String(readings->tempSi7021 - prev_temp_Si7021) + "c from Si7021 temp)"); 
    return true;
  }
  else if (fabs(readings->humidity - prev_humidity) >= SIP_HUMIDITY_THRESH)
  {
    debug("Threshold reached! (" + String(readings->humidity - prev_humidity) + "% from humidity)"); 
    return true;
  }

  debug("Threshold not reached. Skipping...");
  return false;
}


//Update previous temperature state
void updateState(Data* readings)
{
  prev_temp_thermocouple = readings->tempThermocouple;
  prev_temp_Si7021 = readings->tempSi7021;
  prev_humidity = readings->humidity;
}

