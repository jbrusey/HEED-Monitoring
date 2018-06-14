//Include MQTT library
#include <MQTT.h>
MQTTClient client;

//SETUP TOPIC STRTNG BASED ON NODE_ID 
const String MQTT_TOPIC = String("Lantern/") + NODE_ID + "/data";

/**
 * Connects the node to the MQTT broker
 */
 void connectMQTT() {
  if (!client.connected())
  {
    client.begin(MQTT_SERVER, net);  //Start a connection
    //Set options: Keep alive for 10 hours, clean session, 1 second timeout
    client.setOptions(MQTT_KEEP_ALIVE, MQTT_CLEAN_SESSION, MQTT_TIMEOUT);
    debug("connecting to broker...");
    while (!client.connect("arduino")) {
      delay(MQTT_CONNECT_RETRY_TIME);
    }
    debug("connected!");
  }
}


/**
 * Disonnects the node from the MQTT broker
 */
void disconnectMQTT() {
  client.disconnect();
  debug("MQTT disconnected"); 
}

/**
 * Transmits a packet via MQTT
 * @param topic The MQTT topic to send to
 * @param dataString The String to be sent
 * @return True if the String was saved
 */
bool transmit(String topic, String dataString) {
  bool res = client.publish(topic, dataString);
  return res;
}

