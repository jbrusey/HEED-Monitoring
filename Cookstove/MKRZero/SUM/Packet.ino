/**
 * Constructs a data string from the sensed values
 * @param temp The currently sensed solar lantern voltage
 * @param node_batt The currently sensed node battery voltage
 * @param seq The packet sequence number
 * @return A string of all the sensor values as a csv
 */
 String constructPkt(Data* readings) {
  String sep = ",";  //CSV seperator
  String dataString = String(readings->unixtime) + sep +
                      String(NODE_ID) + sep + 
                      String(readings->tempThermocouple) + sep +
                      String(readings->tempSi7021) + sep +
                      String(readings->humidity) + sep +
                      String(readings->nodeBatt) + sep +
                      String(readings->seq);
  debug(dataString);
  
  return dataString;
}

