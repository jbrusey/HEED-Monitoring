//GLOBALS
int seq = 0;
Data* readings = new Data();
bool first = true;

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
  bool transmit_res = false;
  bool csvWriteRes = false;
  bool storeRes = false;
    
  getTemperatureThermocouple(readings);
  getSi7021Data(readings);
  
  if (last_errno != 1) readings->error = last_errno;  
  last_transmitted_errno = last_errno;
  
  if (hasEvent(readings) || isHeartbeat()) 
  {

    
    readings->seq = seq;
    getBatteryVoltage(readings);

    if (connectGSM())
    {
      if (connectMQTT())
      {
        getGSMTime(readings);
        String pkt = constructPkt(readings);
        transmit_res = transmit(MQTT_TOPIC, pkt);
        
        disconnectMQTT();
      }
      disconnectGSM();
    }

    #ifdef STORE
    csvWriteRes = writeDataToFile(readings);
    #endif

    
    #ifdef STORE
    storeRes = transmit_res && csvWriteRes;
    #else
    storeRes = transmit_res;
    #endif

    if (storeRes){
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
}

