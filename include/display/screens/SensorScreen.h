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
        char buf[32];
        sprintf(buf, "Inside Temp: %.1f C", temp);
        UIHelper::getInstance().drawMessage(buf, 0, 30);
    }

    void update() override {
        static unsigned long lastUpdate = 0;
        unsigned long now = millis();

        // First time load or every 3 seconds
        if (lastUpdate == 0 || now - lastUpdate >= updateIntervalSeconds * 1000) {
            lastUpdate = now;

            // Read the sensor here
            temp = TempSensor::getInstance().readTemperature();

            // Update display
            DisplayManager::getInstance().clearBuffer();
            draw();
            DisplayManager::getInstance().sendBuffer();
        }
    }


    void onRotation(int rot) override {
    }   

    void onButtonPress() override {
        changeScreen(0); 
    }
private:
    float temp = -99.9f;
    int updateIntervalSeconds = 3;
};

#endif
