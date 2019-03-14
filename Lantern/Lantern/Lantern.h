#include "Credentials.h"                      // Header file with MQTT server and SIM credentials. Look below for sample.

//GENERAL
#define NODE_ID "1"                           // Set ID of the arduino (make sure this is unique)

#define SD_CS_PIN 4                           // "4" for MKR1400, "SS1" for MKRZero

// TODO: Remove transmit
#define TRANSMIT 1                            // Comment out to not use SIM card

#define SERIAL_SPEED 9600                     // Serial speed
#define STARTUP_DELAY 5000                    // How long to delay before the node starts

//SAMPLE PERIOD
#define RTC_SAMPLE_TIME 25                    // SET RTC alarm off every xth second in a minute
//KB: Time specified in lowPower.sleep()
#define LANTERN_SLEEP_TIME 60000              //Set lantern to sleep for 1 min = 60000ms

//SIP THRESHOLDS
#define HEART_LIMIT 1440

#define BATTERY_LOW_VOLTAGE 3.35
#define SIP_SOLAR_BATTERY_THRESH 0.2         // Threshold to define when the solar lantern battery is eventful (TO-DO)
#define SIP_STATE_THRESH 1000                 // Threshold to define when the solar state has changed (TO-DO)
#define INT_SOURCE_NO_INTERRUPT 0x83        // DATA_READY, Watermark, and Overun are always active so interrupt source will always read 0x83 if no other interrupt has been triggered

//GSM
#define GSM_CONNECT_RETRY_TIME 500            // How long to retry between gsm connection tries (0.5s)


//MQTT
#define MQTT_KEEP_ALIVE 36000                 // MQTT keep alive time (10hours)
#define MQTT_CLEAN_SESSION true               // MQTT start a clean session on connection
#define MQTT_TIMEOUT 1000                     // MQTT 1s timeout
#define MQTT_CONNECT_RETRY_TIME 500           // How long to retry between MQTTT broker connection tries (0.5s)
#define MQTT_JSON_BUFFER 300                  // Calculated at https://arduinojson.org/v5/assistant/
#define NUM_JSON_FIELDS 12


//Sensors
#define DIGITAL_ON_WAIT 10                    // Wait 10ms to ensure digital pin has switched high

#define ADC_MIN 0                             // Min value extect from the ADC
#define ADC_MAX 1023                          // Max value extect from the ADC

#define INT_BATTERY_DIVIDER_MAX 4.2           // Max voltage that can be sensed from the internal voltage divider with 3.3v reference

#define ADC_VREF 6.6                          // Max voltage that can be sensed from the voltage divider
#define ADC_BITS 1023.0                       // number of bits


//ADXL345 settings
// TODO set threshold values based on data collected and learning - Decision trees or LDA
#define INACTIVITY_TIME 30
#define INACTIVITY_THRESH 30                  // 62.5mg per increment   // Set activity   // Activity thresholds (0-255) (CHECK!!!!)
#define ACTIVITY_THRESH 30                    // 62.5mg per increment   // Set activity   // Activity thresholds (0-255) (CHECK!!!!)

#define INACTIVITY_INT_ENABLE 1
#define ACTIVITY_INT_ENABLE 1
#define FREEFALL_INT_ENABLE 0
#define DOUBLETAP_INT_ENABLE 0
#define TAP_INT_ENABLE 0

#define INACTIVITY_X_AXIS_ENABLE 1
#define INACTIVITY_Y_AXIS_ENABLE 1
#define INACTIVITY_Z_AXIS_ENABLE 1

#define ACTIVITY_X_AXIS_ENABLE 1
#define ACTIVITY_Y_AXIS_ENABLE 1
#define ACTIVITY_Z_AXIS_ENABLE 1

#define LINK_BIT_ENABLE true


//Macro for print time
#ifdef DEBUG
  #define debug(msg) _PRINT_TIME(String(msg))
#else
  #define debug(msg) _DO_NOTHING();
#endif

// Below is how Credentials.h should look like:

//CREDENTIALS - SIM
//#define SECRET_PINNUMBER     ""               // SIM Pin number
//#define SECRET_GPRS_APN      ""               // replace GPRS APN
//#define SECRET_GPRS_LOGIN    ""               // replace with GPRS login
//#define SECRET_GPRS_PASSWORD ""               // replace with GPRS password

//CREDENTIALS - MQTT
//#define MQTT_SERVER ""                        // MQTT broker address
//#define MQTT_USER ""
//#define MQTT_PASS ""
