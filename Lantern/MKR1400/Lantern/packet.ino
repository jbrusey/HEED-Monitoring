/**
 * Constructs a data string from the sensed values
 * @param pointer to data structure
 * @return A string of all the sensor values as a csv
 */
String constructPkt(Data* reading)
{
  debug("Construct packet");

  String sep = ","; //CSV seperator
  String dataString = String(reading->unixtime) + sep +
                      String(NODE_ID) + sep +
                      String(reading->solarBatt) + sep +
                      String(reading->usage) + sep +
                      String(reading->charging) + sep +
                      String(reading->interrupt, HEX) + sep +
                      String(reading->inactivity) + sep +
                      String(reading->activity) + sep +
                      String(reading->nodeBatt) + sep +
                      String(reading->error) + sep +
                      String(reading->seq);
  debug(dataString);
  
  return dataString;
}
