#ifndef TEMP_SENSOR_H
#define TEMP_SENSOR_H

#include <Adafruit_BMP085.h>

class TempSensor {
public:
    // Access the single instance
    static TempSensor& getInstance() {
        static TempSensor instance;
        return instance;
    }

    bool setup(bool metric = true);
    float readTemperature();

private:
    TempSensor() {}                            
    TempSensor(const TempSensor&) = delete;    
    void operator=(const TempSensor&) = delete;

    Adafruit_BMP085 bmp;
    bool useMetric = true;
};

#endif
