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
  getTemperatureThermocouple(readings);
  getSi7021Data(readings);
  getBatteryVoltage(readings);
  
  if (last_errno != 1) readings->error = last_errno;  
  last_transmitted_errno = last_errno;

  if (hasEvent(readings)) {
    
    getTime(readings);
    readings->seq = seq;

    bool csvWriteRes = writeDataToFile(readings);

    if (csvWriteRes) { 
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

