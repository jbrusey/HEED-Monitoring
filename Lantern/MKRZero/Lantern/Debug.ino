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
  char formattedtime[9];
  localtime = millis();
  sprintf(formattedtime, "%09d", localtime);
  SerialUSB.println(String(formattedtime) + ": " + msg);
}

