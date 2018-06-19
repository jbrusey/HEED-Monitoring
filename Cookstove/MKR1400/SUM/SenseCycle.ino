//GLOBALS
int seq = 0;

/**
 * The function takes readings from the analog sensors. The data is then checked if it is eventful, 
 * and if so, it is stored to an SD card and is transmitted over GSM and MQTT.
 */
 void doSenseCycle()
{
  Data* readings = new Data();

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
    bool csvWriteRes = writeDataToFile(pkt);
    
    disconnectMQTT();
    disconnectGSM();
    
    if (transmit_res || csvWriteRes) { 
      updateState(readings); 
    }
    else { } //Do something for failure
    seq++; //increment sequence number
  }
}

