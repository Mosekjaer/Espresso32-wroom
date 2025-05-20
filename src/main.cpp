#include <Arduino.h>
#include <config.h>
#include <Sensors.h>
#include <MqttWifiHandler.h>
#include "time.h"

Sensors sensors;
MqttWifiHandler mqttWifiHandler;

const long gmtOffset_sec = 0;
const int daylightOffset_sec = 0;
const char* ntpServer = "pool.ntp.org";

void setup() {
  Serial.begin(115200);

  mqttWifiHandler.initialize(ssid, password, ca_cert, mqtt_server, mqtt_port);

  configTime(gmtOffset_sec, daylightOffset_sec, ntpServer);
  Serial.println("Venter på NTP synkronisering...");

  struct tm timeinfo;
  while (!getLocalTime(&timeinfo)) {
    Serial.print(".");
    delay(500);
  }

  if (!sensors.initialize()) {
    Serial.println("Failed to initialize one or more sensors.");
    while(1);
  }

  mqttWifiHandler.check_connection(mqtt_user, mqtt_pass);

  const char* payload = sensors.get_payload();
  if (payload != nullptr) {
    mqttWifiHandler.send_payload(payload);
#ifdef DEBUG
    Serial.print("Published: ");
    Serial.println(payload);
    Serial.println("-----");
#endif
  }
  delay(1000); // vent et sekund til pakken er sendt
  // Sæt ESP32 i deep sleep i 5 minutter
  esp_sleep_enable_timer_wakeup(300 * 1000000); // 5 minutes
  esp_deep_sleep_start();
}


void loop() {
  
}
