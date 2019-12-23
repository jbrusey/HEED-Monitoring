/** @file error.h
 *  @brief This contains the constants for error codes. 
 *  
 *  @author Kriti Bhargava
 *  @author James Brusey
 *  @bug No known bugs.
 */

/* error codes should be prime. As each error occurs during a sense
   cycle, it is multiplied into last_errno. Factorising the
   resulting value gives both the number of occurences and the type.
*/

enum {
  ERR_GSM_CONNECTION_FAILED = 2,
  ERR_MQTT_CONNECTION_FAILED = 3,
  ERR_MQTT_TRANSMISSION_FAILED = 5,
  ERR_CSV_OPEN = 7,
  ERR_CSV_WRITE = 11,
  ERR_GSM_ATTACH_FAILED = 13
  };

bool batteryLow = false;
uint32_t last_errno = 1;
uint32_t last_transmitted_errno;
