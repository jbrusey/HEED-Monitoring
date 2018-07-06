/*
 * Code to detect the acceleration profile of the lantern.
 * The data is also recorded onto an SD card for further analysis
 * 
 * Author: Akshay Pillai
 * Date: 06/07/2018
 */
#include <SparkFun_ADXL345.h>         // SparkFun ADXL345 Library

/*********** COMMUNICATION SELECTION ***********/
ADXL345 adxl = ADXL345();             // USE FOR I2C COMMUNICATION


/******************** SETUP ********************/
/*          Configure ADXL345 Settings         */
void setup(){
  
  Serial.begin(9600);                 // Start the serial terminal
  Serial.println("SparkFun ADXL345 Accelerometer Hook Up Guide Example");
  Serial.println();
  
  adxl.powerOn();                     // Power on the ADXL345

  adxl.setRangeSetting(8);           // Give the range settings
                                      // Accepted values are 2g, 4g, 8g or 16g
                                      // Higher Values = Wider Measurement Range
                                      // Lower Values = Greater Sensitivity

  adxl.setSpiBit(0);                  // Configure the device to be in 4 wire SPI mode when set to '0' or 3 wire SPI mode when set to 1
                                      // Default: Set to 1
                                      // SPI pins on the ATMega328: 11, 12 and 13 as reference in SPI Library 
  
  // Turn on Interrupts for each mode (1 == ON, 0 == OFF)
  adxl.InactivityINT(0);
  adxl.ActivityINT(0);
  adxl.FreeFallINT(0);
  adxl.doubleTapINT(0);
  adxl.singleTapINT(0);
  


}

/****************** MAIN CODE ******************/
/*     Accelerometer Readings and Interrupt    */
void loop(){
  
  // Accelerometer Readings
  int x,y,z;   
  adxl.readAccel(&x, &y, &z);         // Read the accelerometer values and store them in variables declared above x,y,z

  // Output Results to Serial 
  Serial.print(x);
  Serial.print(", ");
  Serial.print(y);
  Serial.print(", ");
  Serial.println(z); 
  delay(500);

}


