//GLOBALS
int seq = 0;

/**
 * The function takes readings from the abalog sensors. The data is then checked if it is eventful, 
 * and if so is stored is transmitted over GSM and MQTT.
 */
 void doSenseCycle()
{
  float temp = getTemperature(); 
  float batt_volt = getBatteryVoltage();
  
  if (hasEvent(temp)) {
    String pkt = constructMQTTPacket(temp, batt_volt, seq);

    connectGSM();
    connectMQTT();
    
    bool transmit_res = transmit(MQTT_TOPIC, pkt);
    
    disconnectMQTT();
    disconnectGSM();
    if (transmit_res) { updateState(temp); }
    else {} //Do something for failure
    seq++; //increment sequence number
  }
}

