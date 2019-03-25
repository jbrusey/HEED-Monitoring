#pragma once
#include "Credentials.h"                      // Header file with MQTT server and SIM credentials. Look below for sample.

//GENERAL
#define NODE_ID "1"                           // Set ID of the arduino (make sure this is unique)

#define SD_CS_PIN 4                           // "4" for MKR1400, "SS1" for MKRZero

#define SERIAL_SPEED 9600                     // Serial speed
#define STARTUP_DELAY 5000                    // How long to delay before the node starts

//SAMPLE PERIOD
#define RTC_SAMPLE_TIME 25                    // SET RTC alarm off every xth second in a minute
#define CS_SLEEP_TIME 60000                    //Set the device to sleep mode for a min

//SIP THRESHOLDS
#define HEART_LIMIT 1440

#define BATTERY_LOW_VOLTAGE 3.35              // Low battery threshold (3.35 suggested, set to -1 to disable)
#define SIP_TEMP_THERMOCOUPLE_THRESH 5        // Temperature threshold (0.5C suggested, set to -1 for sense-and-send)
#define SIP_TEMP_SI7021_THRESH 5              // Temperature threshold (0.5C suggested, set to -1 for sense-and-send)
#define SIP_HUMIDITY_THRESH 10                // Humidity threshold (2% suggested, set to -1 for sense-and-send)


//GSM
#define GSM_CONNECT_RETRY_TIME 500            // How long to retry between gsm connection tries (0.5s)


//MQTT
#define MQTT_KEEP_ALIVE 36000                 // MQTT keep alive time (10hours)
#define MQTT_CLEAN_SESSION true               // MQTT start a clean session on connection
#define MQTT_TIMEOUT 1000                     // MQTT 1s timeout
#define MQTT_CONNECT_RETRY_TIME 500           // How long to retry between MQTTT broker connection tries (0.5s)
#define MQTT_JSON_BUFFER 200                  // Calculated at https://arduinojson.org/v5/assistant/
#define NUM_JSON_FIELDS 8


//Sensors
#define DIGITAL_ON_WAIT 100                   // Wait 10ms to ensure digital pin has switched high

#define MAX31850_POWER_PIN A2                 // Arduino Pin the MAX31850 3V3 is connected to
#define MAX31850_ADDR 0                       // Address of MAX31850 on 1-wire bus
#define ONE_WIRE_BUS 0                        // Arduino Pin the MAX31850 data line is connected to
#define Si7021_POWER_PIN A1                   // Arduino Pin the Si7021 power is connected to

#define ADC_BITS 1023.0                       // Max value extect from the ADC
#define INT_BATTERY_DIVIDER_MAX 4.2           // Max voltage that can be sensed from the internal voltage divider with 3.3v reference


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
