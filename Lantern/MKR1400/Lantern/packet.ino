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
