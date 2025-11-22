#include "components/TempSensor.h"
#include <Arduino.h>

bool TempSensor::setup(bool metric) {
    useMetric = metric; 

    if (!bmp.begin()) {
        Serial.println("BMP180 not found!");
        return false;
    }
    return true;
}

float TempSensor::readTemperature() {
    float tempC = bmp.readTemperature();
    if (useMetric) {
        return tempC;
    } else {
        return tempC * 9.0 / 5.0 + 32.0;
    }
}
