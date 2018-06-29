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
  readings->solarBatt=0;
  readings->usage=0;
  readings->charging=0;
  readings->interrupt=0;
  readings->inactivity=0;
  readings->activity=0;
  readings->nodeBatt=0;
  readings->seq=0;
  readings->error=0;
}

/**
 * The function takes readings from the battery sensors, and checks the ADXL345 for
 * an interrupt. The data is then checked if it is eventful, and if so is stored 
 * to an SD card.
 */
void doSenseCycle()
{
  getSolarBatteryVoltage(readings);
  getBatteryVoltage(readings);
  adxl345GetInterrupt(readings);
  getLanternState(readings);
  
  if (last_errno != 1) readings->error = last_errno;  
  last_transmitted_errno = last_errno;
  
  if(hasEvent(readings))
  {    
    readings->seq = seq; 
    String pkt = constructPkt(readings);
    
    connectGSM();
    connectMQTT();
    
    getGSMTime(readings);
    bool transmit_res = transmit(MQTT_TOPIC, pkt);
    
    disconnectMQTT();
    disconnectGSM();

    bool csvWriteRes = writeDataToFile(readings);
    
    if (transmit_res && csvWriteRes){
      if (first){
        nodeFunctional();
        first = false;
        }
    }
      
    if (transmit_res || csvWriteRes) { 
      updateState(readings);
      resetErrors(); 
    }
    resetReadings(readings);
    seq++; //increment sequence number*/
  }
}
