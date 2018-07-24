#include "Credentials.h"                      //Header file with MQTT server and SIM credentials. Look below for sample.                  

//GENERAL
#define NODE_ID "1"                           //Set ID of the arduino (make sure this is unique)
#define STARTUP_DELAY 5000                    //How long to delay before the node starts

//DEBUG
#define DEBUG 1                               //Comment out to remove debug messages
#define STORE 1                               //Comment out to not use sd card

#define SERIAL_SPEED 9600                     //Serial speed

//Macro for print time
#ifdef DEBUG
# define debug(msg) _PRINT_TIME(String(msg))
#else
# define debug(msg) _DO_NOTHING();
#endif

//GSM
#define GSM_CONNECT_RETRY_TIME 500            // How long to retry between gsm connection tries (0.5s)

//SAMPLE PERIOD
#define RTC_SAMPLE_TIME 25                    //SET RTC alarm off every xth second in a minute

//Analog sensor settings
#define STATE_POWER_PIN 7                     //Arduino Pin the MAX31850 3V3 is connected to
#define STATE_MV_CONVERSION 1.023
#define DIGITAL_ON_WAIT 10                    //Wait 10ms to ensure digital pin has switched high

#define ADC_MIN 0                             //Min value extect from the ADC
#define ADC_MAX 1023                          //Max value extect from the ADC

#define INT_BATTERY_DIVIDER_MAX 4.2           //Max voltage that can be sensed from the internal voltage divider with 3.3v reference

#define ADC_VREF 6.6                          //Max voltage that can be sensed from the voltage divider
#define ADC_BITS 1023.0                       //number of bits


//ADXL345 settings
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

//SIP THRESHHOLD

#define HEART_LIMIT 1440

#ifdef DEBUG
#define BATTERY_LOW_VOLTAGE -1
#define SIP_SOLAR_BATTERY_THRESH -1           // Enable sense and send
#define SIP_STATE_THRESH -1                   // Enable sense and send
#define INT_SOURCE_NO_INTERRUPT 0x00          //DATA_READY, Watermark, and Overun are always active so interrupt source will always read 0x83 if no other interrupt has been triggered
#else
#define BATTERY_LOW_VOLTAGE 3.35
#define SIP_SOLAR_BATTERY_THRESH -1           // Threshold to define when the solar lantern battery is eventful (TO-DO)
#define SIP_STATE_THRESH -1                     // Threshold to define when the solar state has changed (TO-DO)
#define INT_SOURCE_NO_INTERRUPT 0x83          //DATA_READY, Watermark, and Overun are always active so interrupt source will always read 0x83 if no other interrupt has been triggered
#endif

//MQTT
#define MQTT_KEEP_ALIVE 36000                 //MQTT keep alive time (10hours)
#define MQTT_CLEAN_SESSION false              //MQTT start a clean session on connection
#define MQTT_TIMEOUT 1000                     //MQTT 1s timeout
#define MQTT_CONNECT_RETRY_TIME 500           //How long to retry between MQTTT broker connection tries (0.5s)
#define MQTT_JSON_BUFFER 200                  //Calculated at https://arduinojson.org/v5/assistant/
#define NUM_JSON_FIELDS 12

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


