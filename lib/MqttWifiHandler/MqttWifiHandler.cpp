#include "MqttWifiHandler.h"

void MqttWifiHandler::reconnect(const char* mqtt_user, const char* mqtt_pass) {
  while (!client.connected()) {
    Serial.print("Prøver at forbinde til MQTT broker...");
   if (client.connect("espresso32", mqtt_user, mqtt_pass)) {
      Serial.println("Forbundet til MQTT.");
    } else {
      Serial.print("Fejl: kunne ikke forbinde til MQTT broker.");
      delay(3000);
    }
  }
}

void MqttWifiHandler::initialize(
    const char* ssid,
    const char* password,
    const char* ca_cert,
    const char* mqtt_server,
    const uint16_t& mqtt_port
) 
{
    this->client = PubSubClient(secureClient);

    Serial.println("Forbinder til WiFi...");
    WiFi.begin(ssid, password);
    while (WiFi.status() != WL_CONNECTED) { delay(500); }
    Serial.println("\nForbundet til WiFi");

    secureClient.setCACert(ca_cert);
    client.setServer(mqtt_server, mqtt_port);
}

void MqttWifiHandler::check_connection(const char* mqtt_user, const char* mqtt_pass)
{
    if (!client.connected()) {
        reconnect(mqtt_user, mqtt_pass);
    }
    client.loop();
}

void MqttWifiHandler::send_payload(const char *payload)
{
    // Lever til MQTT Broker
    if (client.publish("sensor/data", payload)) {
        Serial.println("MQTT publish lykkedes!");
    } else {
        Serial.println("MQTT publish FEJLEDE!");
    }
}
