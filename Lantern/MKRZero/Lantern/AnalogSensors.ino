/**
 * Setups the Analog sensors
 * 1. Sets the reference voltage
 * 3. Turns of the LED
 */
void setupAnalogSensors() {
  analogReference(AR_DEFAULT); //3.3V
  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, LOW);
  debug("Digital pins set");
}

/**
 * Gets the battery voltage from the MKR Zero's internal
 * voltage divider
 * @param pointer to a data struct
 */
void getBatteryVoltage(Data* readings)
{
  readings->nodeBatt = (analogRead(ADC_BATTERY) / ADC_BITS ) * INT_BATTERY_DIVIDER_MAX;
}

/**
 * Gets the battery voltage from the Lantern monitoris
 * voltage divider
 * * @param pointer to a data struct
 */
void getSolarBatteryVoltage(Data* readings)
{
  readings->solarBatt = (analogRead(A1) / ADC_BITS ) * ADC_VREF;
}


/**
 * Dummy value for the lantern state 
 * * @param pointer to a data struct
 */
float getLanternState(Data* readings)
{
  readings->lanternState <- 15;
}
