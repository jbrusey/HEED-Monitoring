//GLOBALS
int seq = 0;

/**
 * The function takes readings from the abalog sensors. The data is then checked if it is eventful, 
 * and if so is stored is transmitted over GSM and MQTT.
 */
 void doSenseCycle()
{
  
  float temp = getTemperatureThermocouple(); 

  float Si7021Data[2];
  getSi7021Data(Si7021Data);
  
  float batt_volt = getBatteryVoltage();
  
  if (hasEvent(temp)) {
    
    connectGSM();
    connectMQTT();

    String pkt = constructPkt(temp, Si7021Data[0], Si7021Data[1], batt_volt, seq);
    bool transmit_res = transmit(MQTT_TOPIC, pkt);
    
    disconnectMQTT();
    disconnectGSM();
    
    if (transmit_res) { updateState(temp); }
    else {} //Do something for failure
    seq++; //increment sequence number
  }
}

