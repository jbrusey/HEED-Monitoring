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
 * @return Node battery voltage (0-3.3V)
 */
float getBatteryVoltage()
{
  return (analogRead(ADC_BATTERY) / ADC_BITS ) * INT_BATTERY_DIVIDER_MAX;;
}

/**
 * Gets the battery voltage from the Lantern monitoris
 * voltage divider
 * @return Solar lantern battery voltage (0-3.3V)
 * 
 */
float getSolarBatteryVoltage()
{
  return (analogRead(A1) / ADC_BITS ) * ADC_VREF;;
}
