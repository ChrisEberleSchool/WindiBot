#ifndef TEMP_SENSOR_H
#define TEMP_SENSOR_H

#include <Adafruit_BMP085.h>

class TempSensor {
public:
    // Access the single instance
    static TempSensor& getInstance();

    // Initialize sensor; set default units
    bool setup();

    // Read temperature based on current unit setting
    float readTemperature(bool metric);

    // Read temperature explicitly in Celsius
    float readCelsius();

    // Read temperature explicitly in Fahrenheit
    float readFahrenheit();
private:
    TempSensor();                            
    TempSensor(const TempSensor&) = delete;    
    void operator=(const TempSensor&) = delete;

    float cToF(float tempC);

    Adafruit_BMP085 bmp;
};

#endif
