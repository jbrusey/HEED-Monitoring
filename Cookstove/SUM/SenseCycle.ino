//GLOBALS
uint32_t seq = 0;
bool first = true;
Data* readings = new Data();

void resetErrors(){
    /* the error code has been transmitted and so can now be reset.     
   *  The method of resetting used here allows for errors to have 
   *  occurred between sending the message and receiving
   acknowledgement. */
   if (last_transmitted_errno < last_errno && last_transmitted_errno != 0)
    last_errno = last_errno / last_transmitted_errno;
   else
    last_errno = 1;
}

void resetReadings(Data* readings){
  readings->unixtime=0;
  readings->tempThermocouple=0;
  readings->tempSi7021=0;
  readings->humidity=0;
  readings->nodeBatt=0;
  readings->error=0;
  readings->seq=0;
}

/**
 * The function takes readings from the analog sensors. The data is then checked if it is eventful, 
 * and if so, it is stored to an SD card and is transmitted over GSM and MQTT.
 */
 void doSenseCycle()
{
  debug("Start Sense " + String(seq));
  
  bool result_transmit = false;
  bool result_store = false;
  bool result_final = false;
    
  getTemperatureThermocouple(readings);
  getSi7021Data(readings);
  
  if (last_errno != 1) readings->error = last_errno;  
  last_transmitted_errno = last_errno;
  
  if (hasEvent(readings) || isHeartbeat()) 
  {
    readings->seq = seq;
    getBatteryVoltage(readings);

    #ifdef TRANSMIT
      if (connectGSM())
      {
        if (connectMQTT())
        {
          getGSMTime(readings);
          String JSON = constructJSON(readings);
          debug("JSON created : " + JSON);
          result_transmit = transmit(MQTT_TOPIC, JSON);
          
          disconnectMQTT();
        }
        disconnectGSM();
      }
    #else
      result_transmit = true;
      getTime(readings);
    #endif  
    
    #ifdef STORE
      result_store = writeDataToFile(readings);
    #else
      result_store = true;
    #endif

    result_final = result_transmit && result_store;

    if (result_final)
    {
      // If this was the first successfull cycle, turn off the LED
      if (first){
        nodeFunctional();
        first = false;
        }
        
      updateState(readings);
      resetErrors();
    }      
    resetReadings(readings);
    seq++; //increment sequence number
  }

  debug("End Sense");
}

