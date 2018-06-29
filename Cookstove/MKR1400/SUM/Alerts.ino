
/**
 * Turn the LED off when a
 * node sends a transmission succesffully
 */
void nodeFunctional(){
  bool ledState=LOW;
  uint8_t x = 0;
  while (x < 6){
    if (ledState == LOW) ledState = HIGH;
    else ledState = LOW;
    digitalWrite(LED_BUILTIN, ledState);
    delay(1000);
    x++;
  }
}


bool batteryState = LOW;
void batteryError(){
  debug("battery dead");
  if (batteryState == LOW) batteryState = HIGH;
  else batteryState = LOW;
  digitalWrite(LED_BUILTIN, batteryState);
}


/** reportError records a code to be sent on the next transmission.
* @param errno error code
*/
void reportError(uint8_t errno) {
  uint32_t errno_cubed;
  debug(("Error message: %u\n", errno));
  errno_cubed = errno;
  errno_cubed = errno_cubed * errno_cubed * errno_cubed;
  if (last_errno % errno_cubed != 0) last_errno *= errno;
}
