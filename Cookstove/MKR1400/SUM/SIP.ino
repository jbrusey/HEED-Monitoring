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
  return abs(current_temp - prev_temp) > SIP_TEMP_THRESH;
}

/**
 * Constructs a data string from the sensed values
 * @param temp The currently sensed solar lantern voltage
 * @param node_batt The currently sensed node battery voltage
 * @param seq The packet sequence number
 * @return A string of all the sensor values as a csv
 */
 String constructMQTTPacket(float temp, float node_batt, int seq) {
  String sep = ",";  //CSV seperator
  String dataString = String(NODE_ID) + sep + gsmAccess.getTime() + sep + temp + sep + node_batt + sep + seq;
  debug(dataString);
  return dataString;
}

//Update previous temperature state
void updateState(float temp){
  prev_temp = temp;
}

