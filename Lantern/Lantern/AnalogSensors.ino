/**
 * Setups the Analog sensors
 * 1. Sets the reference voltage
 * 2. Turns on the LED
 */
void setupAnalogSensors() {
  analogReference(AR_DEFAULT); //3.3V

  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, HIGH); //turn LED on until first successfull write

  dbg("Digital pins set");
}



/**
 * Gets the battery voltage from the MKR Zero's internal voltage
 * divider
 * @return battery voltage in range 0 - 1023 (see ADC_MAX) where 1023
 * means 4.2 volts
 */
short int getBatteryVoltage()
{
  return analogRead(ADC_BATTERY);
}

/**
 * Gets the battery voltage from the lantern's solar recharged battery
 * @return battery voltage in range 0 - 1023 (for MKR) where 1023
 * means 2 * 3.3V
 */
short int getSolarBatteryVoltage()
{
  return analogRead(ADC_SOLAR_BATTERY);
}
