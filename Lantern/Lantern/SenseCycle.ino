//GLOBALS
uint32_t seq = 0;
bool first = true;
Data* readings = new Data();

void resetErrors(){
  /* the error code has been transmitted and so can now be reset.  The
   * method of resetting used here allows for errors to have occurred
   * between sending the message and receiving acknowledgement. */
  if (last_transmitted_errno < last_errno && last_transmitted_errno != 0)
    last_errno = last_errno / last_transmitted_errno;
  else
    last_errno = 1;
}

void resetReadings(Data* readings){
  readings->unixtime=0;
  readings->solarBatt=0;
  readings->interrupt=0;
  readings->inactivity=0;
  readings->activity=0;
  readings->nodeBatt=0;
  readings->error=0;
  readings->seq=0;
}

/**
 * The function takes readings from the battery sensors, and checks the ADXL345 for
 * an interrupt. The data is then checked if it is eventful, and if so is stored
 * to an SD card.
 */
void doSenseCycle()
{
  dbg("Start Sense " + String(seq));

  bool result_transmit = false;
  bool result_store = false;
  bool result_final = false;

  getSolarBatteryVoltage(readings);
  adxl345GetInterrupt(readings);

  if (last_errno != 1) readings->error = last_errno;
  last_transmitted_errno = last_errno;

  if (hasEvent(readings) || isHeartbeat())
    {
      readings->seq = seq;
      getBatteryVoltage(readings);

      if (connectGSM()) {
	if (connectMQTT()) {
	  getGSMTime(readings);
	  String JSON = constructJSON(readings);
	  dbg("JSON created : " + JSON);
	  result_transmit = transmit(MQTT_TOPIC, JSON);

	  disconnectMQTT();
	}
	disconnectGSM();
      }

      result_store = writeDataToFile(readings);

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

  dbg("End Sense");
}
