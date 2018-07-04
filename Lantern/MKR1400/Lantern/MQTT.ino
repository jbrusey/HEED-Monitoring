//Include MQTT library
#include <MQTT.h>
MQTTClient client;

//SETUP TOPIC STRTNG BASED ON NODE_ID 
const String MQTT_TOPIC = String("Lantern/") + NODE_ID + "/data";

/**
 * Connects the node to the MQTT broker
 */
bool connectMQTT() {
  bool mqtt_connected = false;
  if (!client.connected())
  {
    client.begin(MQTT_SERVER, net);  //Start a connection
    //Set options: Keep alive for 10 hours, clean session, 1 second timeout
    client.setOptions(MQTT_KEEP_ALIVE, MQTT_CLEAN_SESSION, MQTT_TIMEOUT);
    debug("MQTT: Connecting to broker...");
    // TODO: Implement some timeout here - the connection process can sometimes freeze indefinetely
    bool res = client.connect("arduino");
    if(res){
      mqtt_connected = true;
      debug("MQTT: Connected!");
    }
    else
    {
      reportError(ERR_MQTT_CONNECTION_FAILED);
      debug("MQTT: Can't connect!");
    }  
  }
  else mqtt_connected = true;
  return mqtt_connected;
}


/**
 * Disonnects the node from the MQTT broker
 */
void disconnectMQTT() {
  client.disconnect();
  debug("MQTT: Disconnected"); 
}

/**
 * Transmits a packet via MQTT
 * @param topic The MQTT topic to send to
 * @param dataString The String to be sent
 * @return True if the String was saved
 */
bool transmit(String topic, String dataString) {
  bool res = client.publish(topic, dataString);
  if (!res){
    reportError(ERR_MQTT_TRANSMISSION_FAILED);
    debug("ERROR: MQTT: Data not sent!");
  }
  else debug("MQTT: Data sent!");
  return res;
}

