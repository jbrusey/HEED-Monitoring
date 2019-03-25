
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
  //LED turned off if GSM or MQTT fail in the first cycle - So ensure LED is off after nodeFuncitonal is called!
  digitalWrite(LED_BUILTIN, LOW);
}

void batteryError(){
  dbg("battery dead");
  uint8_t x = 0;
  while (x < 3){
    digitalWrite(LED_BUILTIN, HIGH);
    delay(250);
    digitalWrite(LED_BUILTIN, LOW);
    delay(250);
    x++;
  }
  nodeSleepLowBattery();      //Device sleeps for 10 minutes - wakes up and signals battery low again through LED - check if required - and goes back to sleep
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
