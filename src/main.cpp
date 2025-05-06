#include <Arduino.h>
#include <config.h>
#include <Sensors.h>
#include <MqttWifiHandler.h>

Sensors sensors;
MqttWifiHandler mqttWifiHandler;

void setup() {
  Serial.begin(115200);

  // Initialisere MQTT og Wifi
  mqttWifiHandler.initialize(ssid, password, ca_cert, mqtt_server, mqtt_port);

  // Initialisere sensorer
  if (!sensors.initialize()) {
    Serial.println("Failed to initialize one or more sensors.");
    while(1);
  }
}

void loop() {
  mqttWifiHandler.check_connection(mqtt_user, mqtt_pass);

  static unsigned long lastMsg = 0;
  if (millis() - lastMsg > 5000) {
    lastMsg = millis();

    // Få payload
    const char* payload = sensors.get_payload();

    // Send payload til MQTT broker
    mqttWifiHandler.send_payload(payload);
#ifdef DEBUG
    Serial.print("Published: ");
    Serial.println(payload);
    Serial.println("-----");
#endif
  }

  delay(1000);
}
