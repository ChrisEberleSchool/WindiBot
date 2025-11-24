#include <components/TempSensor.h>
#include <Arduino.h>

TempSensor& TempSensor::getInstance() {
    static TempSensor instance;
    return instance;
}

TempSensor::TempSensor()  {}

bool TempSensor::setup() {
    if (!bmp.begin()) {
        Serial.println("BMP180/085 not found!");
        return false;
    }
    return true;
}

float TempSensor::readTemperature(bool metric) {
    float tempC = bmp.readTemperature();
    return metric ? tempC : cToF(tempC);
}

float TempSensor::readCelsius() {
    return bmp.readTemperature();
}

float TempSensor::readFahrenheit() {
    return cToF(bmp.readTemperature());
}

float TempSensor::cToF(float tempC) {
    return tempC * 9.0 / 5.0 + 32.0;
}
