#ifndef LOCATION_DATA_SCREEN_H
#define LOCATION_DATA_SCREEN_H

#include <display/IScreen.h>
#include <display/DisplayManager.h>
#include <Arduino.h>
#include "api/LocationAPI.h"
#include "api/WeatherAPI.h"

class LocationDataScreen : public IScreen {
public:
    LocationDataScreen() = default;
    
    void draw() override {
        UIHelper::getInstance().setFont(FontStyle::MenuBold);
        UIHelper::getInstance().drawHeader("Location Menu");
        UIHelper::getInstance().setFont(FontStyle::MenuSmall);

        // LOCATION
        String locString = "Location: " + LocationAPI::getInstance().getCity();
        UIHelper::getInstance().drawMessage(locString.c_str(),0,30);
        // TEMPERATURE with units
        float temp = WeatherAPI::getInstance().getTemperature();
        String tempStr = "Temp: " + String(temp, 1) + " C";   // °C shown
        UIHelper::getInstance().drawMessage(tempStr.c_str(),0,45);

        // WIND SPEED with units
        float wind = WeatherAPI::getInstance().getWindSpeed();
        String windStr = "Wind: " + String(wind, 1) + " m/s";  // m/s shown
        UIHelper::getInstance().drawMessage(windStr.c_str(),0,60);
    }

    void onRotation(int rot) override {
    }   

    void onButtonPress() override {
        changeScreen(0); 
    }

};

#endif
