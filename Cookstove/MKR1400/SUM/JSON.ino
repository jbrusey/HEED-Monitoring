/**
 * Constructs a data string from the sensed values
 * @param pointer to a data struct
 * @return A string of all the sensor values as a csv
 */

//Create JSON buffer and object
const size_t bufferSize = JSON_OBJECT_SIZE(NUM_JSON_FIELDS);
DynamicJsonBuffer jsonBuffer(bufferSize);
JsonObject& payload = jsonBuffer.createObject();
  
String constructJSON(Data* reading)
{
  debug("JSON constructing...");
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

