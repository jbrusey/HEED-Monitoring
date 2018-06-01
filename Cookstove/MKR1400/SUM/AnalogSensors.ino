//Include 1-wire and thermocouple libraries
#include <OneWire.h>
#include <DallasTemperature.h>

OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature sensors(&oneWire);


/**
 * Setups the Analog sensors
 * 1. Sets the reference voltage
 * 2. Sets up the pin for powering the MAX31850
 * 3. Turns of the LED
 */
void setupAnalogSensors() {
  analogReference(AR_DEFAULT); //3.3V

  //Set MAX3150 power pin to output and make sure it is off
  pinMode(MAX31850_POWER_PIN, OUTPUT);
  digitalWrite(MAX31850_POWER_PIN, LOW); //turn LED off

  //Turn the built in LED off 
  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, LOW);
  
  debug("Digital pins Set");
}

/**
 * Power up the MAX31850
 */
void powerMAX31850() { 
  digitalWrite(MAX31850_POWER_PIN, HIGH);
  delay(DIGITAL_ON_WAIT);//Slight delay to allow the switch to happen
  debug("MAX31850 On");
}

/**
 * Remove power from the MAX31850
 */
void unpowerMAX31850() {
  digitalWrite(MAX31850_POWER_PIN, LOW);
  debug("MAX31850 Off");
}

/**
 * Gets the current temperature for the MAX31850 1-wire 
 * interface
 * @return Current thermnocouple temperature
 */
float getTemperature() {
  powerMAX31850(); //turn on sensor
  debug("Read temperature");

  sensors.requestTemperatures(); // Send the command to get temperatures
  float temp = sensors.getTempCByIndex(MAX31850_ADDR); //get temp
  debug(String("Temp: ") + String(temp) + " C");
  
  unpowerMAX31850(); //turn off sensor
  return temp;
}


/**
 * Gets the battery voltage from the MKR Zero's internal
 * voltage divider
 * @return Node battery voltage (0-3.3V)
 */
float getBatteryVoltage()
{
  return ( analogRead(ADC_BATTERY) / ADC_BITS ) * INT_BATTERY_DIVIDER_MAX;
}


