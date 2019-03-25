//ADXL345 LIBRARY
#include <SparkFun_ADXL345.h>
ADXL345 adxl = ADXL345();
#include <Wire.h>


/**
 * Configures the ADXL345 by:
 * 1. Setting the measurement range
 * 2. Configuring interrupts
 * 3. Setting activity/inactivity thresholds
 * 4. Enabling interrupts
 */
void configureADXL345(){
  adxl.setRangeSetting(4);
  configureActivityInterrupt();
  configureInactivityInterrupt();
  setInterrupts();
}

/**
 * Configure activity interrupt
 * 1. Enable link bit (to only detect inactivity after activity and vice-versa)
 * 2. Set axis to monitor
 * 3. Set activity threshold
 */
void configureActivityInterrupt(){
   //Set the link bit, we only want
  //KB: No setLinkBit function found in adxl source code - please verify why it has been implemented
  adxl.setLinkBit(LINK_BIT_ENABLE);
  adxl.setActivityXYZ(ACTIVITY_X_AXIS_ENABLE,
                      ACTIVITY_Y_AXIS_ENABLE,
                      ACTIVITY_Z_AXIS_ENABLE);
  adxl.setActivityThreshold(ACTIVITY_THRESH);
}

/**
 * Configure inactivity interrupt
 * 1. Set axis to monitor
 * 2. Set inactivity threshold
 * 3. Set inactivity time threshold
 */
void configureInactivityInterrupt(){
  adxl.setInactivityXYZ(INACTIVITY_X_AXIS_ENABLE,
                        INACTIVITY_Y_AXIS_ENABLE,
                        INACTIVITY_Z_AXIS_ENABLE);
  adxl.setInactivityThreshold(INACTIVITY_THRESH);
  adxl.setTimeInactivity(INACTIVITY_TIME);      // 30 seconds of no movement is inactivity
}


/**
 * Enable/Disable Interrupts
 */
void setInterrupts(){
  // Turn on/off Interrupts for each mode (1 == ON, 0 == OFF)

  //KB: Interrupts need to be mapped to INT1 pin on ADXL
  adxl.setImportantInterruptMapping(1,1,1,1,1);
  adxl.InactivityINT(INACTIVITY_INT_ENABLE);
  adxl.ActivityINT(ACTIVITY_INT_ENABLE);
  adxl.FreeFallINT(FREEFALL_INT_ENABLE);
  adxl.doubleTapINT(DOUBLETAP_INT_ENABLE);
  adxl.singleTapINT(TAP_INT_ENABLE);
}

/**
 * Setups the ADXL345 sensor by powering on and setting the registers required.
 */
void setupADXL345() {
  adxl.powerOn();
  dbg("ADXL345 on");
  configureADXL345();

  dbg("ADXL345 configured");

}

/**
 * Get any interrupt on the ADXL345 buffer, and checks for activity or inactivity triggers
 * * @param pointer to a data struct
 */
int movement = 0;
void adxl345GetInterrupt(Data* reading){
  byte interrupts = adxl.getInterruptSource(); //Clear interrupts by reading INT_SOURCE register

  reading->interrupt = interrupts;
  // TODO Activity and Inactivity interrupts should be coded as if and else if.
  reading->activity = adxl.triggered(interrupts, ADXL345_ACTIVITY);
  if (reading->activity==1) reading->movement = 1;
  reading->inactivity = adxl.triggered(interrupts, ADXL345_INACTIVITY);
  if (reading->inactivity==1) reading->movement = 0;

  dbg("Interrupt: " + String(reading->interrupt));
  dbg("Activity: " + String(reading->activity));
  dbg("Inactivity: " + String (reading->inactivity));
  dbg("Movement: " + String (reading->movement));
}
