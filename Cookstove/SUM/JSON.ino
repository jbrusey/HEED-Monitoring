/** @file JSON.ino
 *  @brief Create a data string with payload.  
 *
 *  This contains the function to create a data string
 *  and assign sensor values to payload.
 *  
 *  @author Kriti Bhargava
 *  @author James Brusey
 *  @bug No known bugs.
 */

//Create JSON buffer and object
const size_t bufferSize = JSON_OBJECT_SIZE(NUM_JSON_FIELDS);
DynamicJsonBuffer jsonBuffer(bufferSize);
JsonObject& payload = jsonBuffer.createObject();

/**
 * Constructs a data string from the sensed values
 * @param pointer to a data struct
 * @return A string of all the sensor values as a csv
 */
String constructJSON(Data* reading)
{
  dbg("JSON constructing...");
  String dataString = "";

  payload["unixtime"] = reading->unixtime;
  payload["NODE_ID"] = NODE_ID;
  payload["tempThermocouple"] =  reading->tempThermocouple;
  payload["tempSi7021"] =  reading->tempSi7021;
  payload["humidity"] =  reading->humidity;
  payload["nodeBatt"] =  reading->nodeBatt;
  payload["error"] =  reading->error;
  payload["seq"] =  reading->seq;

  payload.printTo(dataString);
  return dataString;
}
