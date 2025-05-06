#include "Sensors.h"
#include <Wire.h>

#define SDA_PIN 21
#define SCL_PIN 22

#define MIC_PIN 34 // input ben for MAX9814

bool Sensors::initialize() {
    Wire.begin(SDA_PIN, SCL_PIN);

    // byte error, address;
    // int nDevices = 0;

    // for (address = 1; address < 127; address++) {
    //     Wire.beginTransmission(address);
    //     error = Wire.endTransmission();

    //     if (error == 0) {
    //     Serial.print("I2C device found at address 0x");
    //     if (address < 16) Serial.print("0");
    //     Serial.print(address, HEX);
    //     Serial.println("  ✔");
    //     nDevices++;
    //     } else if (error == 4) {
    //     Serial.print("Unknown error at address 0x");
    //     if (address < 16) Serial.print("0");
    //     Serial.println(address, HEX);
    //     }
    // }

    // if (nDevices == 0)
    //     Serial.println("No I2C devices found ❌");
    // else
    //     Serial.println("Scan complete ✅");

    // if (!tempSensor.begin(0x76)) {
    //     Serial.println("Could not find a valid BME280 sensor, check wiring!");
    //     return false;
    // }

    // initialiser BH1750
    if (!lightMeter.begin(BH1750::CONTINUOUS_HIGH_RES_MODE)) {
        Serial.println("Kunne ikke initialisere BH1750.");
        return false;
    } 
    Serial.println("BH1750 initialiseret.");

    // initialiser CCS811
    if (!ccs.begin()) {
        Serial.println("CCS811 ikke fundet.");
        return false;
    }
    Serial.println("CCS811 initialiseret.");

    // venter på CCS811
    while (!ccs.available()) {
        delay(100);
    }

    // mikrofon
    analogReadResolution(12); // Optional: ESP32 default is 12 bits (0–4095)
    return true;
}

const char *Sensors::get_payload()
{
    static char payload[128];

    // Serial.print("Temperature = ");
    // Serial.print(tempSensor.readTemperature());
    // Serial.println(" °C");

    // Serial.print("Pressure = ");
    // Serial.print(tempSensor.readPressure() / 100.0F);
    // Serial.println(" hPa");

    // Serial.print("Humidity = ");
    // Serial.print(tempSensor.readHumidity());
    // Serial.println(" %");

    // Læser lys
    float lux = lightMeter.readLightLevel();
#ifdef DEBUG
    Serial.print("Light: ");
    Serial.print(lux);
    Serial.println(" lx");
#endif

    // Læser luft kvalitet
    int eco2 = -1;
    int tvoc = -1;
    if (ccs.available()) {
      if (!ccs.readData()) {
        eco2 = ccs.geteCO2();
        tvoc = ccs.getTVOC();
#ifdef DEBUG
        Serial.print("eCO2: ");
        Serial.print(eco2);
        Serial.print(" ppm, TVOC: ");
        Serial.print(tvoc);
        Serial.println(" ppb");
#endif
      } else {
#ifdef DEBUG
        Serial.println("CCS811 data read error.");
#endif
      }
    }

    // Læs mikrofon
    int micValue = analogRead(MIC_PIN);
#ifdef DEBUG
    Serial.print("Mikrofon: ");
    Serial.println(micValue);
#endif

    // Laver JSON payload
    snprintf(payload, sizeof(payload),
             "{\"light\":%.2f,\"eco2\":%d,\"tvoc\":%d,\"mic\":%d}",
             lux, eco2, tvoc, micValue);
    return payload;
}
