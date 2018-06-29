/**
 * Constructs a data string from the sensed values
 * @param pointer to a data struct
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
                      String(readings->error) + sep +
                      String(readings->seq);
  debug("Data string created: " + dataString);
  
  return dataString;
}

