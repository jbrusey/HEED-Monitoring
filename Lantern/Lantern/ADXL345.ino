/** @file ADXL345.ino
 *  @brief Initialise ADXL345 sensor.  
 *
 *  This contains the functions to configure and set up  
 *  the ADXL interrupt routine and estimate step count.
 *  
 *  @author Kriti Bhargava
 *  @author James Brusey
 *  @bug No known bugs.
 */
 
#include "ArduinoLowPower.h"

//ADXL345 LIBRARY
#include <SparkFun_ADXL345.h>
ADXL345 adxl = ADXL345();
#include <Wire.h>

volatile int state = 1;
volatile int adxl_step = 0;
/**
 * Configures the ADXL345 by:
 * 1. Setting the measurement range
 * 2. Configuring interrupts
 * 3. Setting activity/inactivity thresholds
 * 4. Enabling interrupts
 */
void configureADXL345(){
  //Initially disabled
  adxl.InactivityINT(0);
  adxl.ActivityINT(0);
  adxl.FreeFallINT(0);
  adxl.doubleTapINT(0);
  adxl.singleTapINT(0);

  adxl.setRangeSetting(4);           // Give the range settings
                                      // Accepted values are 2g, 4g, 8g or 16g
                                      // Higher Values = Wider Measurement Range
                                      // Lower Values = Greater Sensitivity
  configureActivityInterrupt();
  configureFreefallInterrupt();
  setInterrupts();
}

/**
 * Configure activity interrupt
 * 1. Enable link bit (to only detect inactivity after activity and vice-versa)
 * 2. Set axis to monitor
 * 3. Set activity threshold
 */
void configureActivityInterrupt(){
  adxl.setActivityXYZ(ACTIVITY_X_AXIS_ENABLE,
                      ACTIVITY_Y_AXIS_ENABLE,
                      ACTIVITY_Z_AXIS_ENABLE);
  adxl.setActivityThreshold(ACTIVITY_THRESH);
}

/**
 * Configure inactivity interrupt
 * 1. Set axis to monitor
 * 2. Set freefall threshold
 * 3. Set freefall time threshold
 */
void configureFreefallInterrupt(){
  adxl.setFreeFallThreshold(FREEFALL_THRESH);
  adxl.setFreeFallDuration(FREEFALL_TIME);
}


/**
 * Enable/Disable Interrupts
 */
void setInterrupts(){
  adxl.setImportantInterruptMapping(1,1,1,1,1);

  pinMode(INTERRUPT_PIN, INPUT_PULLDOWN);
  LowPower.attachInterruptWakeup(INTERRUPT_PIN, ADXL_ISR, RISING); //Attach interrupt for sleep
  delay(1000);

  // Turn on/off Interrupts for each mode (1 == ON, 0 == OFF)
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
 * Get any interrupt on the ADXL345 buffer, and checks for activity or freefall triggers
 * * @param pointer to a data struct
 */

void freefall_detected() {
  if (state == 2) {
    state = 1;
    adxl_step++;
  }
}

void activity_detected() {
  if (state == 1) {
    state = 2;
  }
}

void ADXL_ISR() {
  // getInterruptSource clears all triggered actions after returning value
  // Do not call again until you need to recheck for triggered actions
  byte interrupts = adxl.getInterruptSource();

  // Free Fall Detection
  if(adxl.triggered(interrupts, ADXL345_FREE_FALL)){
    freefall_detected();
  }

  // Activity
  if(adxl.triggered(interrupts, ADXL345_ACTIVITY)){
    activity_detected();
  }
}
