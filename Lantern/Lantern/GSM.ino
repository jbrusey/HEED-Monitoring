//Include GSM library
#include <MKRGSM.h>
  
// initialize the library instances
GSMClient net;
GPRS gprs;
GSM gsmAccess;

// Please enter your sensitive data in arduino_secrets.h
const char PINNUMBER[]     = SECRET_PINNUMBER;
const char GPRS_APN[]      = SECRET_GPRS_APN;
const char GPRS_LOGIN[]    = SECRET_GPRS_LOGIN;
const char GPRS_PASSWORD[] = SECRET_GPRS_PASSWORD;

/**
 * Connects the node to a GSM netwrok
 */
bool connectGSM() {
  dbg("GSM: Connecting to cellular network...");

  //set a timeout of 60 s for the GPRS and GSM operation
  gprs.setTimeout(GSM_CONNECT_RETRY_TIME);
  gsmAccess.setTimeout(GSM_CONNECT_RETRY_TIME);
  
  // After starting the modem with GSM.begin()
  // attach the shield to the GPRS network with the APN, login and password
  if (gsmAccess.begin(PINNUMBER) == GSM_READY) {
    if (gprs.attachGPRS(GPRS_APN, GPRS_LOGIN, GPRS_PASSWORD) == GPRS_READY) {
      gsmAccess.lowPowerMode();
      dbg("GSM: Ready");
      return true;
    }
    else {
      // Incorrect GSM settings
      disconnectGSM();
      digitalWrite(LED_BUILTIN, LOW);
      reportError(ERR_GSM_ATTACH_FAILED);
      return false;
    }
  }
  else {
    // GSM begin failed
    gsmAccess.shutdown();
    digitalWrite(LED_BUILTIN, LOW);
    reportError(ERR_GSM_CONNECTION_FAILED);
    return false;
  }
}

unsigned long getGSMTime() {
  return gsmAccess.getTime();
}

void disconnectGSM() {
  //TODO: Implement some check if GPRS is still online, otherwise the program will freeze here
  gprs.detachGPRS();
  gsmAccess.shutdown();
  dbg("GSM: Disconnected");
}
