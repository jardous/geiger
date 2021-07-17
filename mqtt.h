/*
Functions for logging to MQTT (used by HomeAssistant)
 
   Configuration (HA):
    sensor 1:
      platform: mqtt
      state_topic: 'sensors/geiger'
      name: 'Radiation'
      unit_of_measurement: 'cpm'
      value_template: '{{ value_json.cpm }}'
*/

#include <PubSubClient.h>
#include <ArduinoJson.h>

const PROGMEM uint16_t MQTT_SERVER_PORT = 1883;
const PROGMEM char* MQTT_USER = "";
const PROGMEM char* MQTT_PASSWORD = "";

const PROGMEM char* MQTT_SENSOR_TOPIC = "sensors/geiger";

WiFiClient mqtt_client;
PubSubClient ps_client(mqtt_client);


// function called to publish the cpm
void publishData(int cpm)
{
      DynamicJsonDocument doc(128);
      doc["cpm"] = (String)cpm;
      serializeJson(doc, Serial);
      Serial.println("");
      /*
         {
            "cpm": "23.20" ,
         }
      */
      char data[200];
      serializeJson(doc, data);
      Serial.println(data);
      ps_client.publish(MQTT_SENSOR_TOPIC, data, true);
      yield();
}

// TCP client reply (MQTT message arrived)
void callback(char* p_topic, byte* p_payload, unsigned int p_length)
{
    char data[200];
    Serial.println(p_topic);
    sprintf(data, "%s", p_payload, p_length);
    Serial.println(data);
}

static void logToMqtt(int cpm)
{
    // Attempt to connect
    if (ps_client.connect(MQTT_CLIENT_ID, MQTT_USER, MQTT_PASSWORD)) {
        Serial.println("INFO: connected");
    } else {
        Serial.print("ERROR: failed, rc=");
        Serial.print(ps_client.state());
    }

    if (cpm == 0) return;
    if (WiFi.status() != WL_CONNECTED) return;

    Serial.print("connecting to ");
    Serial.println(MQTT_SERVER_IP);

    // init the MQTT connection
    ps_client.setServer(MQTT_SERVER_IP, MQTT_SERVER_PORT);
    ps_client.setCallback(callback);

    // We now create a URI for the request
    publishData(cpm);

    Serial.println();
    Serial.println("closing connection");

    ps_client.disconnect();
}
