uint32_t seq = 0;
#ifdef LEDS
int state = false;
#endif
/**
 * The function takes readings from the battery sensors, and checks the ADXL345 for
 * an interrupt. The data is then checked if it is eventful, and if so is stored 
 * to an SD card.
 */
void doSenseCycle()
{
  Data* readings = new Data();
   
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
    seq++;
  }
}
