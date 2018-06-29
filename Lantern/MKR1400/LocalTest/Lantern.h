#define NODE_ID "1"                 //Set ID of the arduino (make sure this is unique)
#define STARTUP_DELAY 5000         //How long to delay before the node starts

//DEBUG
#define PRINTF 1                //Comment out to remove debuf messages
#define SERIAL_SPEED 9600       //Serial speed

#define SECRET_PINNUMBER     ""               // SIM Pin number
#define SECRET_GPRS_APN      "giffgaff.com"   // replace GPRS APN
#define SECRET_GPRS_LOGIN    "giffgaff"       // replace with GPRS login
#define SECRET_GPRS_PASSWORD ""               // replace with GPRS password
#define GSM_CONNECT_RETRY_TIME 500            // How long to retry between gsm connection tries (0.5s)

#define MQTT_SERVER "159.65.25.153" //MQTT broker address
#define MQTT_KEEP_ALIVE 36000       //MQTT keep alive time (10hours)
#define MQTT_CLEAN_SESSION false    //MQTT start a clean session on connection
#define MQTT_TIMEOUT 1000           //MQTT 1s timeout
#define MQTT_CONNECT_RETRY_TIME 500 //How long to retry between MQTTT broker connection tries (0.5s)

//Macro for print time
#ifdef PRINTF
# define debug(msg) _PRINT_TIME(String(msg))
#else
# define debug(msg) do {} while(0)
#endif

#define STATE_POWER_PIN 7                  //Arduino Pin the MAX31850 3V3 is connected to
#define STATE_MV_CONVERSION 1.023
#define DIGITAL_ON_WAIT 10          //Wait 10ms to ensure digital pin has switched high

#define ADC_MIN 0                 //Min value extect from the ADC
#define ADC_MAX 1023              //Max value extect from the ADC

#define RTC_SAMPLE_TIME 25        //SET RTC alarm off every xth second in a minute

#define INT_BATTERY_DIVIDER_MAX 4.2 //Max voltage that can be sensed from the internal voltage divider with 3.3v reference

#define ADC_VREF 3.468                    //Max voltage that can be sensed from the voltage divider
#define ADC_BITS 1023.0             //number of bits

#define BATTERY_LOW_VOLTAGE 3.35

//SIP THRESHHOLD
#define SIP_SOLAR_BATTERY_THRESH -1 // Threshold to define when the solar lantern battery is eventful
#define SIP_STATE_THRESH -1 // Threshold to define when the solar state has changed (needs testing)ful

#define INT_SOURCE_NO_INTERRUPT 0x83  //DATA_READY, Watermark, and Overun are always active so interrupt source will always read 0x83 if no other interrupt has been triggered
#define INACTIVITY_TIME 30
#define INACTIVITY_THRESH 75  // 62.5mg per increment   // Set activity   // Activity thresholds (0-255) (CHECK!!!!)
#define ACTIVITY_THRESH 75    // 62.5mg per increment   // Set activity   // Activity thresholds (0-255) (CHECK!!!!)

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




