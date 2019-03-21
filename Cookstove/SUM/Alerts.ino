
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

void batteryError(){
  // TODO implement instead an extremely long sleep to avoid over-depleting the battery
  dbg("battery dead");
  uint8_t x = 0;
  while (x < 3){
    digitalWrite(LED_BUILTIN, HIGH);
    delay(250);
    digitalWrite(LED_BUILTIN, LOW);
    delay(250);
    x++;
  }
}

/** reportError records a code to be sent on the next transmission.
* @param errno error code
*/
void reportError(uint8_t errno) {
  uint32_t errno_cubed;
  dbg("Error message: " + String(errno));
  errno_cubed = errno;
  errno_cubed = errno_cubed * errno_cubed * errno_cubed;
  if (last_errno % errno_cubed != 0) last_errno *= errno;
}
