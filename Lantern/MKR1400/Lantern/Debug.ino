/**
 * Starts the USB serial connection
 */
 void startSerial(){
  while(!SerialUSB);
  debug("Lantern Test");
  }

/**
 * Debug print statement, prints a message and the local time
 */
void _PRINT_TIME(String msg) {
  unsigned long localtime;
  localtime = millis();
  SerialUSB.println(msg + ": " + String(localtime));
}

void _newline() {
  SerialUSB.println();
}
