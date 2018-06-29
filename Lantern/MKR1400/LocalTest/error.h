
/* error codes should be prime. As each error occurs during a sense
   cycle, it is multiplied into last_errno. Factorising the
   resulting value gives both the number of occurences and the type.
*/

enum {
  ERR_SEND_FAILED = 2,
  ERR_SD_WRITE_FAILED = 3,
  ERR_LOW_BATTERY = 5,
  ERR_GSM_CONNECTION_FAILED = 7,
  ERR_MQTT_CONNECTION_FAILED = 11
  //ERR_HEARTBEAT = 13,
  //ERR_ACK_CRC_CORRUPT = 17,
  //ERR_PACK_STATE_OVERFLOW = 19
};

bool batteryLow = false;
uint32_t last_errno = 1;
uint32_t last_transmitted_errno;

