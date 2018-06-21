uint32_t seq = 0;
#ifdef LEDS
int state = false;
#endif
/**
 * The function takes readings from the battery sensors, and checks the ADXL345 for
 * an interrupt. The data is then checked if it is eventful, and if so is stored 
 * to an SD card.
 */
Data* readings = new Data();


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
}

void doSenseCycle()
{   
  getTime(readings);
  getSolarBatteryVoltage(readings);
  getBatteryVoltage(readings);
  adxl345GetInterrupt(readings);
  getLanternState(readings);
  
  if(hasEvent(readings))
  {
    readings->seq = seq; 
    
    if (writeDataToFile(readings)) 
    { 
#ifdef LEDS   
      state = !state;
      digitalWrite(LED_BUILTIN, state);
#endif
      updateState(readings);
    }
    else {} //Do something for failure
    resetReadings(readings);
    seq++;
  }
}
