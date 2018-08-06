#include "Credentials.h"                      //Header file with MQTT server and SIM credentials. Look below for sample.                  

//GENERAL
#define NODE_ID "1"                           //Set ID of the arduino (make sure this is unique)
#define STARTUP_DELAY 10000                   //How long to delay before the node starts

//DEBUG
//#define DEBUG 1                             //Comment out to remove debug messages
//#define STORE 1                             //Comment out to not use sd card
#define SERIAL_SPEED 9600                   //Serial speed

//Macro for print time
#ifdef DEBUG
# define debug(msg) _PRINT_TIME(String(msg))
#else
# define debug(msg) _DO_NOTHING();
#endif

//GSM
#define GSM_CONNECT_RETRY_TIME 500            // How long to retry between gsm connection tries (0.5s)

//Sample period
#define RTC_SAMPLE_TIME 25                    //SET RTC alarm off every xth second in a minute

//Sensors
#define MAX31850_POWER_PIN 7                  //Arduino Pin the MAX31850 3V3 is connected to
#define MAX31850_ADDR 0                       //Address of MAX31850 on 1-wire bus 
#define ONE_WIRE_BUS 2                        //Arduino Pin the MAX31850 data line is connected to
#define Si7021_POWER_PIN A3                   //Arduino Pin the Si7021 power is connected to

#define ADC_BITS 1023.0                       //Max value extect from the ADC
#define INT_BATTERY_DIVIDER_MAX 4.2           //Max voltage that can be sensed from the internal voltage divider with 3.3v reference

//SIP THRESHOLDS
#define HEART_LIMIT 1440

#ifndef DEBUG
#define BATTERY_LOW_VOLTAGE -1              //Low battery threshold (3.35 suggested, set to -1 to disable)
#define SIP_TEMP_THERMOCOUPLE_THRESH -1      //Temperature threshold (0.5C suggested, set to -1 for sense-and-send)
#define SIP_TEMP_SI7021_THRESH 0.5           //Temperature threshold (0.5C suggested, set to -1 for sense-and-send)
#define SIP_HUMIDITY_THRESH 2                 //Humidity threshold (2% suggested, set to -1 for sense-and-send)
#else
#define BATTERY_LOW_VOLTAGE -1
#define SIP_TEMP_THERMOCOUPLE_THRESH -1
#define SIP_TEMP_SI7021_THRESH -1
#define SIP_HUMIDITY_THRESH -1
#endif

//MQTT
#define MQTT_KEEP_ALIVE 36000                 //MQTT keep alive time (10hours)
#define MQTT_CLEAN_SESSION true              //MQTT start a clean session on connection
#define MQTT_TIMEOUT 1000                     //MQTT 1s timeout
#define MQTT_CONNECT_RETRY_TIME 500           //How long to retry between MQTTT broker connection tries (0.5s)
#define MQTT_JSON_BUFFER 200                  //Calculated at https://arduinojson.org/v5/assistant/
#define NUM_JSON_FIELDS 8
#define DIGITAL_ON_WAIT 10                    //Wait 10ms to ensure digital pin has switched high

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


