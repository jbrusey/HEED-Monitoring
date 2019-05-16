#pragma once
#include "Credentials.h"                      // Header file with MQTT server and SIM credentials. Look below for sample.

//GENERAL
#define NODE_ID "1"                           // Set ID of the arduino (make sure this is unique)

#define SD_CS_PIN 4                           // "4" for MKR1400, "SS1" for MKRZero

#define SERIAL_SPEED 9600                     // Serial speed
#define STARTUP_DELAY 5000                    // How long to delay before the node starts

const uint32_t LANTERN_SLEEP_TIME = 60;       //the value is number of seconds = 1 min

//SIP THRESHOLDS
#define HEARTBEAT 86400                         // 1 day in secs
#define SIP_STEP_THRESHOLD 10                 // transmit if > 10 steps error

#define ADC_SOLAR_BATTERY A1                  // pin that solar battery is connected to.

#define BATTERY_LOW_VOLTAGE 816               // 816 / 1023 * 3.3 = 2.63V

//GSM
#define GSM_CONNECT_RETRY_TIME 60000            // How long to retry between gsm connection tries (1min)


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

/* voltage for solar battery can be calculated as REF_VOLTAGE * 2 *
   (adc_value / 1023) where REF_VOLTAGE is 3.3V. The multiplier of 2
   is due to the voltage divider with equal resistors.
 */

//ADXL345 settings
// TODO set threshold values based on data collected and learning - Decision trees or LDA
const int INTERRUPT_PIN = 5;
#define FREEFALL_TIME 20                      //// (20 - 70) recommended - 5ms per increment
#define FREEFALL_THRESH 14                  // 62.5mg per increment   // Set activity   // Activity thresholds (0-255) - Set to 0.7829181 g
#define ACTIVITY_THRESH 18                    // 62.5mg per increment   // Set activity   // Activity thresholds (0-255) - Set to 1.16137 g

#define INACTIVITY_INT_ENABLE 0
#define ACTIVITY_INT_ENABLE 1
#define FREEFALL_INT_ENABLE 1
#define DOUBLETAP_INT_ENABLE 0
#define TAP_INT_ENABLE 0

#define ACTIVITY_X_AXIS_ENABLE 1
#define ACTIVITY_Y_AXIS_ENABLE 0
#define ACTIVITY_Z_AXIS_ENABLE 0


//Macro for print time
#ifdef DEBUG
#define dbg(msg) _PRINT_TIME(String(msg))
#else
#define dbg(msg) do {} while (0)
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
