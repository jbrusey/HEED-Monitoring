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
  // TODO remove variable
  bool GSMConnected = false;
  dbg("GSM: Connecting to cellular network...");
  
  // After starting the modem with GSM.begin()
  // attach the shield to the GPRS network with the APN, login and password
  // TODO split into separate if statements so failure logic can be different
  if ((gsmAccess.begin(PINNUMBER) == GSM_READY) &&
      (gprs.attachGPRS(GPRS_APN, GPRS_LOGIN, GPRS_PASSWORD) == GPRS_READY)) 
    {
      gsmAccess.lowPowerMode();
      dbg("GSM: Ready");
      // return true
      GSMConnected = true;
    } 
  else {
    // TODO disconnect / lowpower gsm here and return false
    reportError(ERR_GSM_CONNECTION_FAILED);
  }
  // TODO not needed
  return GSMConnected;
}
  
void getGSMTime(Data* readings) {
  readings->unixtime = gsmAccess.getTime();
}
  
void disconnectGSM() {
  //TODO: Implement some check if GPRS is still online, otherwise the program will freeze here
  gprs.detachGPRS();
  gsmAccess.shutdown();
  dbg("GSM: Disconnected");
}
