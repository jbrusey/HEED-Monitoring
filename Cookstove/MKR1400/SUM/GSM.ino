//Include GSM library
#include <MKRGSM.h>

// initialize the library instance
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
 void connectGSM() {
  bool GSMConnected = false;
  debug("connecting to cellular network...");

  // After starting the modem with GSM.begin()
  // attach the shield to the GPRS network with the APN, login and password
  while (!GSMConnected) {
    if ((gsmAccess.begin(PINNUMBER) == GSM_READY) &&
        (gprs.attachGPRS(GPRS_APN, GPRS_LOGIN, GPRS_PASSWORD) == GPRS_READY)) 
    {
      gsmAccess.lowPowerMode();
      debug("GSM ready");
      GSMConnected = true;
    } 
    else {
      delay(GSM_CONNECT_RETRY_TIME);
    }
  }
}

void disconnectGSM() {
  gprs.detachGPRS();
  gsmAccess.shutdown();
  debug("GSM disconnected");
}
