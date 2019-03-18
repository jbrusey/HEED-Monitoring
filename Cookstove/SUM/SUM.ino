//Production code on Github with pullup resistors

#include <ArduinoJson.h>
#include "SUM.h" // Header file with consts/config
#include "struct.h" // Header file with data struct fetched in sense cycle
#include "error.h"

/**
 * Setups the program by
 * 1. Starting USB serial if in debug mode
 * 2. Setting up the RTC
 * 3. Setting up a connection to the SD Card
 * 4. Setting up all sensors
 * 5, Setting up the RTC to trigger every minute
 */
void setup() {
  pinMode(0, INPUT_PULLUP);
  pinMode(1, INPUT_PULLUP);
  pinMode(2, INPUT_PULLUP);
  pinMode(3, INPUT_PULLUP);     //Pin 4 is used by the SD card
  pinMode(5, INPUT_PULLUP);     //Pin 6 is the built-in LED
  pinMode(7, INPUT_PULLUP);
  pinMode(8, INPUT_PULLUP);
  pinMode(9, INPUT_PULLUP);
  pinMode(10, INPUT_PULLUP);
  pinMode(11, INPUT_PULLUP);
  pinMode(12, INPUT_PULLUP);
  pinMode(13, INPUT_PULLUP);
  pinMode(14, INPUT_PULLUP);
  pinMode(A0, INPUT_PULLUP);      //A1 and A2 are used for the two sensors
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

  setupNodeSleep();

  setupSD();

  setupAnalogSensors();
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
    nodeSleep(); //go back to sleep
  #endif
}
