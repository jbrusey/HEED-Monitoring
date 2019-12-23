/** @file Debug.ino
 *  @brief Start serial to print messages in debug mode.  
 *
 *  This contains the functions to connect the serial
 *  port and print messages in debug mode.
 *  
 *  @author Kriti Bhargava
 *  @author James Brusey
 *  @bug No known bugs.
 */
 
/**
 * Starts the USB serial connection
 */
 void startSerial(){
   // Wait for serial USB port to open
   for (int i = 0; i < 1000 && !SerialUSB; i++) {
     delay(1);
   }

   dbg("Lantern Test");
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
