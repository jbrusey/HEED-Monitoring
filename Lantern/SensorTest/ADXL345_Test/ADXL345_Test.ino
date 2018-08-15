#include <SparkFun_ADXL345.h>                                               // SparkFun ADXL345 Library
ADXL345 adxl = ADXL345();  
#include <Wire.h>

int interruptPin = 7;                                                       // Setup pin 2 to be the interrupt pin (for most Arduino Boards)
volatile bool isInt = false;      

void setup() {
  while(!SerialUSB);
  delay(5000); //avoids the node going to sleep straight away (avoids the node hanging during flashing)
  // Start the serial terminal
  SerialUSB.println("ADXL345 test");

  Wire.begin();                                                             // Start the I2C interface

  /******************** SETUP ADXL345 ********************/
  adxl.powerOn();                                                           // Power on the ADXL345
  SerialUSB.println("ADXL345 on");
  adxl.setRangeSetting(4);  
  
  adxl.setActivityXYZ(1, 1, 1);                                             // Set to activate movement detection in the axes "adxl.setActivityXYZ(X, Y, Z);" (1 == ON, 0 == OFF)
  adxl.setActivityThreshold(75);                                            // 62.5mg per increment   // Set activity   // Activity thresholds (0-255) (CHECK!!!!)
 
  adxl.setInactivityXYZ(1, 1, 1);                                           // Set to detect inactivity in all the axes "adxl.setInactivityXYZ(X, Y, Z);" (1 == ON, 0 == OFF)
  adxl.setInactivityThreshold(75);                                          // 62.5mg per increment   // Set inactivity // Inactivity thresholds (0-255) (CHECK!!!!)
  adxl.setTimeInactivity(5);                                               // 30 seconds of no movement is inactivity
  
  adxl.setLinkBit(true);                                                    //Set the link bit, we only want to detect inactivity after activity and vice-versa

  adxl.setImportantInterruptMapping(1, 1, 1, 1, 1);                         // Setting all interupts to take place on INT1 pin     
  
  // Turn on Interrupts for each mode (1 == ON, 0 == OFF)
  adxl.InactivityINT(1);                                                    //Turn on inactivity monitoring
  adxl.ActivityINT(1);                                                      //Turn on activity monitoring
  adxl.FreeFallINT(0);                                                      //Turn off all other monitoring
  adxl.doubleTapINT(0);
  adxl.singleTapINT(0);
  
  byte interrupts = adxl.getInterruptSource();                              //Clear interrupts by reading INT_SOURCE register
  attachInterrupt(digitalPinToInterrupt(interruptPin), ADXL_ISR, RISING);   //Attach Interrupt

}

void PRINT_INT(){
  byte interrupts = adxl.getInterruptSource();                               // Clear interrupts by reading INT_SOURCE register
  bool inactivity = adxl.triggered(interrupts, ADXL345_INACTIVITY);          // Was there an interrupt for inactivity?
  bool activity = adxl.triggered(interrupts, ADXL345_ACTIVITY);              // Was there an interrupt for inactivity?
  
  String activityString = String(inactivity);
  activityString += ",";
  activityString += String(activity);
  SerialUSB.println(activityString);
}

void ADXL_ISR() {
  isInt = true;
}

void loop() {
  // Accelerometer Readings
  int x,y,z;   
  adxl.readAccel(&x, &y, &z);         // Read the accelerometer values and store them in variables declared above x,y,z
  
  if (isInt) {
    PRINT_INT();
    isInt = false;
    }
  // Output Results to Serial
  // UNCOMMENT TO VIEW X Y Z ACCELEROMETER VALUES  
  SerialUSB.print(x);
  SerialUSB.print(", ");
  SerialUSB.print(y);
  SerialUSB.print(", ");
  SerialUSB.println(z);

}
