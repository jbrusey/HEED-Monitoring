//Include 1-wire and thermocouple libraries
#include <OneWire.h>
#include <DallasTemperature.h>
#include <SparkFun_Si7021_Breakout_Library.h>
#include <Wire.h>

OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature sensors(&oneWire);
Weather Si7021;


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

  //Set Si7021 power pin to output and make sure they are off
  pinMode(Si7021_POWER_PIN, OUTPUT);
  digitalWrite(Si7021_POWER_PIN, LOW);

  Si7021.begin();

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
float getTemperatureThermocouple() {
  powerMAX31850(); //turn on sensor
  debug("Read temperature (thermocouple)");

  sensors.requestTemperatures(); // Send the command to get temperatures
  float temp = sensors.getTempCByIndex(MAX31850_ADDR); //get temp
  debug(String("Temp (thermocouple): ") + String(temp) + " C");
  
  unpowerMAX31850(); //turn off sensor
  return temp;
}

/**
 * Power up the Si7021
 */
void powerSi7021() { 
  digitalWrite(Si7021_POWER_PIN, HIGH);
  delay(DIGITAL_ON_WAIT);//Slight delay to allow the switch to happen
  debug("Si7021 On");
}

/**
 * Remove power from the Si7021
 */
void unpowerSi7021() {
  digitalWrite(Si7021_POWER_PIN, LOW);
  debug("Si7021 Off");
}

/**
 * Gets the current temperature for the Si7021 sensor
 * interface
 * @return Current Si7021 sensor temperature
 */
void getSi7021Data(float *result) {
  
  powerSi7021(); //turn on sensor
  debug("Read temperature + humidity (Si7021 sensor)");

  result[0] = Si7021.getTemp(); // Send the command to get temperatures
  result[1] = Si7021.getRH(); // Send the command to get humidity
  debug(String("Temp (Si7021): ") + String(result[0]) + " C");
  debug(String("Humidity (Si7021): ") + String(result[1]) + " %");

  unpowerSi7021(); //turn off sensor
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


