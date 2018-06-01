/**
 * Starts the USB serial connection
 */
 void startSerial(){
  // Wait for serial USB port to open
  while(!SerialUSB);
  debug("SUM Test");
  }

/**
 * Debug print statement, prints a message and the local time
 */
void _PRINT_TIME(String msg) {
  unsigned long localtime;
  localtime = millis();
  SerialUSB.println(msg + ": " + String(localtime));
}

