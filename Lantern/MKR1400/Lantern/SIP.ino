//GLOBALS
float prev_solat_batt = -1;


/**
 * Checks if the data is "interesting" and needs to be sent. The functions defines data 
 * as being eventful if:
 * 1. The current lantern battery voltage differs from the previous saved voltage by more than a defined
 * threshold
 * 2. The adxl345 has triggered an interruptsee's
 * @param current_voltage The currently sensed solar lantern voltage
 * @param interrupt The INT_SOURCE register from an ADXL345 sensor.
 * @return True if the data is eventful, else false
 */
bool hasEvent(float current_voltage, byte interrupt)
{
  return fabs(current_voltage - prev_solat_batt) >= SIP_SOLAR_BATTERY_THRESH || interrupt != INT_SOURCE_NO_INTERRUPT;
} 


/**
 * Updates the previously stored solar battery voltage
 * @param solar_batt_volt The currently sensed solar lantern voltage
 */ 
void updateState(float solar_batt_volt){
  prev_solat_batt = solar_batt_volt;
}
