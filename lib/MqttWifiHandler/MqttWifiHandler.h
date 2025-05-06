#ifndef MQTTWIFIHANDLER_H
#define MQTTWIFIHANDLER_H

#include <PubSubClient.h>
#include <WiFiClientSecure.h>

class MqttWifiHandler {
private:
    WiFiClientSecure secureClient;
    PubSubClient client;
    void reconnect(const char* mqtt_user, const char* mqtt_pass);
public:
    void initialize(const char* ssid, const char* password, const char* ca_cert, const char* mqtt_server, const uint16_t& mqtt_port);
    void check_connection(const char* mqtt_user, const char* mqtt_pass);
    void send_payload(const char* payload);
};

#endif