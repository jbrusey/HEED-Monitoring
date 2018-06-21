//GLOBALS
int seq = 0;
Data* readings = new Data();

void resetReadings(Data* readings){
  readings->unixtime=0;
  readings->tempThermocouple=0;
  readings->tempSi7021=0;
  readings->humidity=0;
  readings->nodeBatt=0;
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
  
  if (hasEvent(readings)) {
    
    connectGSM();
    connectMQTT();

    getGSMTime(readings);
    readings->seq = seq;

    String pkt = constructPkt(readings);
    bool transmit_res = transmit(MQTT_TOPIC, pkt);
    bool csvWriteRes = writeDataToFile(readings);
    
    disconnectMQTT();
    disconnectGSM();
    
    if (transmit_res || csvWriteRes) { 
      updateState(readings); 
    }
    else { } //Do something for failure
    seq++; //increment sequence number
  }
}

