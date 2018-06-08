int seq = 0;

/**
 * The function takes readings from the battery sensors, and checks the ADXL345 for
 * an interrupt. The data is then checked if it is eventful, and if so is stored 
 * to an SD card.
 */
void doSenseCycle()
{
  float solar_batt_volt = getSolarBatteryVoltage();
  float node_batt_volt = getBatteryVoltage();
  byte interrupt = adxl345CheckForInterrupt();
  
  if(hasEvent(solar_batt_volt, interrupt))
  {
    String pkt = constructPkt(solar_batt_volt, node_batt_volt, interrupt, seq);
    
    connectGSM();
    connectMQTT();
    
    bool transmit_res = transmit(MQTT_TOPIC, pkt);
    bool csvWriteRes = writeDataToFile(pkt);
    
    disconnectMQTT();
    disconnectGSM();
    if (transmit_res || csvWriteRes) { 
      updateState(solar_batt_volt); 
    }
    else {} //Do something for failure
    seq++; //increment sequence number
  }
}
