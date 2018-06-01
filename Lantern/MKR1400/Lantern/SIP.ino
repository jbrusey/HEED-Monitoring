//GLOBALS
float prev_solat_batt = -1;

/**
 * Constructs a data string from the sensed values
 * @param solar_batt The currently sensed solar lantern voltage
 * @param node_batt The currently sensed node battery voltage
 * @param interrupt The INT_SOURCE register from an ADXL345 sensor.
 * @param seq The packet sequence number
 * @return A string of all the sensor values as a csv
 */
String constructPkt(float solar_batt, float node_batt, byte interrupt, int seq)
{
  debug("Construct packet");

  float unixtime = rtc.getEpoch();                     
  bool inactivity = adxl345IsActivity(interrupt);      
  bool activity = adxl345IsInactivity(interrupt);      
  
  String sep = ","; //CSV seperator
  String dataString = String(unixtime) + sep + solar_batt + sep + 
                      String(interrupt, HEX) + sep + inactivity + sep + 
                      activity + sep + node_batt + sep + seq;
  debug(dataString);
  
  return dataString;
}

/**
 * Checks if the data is "interesting" and needs to be sent. The functions defines data 
 * as being eventful if:
 * 1. The current lantern battery voltage differs from the previous saved voltage by more than a defined
 * threshold
 * 2. The adxl345 has triggered an interruptsee's
 * @param current_voltage The currently sensed solar lantern voltage
 * @param interrupt The INT_SOURCE register from an ADXL345 sensor.
 * @return True if the data is eventful, else false
 */
bool hasEvent(float current_voltage, byte interrupt)
{
  return fabs(current_voltage - prev_solat_batt) >= SIP_SOLAR_BATTERY_THRESH || interrupt != INT_SOURCE_NO_INTERRUPT;
} 


/**
 * Updates the previously stored solar battery voltage
 * @param solar_batt_volt The currently sensed solar lantern voltage
 */ 
void updateState(float solar_batt_volt){
  prev_solat_batt = solar_batt_volt;
}
