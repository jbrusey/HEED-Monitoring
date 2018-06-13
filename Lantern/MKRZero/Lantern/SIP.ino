//GLOBALS
float prev_solat_batt = -1;

/**
 * Checks if the data is "interesting" and needs to be sent. The functions defines data 
 * as being eventful if:
 * 1. The current lantern battery voltage differs from the previous saved voltage by more than a defined
 * threshold
 * 2. The adxl345 has triggered an interruptsee's  
 * * @param pointer to data structure
 * @return True if the data is eventful, else false
 */
bool hasEvent(Data* readings)
{
  return fabs(readings->solarBatt - prev_solat_batt) >= SIP_SOLAR_BATTERY_THRESH || readings->interrupt != INT_SOURCE_NO_INTERRUPT;
} 


/**
 * Updates the previously stored solar battery voltage
 * @param pointer to data structure
 */ 
void updateState(Data* readings){
  prev_solat_batt = readings->solarBatt;
}
