/*
 * Code to detect the acceleration profile of the lantern.
 * The data is also recorded onto an SD card for further analysis
 * 
 * Author: Akshay Pillai
 * Date: 06/07/2018
 */
#include <SparkFun_ADXL345.h>         // SparkFun ADXL345 Library

//libraries and global variables for SD card writing
#include <SPI.h>
#include "SdFat.h"
#define SD_CS_PIN SS1

char fileName[25] = "Acceleration_Profile.csv";
SdFat SD;
SdFile file;


/*********** COMMUNICATION SELECTION ***********/
ADXL345 adxl = ADXL345();             // USE FOR I2C COMMUNICATION

/**
 * Configure the SD card for use. This function checks to
 * see if the card is present and can be initialized:
 */
void setupSD() {
  Serial.print("Setting up SD:");

  if (!SD.begin(SD_CS_PIN, SPI_HALF_SPEED)) {
    Serial.println("SD card failed, or not present");
    return; //Need to turn LED on or similar
  }

  Serial.println(fileName);

  Serial.println("SD card initialized");
}

/******************** SETUP ********************/
/*          Configure ADXL345 Settings         */
void setup(){
  
  Serial.begin(9600);                 // Start the serial terminal
  
  delay(5000);
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
  
  setupSD();

}

/****************** MAIN CODE ******************/
/*     Accelerometer Readings and Interrupt    */
void loop(){
  
  // Accelerometer Readings
  int x,y,z;   
  adxl.readAccel(&x, &y, &z);         // Read the accelerometer values and store them in variables declared above x,y,z

  file.open(fileName, O_APPEND | O_CREAT | O_WRITE );
  
  //outputting results to file in SD card.
  delay(10);
  file.print(x);
  file.print(", ");
  file.print(y);
  file.print(", ");
  file.println(z);
  delay(10);


  file.close();
  
  // Output Results to Serial 
  Serial.print(x);
  Serial.print(", ");  
  Serial.print(y);
  Serial.print(", ");  
  Serial.println(z); 
  
  delay(80);

}


