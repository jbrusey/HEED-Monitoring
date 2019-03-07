/**
 * Starts the USB serial connection
 */
 void startSerial(){
   // Wait for serial USB port to open
   
   for (int i = 0; i < 1000 && !SerialUSB; i++) {
     delay(1);
   }
   
   //while(!SerialUSB);
   dbg("SUM Test");
 }

/**
 * Debug print statement, prints a message and the local time
 */
void _PRINT_TIME(String msg) {
  unsigned long localtime;
  char formattedtime[9];
  localtime = millis();
  sprintf(formattedtime, "%09lu", localtime);
  SerialUSB.println(String(formattedtime) + ": " + msg);
}



