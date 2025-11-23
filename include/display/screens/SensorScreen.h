#ifndef SENSOR_SCREEN_H
#define SENSOR_SCREEN_H

#include <display/IScreen.h>
#include <display/DisplayManager.h>
#include <Arduino.h>
#include <components/TempSensor.h>

class SensorScreen : public IScreen {
public:
    SensorScreen() = default;
    
    void draw() override {
        UIHelper::getInstance().setFont(FontStyle::MenuBold);
        UIHelper::getInstance().drawHeader("Sensor Menu");
        UIHelper::getInstance().setFont(FontStyle::MenuSmall);
        float temp = TempSensor::getInstance().readTemperature();
        char buf[32];
        sprintf(buf, "Inside Temp: %.1f C", temp);
        UIHelper::getInstance().drawMessage(buf, 0, 30);
    }

    void onRotation(int rot) override {
    }   

    void onButtonPress() override {
        changeScreen(0); 
    }
private:
};

#endif
