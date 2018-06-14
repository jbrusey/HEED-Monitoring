/**
 * Constructs a data string from the sensed values
 * @param temp The currently sensed solar lantern voltage
 * @param node_batt The currently sensed node battery voltage
 * @param seq The packet sequence number
 * @return A string of all the sensor values as a csv
 */
 String constructPkt(float temp, float tempSi7021, float humidity, float node_batt, int seq) {
  String sep = ",";  //CSV seperator
  String dataString = String(NODE_ID) + sep + gsmAccess.getTime() + sep + temp + sep + tempSi7021 + sep + humidity + sep + node_batt + sep + seq;
  debug(dataString);
  return dataString;
}

