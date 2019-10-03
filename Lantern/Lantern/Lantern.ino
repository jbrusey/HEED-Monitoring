
#include <ArduinoJson.h>
#include "Lantern.h" // Header file with consts/config
#include "struct.h" // Header file with data struct fetched in sense cycle
#include "error.h"

/**
 * Setups the program by
 * 1. Starting USB serial if in dbg mode
 * 2. Setting up the RTC
 * 3. Setting up a connection to the SD Card
 * 4. Setting up all sensors
 * 5, Setting up the RTC to trigger every minute
 */
void setup() {
  pinMode(0, INPUT_PULLUP);
  pinMode(1, INPUT_PULLUP);
  pinMode(2, INPUT_PULLUP);
  pinMode(3, INPUT_PULLUP);     //Pin 4 is used for mem breakout board, 5 for ADXL interrupt from INT 1 and 6 is the built-in LED
  pinMode(7, INPUT_PULLUP);     //Pins 8,9,10 are connected to Mem board; 
  pinMode(13, INPUT_PULLUP);    //Pins 11 and 12 are connected to SDA and SCL of ADXL respectively and to VCC for bus termination using 4.3Kohm resistors
  pinMode(14, INPUT_PULLUP);
  pinMode(A0, INPUT_PULLUP);     //A1 used for solar battery
  pinMode(A2, INPUT_PULLUP);
  pinMode(A3, INPUT_PULLUP);
  pinMode(A4, INPUT_PULLUP);
  pinMode(A5, INPUT_PULLUP);
  pinMode(A6, INPUT_PULLUP);

  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, LOW);

  pinMode(26, INPUT_PULLUP);
  pinMode(27, INPUT_PULLUP);
  pinMode(28, INPUT_PULLUP);
  pinMode(29, INPUT_PULLUP);

  #ifdef DEBUG
    startSerial();
  #else
    delay(STARTUP_DELAY); //avoids the node going to sleep straight away (avoids the node hanging during flashing)
    USBDevice.detach();
  #endif

  watchdog_setup();

  setupNodeSleep();
  setupSD();

  setupAnalogSensors();
  setupADXL345();

  watchdog_clear();
}

/**
 * MAIN LOOP: Calls for the sensing procedure to run before
 * putting the node to sleep
 */
void loop() {
 if (batteryLow)
    batteryError();
 else{
    #ifdef DEBUG
      SerialUSB.println("\n-----------------");
    #endif
      doSenseCycle();
  }

  #ifdef DEBUG
    delay(5000); // keeps USB connection on instead
  #else
    nodeSleep(LANTERN_SLEEP_TIME); //go back to sleep
  #endif
}
