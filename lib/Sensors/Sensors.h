#ifndef SENSORS_H
#define SENSORS_H

#include <BH1750.h>
#include <Adafruit_BME280.h>
#include "Adafruit_CCS811.h"

class Sensors {
private:
    BH1750 lightMeter;
    Adafruit_CCS811 ccs;
    Adafruit_BME280 tempSensor;
public:
    bool initialize();
    const char* get_payload();
};

#endif