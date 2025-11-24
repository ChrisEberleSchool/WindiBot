#ifndef LOCATION_DATA_SCREEN_H
#define LOCATION_DATA_SCREEN_H

#include <display/IScreen.h>
#include <display/DisplayManager.h>
#include <Arduino.h>
#include "api/LocationAPI.h"
#include "api/WeatherAPI.h"
#include <SettingsManager.h>

class LocationDataScreen : public IScreen {
public:
    LocationDataScreen() = default;
    
    void draw() override {
        UIHelper::getInstance().setFont(FontStyle::MenuBold);
        UIHelper::getInstance().drawHeader("Location Menu");
        UIHelper::getInstance().setFont(FontStyle::RetroTiny);

        bool isMetric = SettingsManager::getInstance().loadUnits();

        // LOCATION
        String locString = "Location: " + LocationAPI::getInstance().getCity();
        UIHelper::getInstance().drawMessage(locString.c_str(), 0, 28);

        // WEATHER CODE
        String weatherDesc = "Weather Status: " + WeatherAPI::getInstance().getWeatherDescription();
        UIHelper::getInstance().drawMessage(weatherDesc.c_str(), 0, 36);

        // TEMPERATURE
        float temp = WeatherAPI::getInstance().getTemperature(isMetric);
        String tempStr = "Temp: " + String(temp, 1) + (isMetric ? " C" : " F");
        UIHelper::getInstance().drawMessage(tempStr.c_str(), 0, 44);

        // WIND SPEED
        float wind = WeatherAPI::getInstance().getWindSpeed(isMetric);
        String windStr = "Wind: " + String(wind, 1) + (isMetric ? " m/s" : " mph");
        UIHelper::getInstance().drawMessage(windStr.c_str(), 0, 52);
    }


    void onRotation(int rot) override {
    }   

    void onButtonPress() override {
        changeScreen(0); 
    }

};

#endif
