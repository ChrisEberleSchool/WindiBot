#ifndef OPERATION_SCREEN_H
#define OPERATION_SCREEN_H

#include <display/IScreen.h>
#include <display/DisplayManager.h>
#include <Arduino.h>
#include <display/UIHelper.h>
#include "WeatherControl.h"
#include <components/Encoder.h>
#include <components/TempSensor.h>
#include <api/WeatherAPI.h>
#include <SettingsManager.h>

class OperationScreen : public IScreen {
public:
    OperationScreen() : state(INIT), lastHandleTime(0), prefTempValue(0.0f) {}

    void init() override {
        // must be reset each time screen is shown
        state = INIT;
        lastHandleTime = 0;
        prefTempValue = 20.0f;
        needsRedraw = true;
         
        DisplayManager::getInstance().clearBuffer();
        draw();
        DisplayManager::getInstance().sendBuffer();

        WeatherAPI::getInstance().update(); 

        // Check calibration status
        if (WeatherControl::getInstance().loadRotations() <= 0.0f) 
            state = NOT_CALIBRATED;
        else 
            state = SET_PREF_TEMP;
    }
    
    void draw() override {
        UIHelper::getInstance().setFont(FontStyle::MenuBold);
        UIHelper::getInstance().drawHeader("Operation Menu");
        UIHelper::getInstance().setFont(FontStyle::MenuSmall);
        
        switch(state) {
            case INIT: {
                UIHelper::getInstance().drawMessage("Loading...", 0, 40);
                break;
            }

            case NOT_CALIBRATED: {
                UIHelper::getInstance().drawMessage("Calibration Required", 0, 30);
                UIHelper::getInstance().drawMessage("Press button to return", 0, 45);
                break;
            }

            case SET_PREF_TEMP: {
                UIHelper& ui = UIHelper::getInstance();
                bool useMetric = SettingsManager::getInstance().loadUnits();
            
                ui.drawMessage("Set Preferred Temp:", 0, 30);
            
                String unit = useMetric ? " C" : " F";
                String msg = String(prefTempValue, 1) + unit;
                ui.drawMessage(msg.c_str(), 0, 45);
            
                ui.drawMessage("Press button to confirm", 0, 60);
                break;
            }

            case OPERATION: {
                drawOperationStatus();
                break;
            }
        }
    }

    void drawOperationStatus() {
        UIHelper& ui = UIHelper::getInstance();
        ui.setFont(FontStyle::RetroTiny);

        bool isOpen = WeatherControl::getInstance().loadWindowState();
        String windowStatus = "Window Status: " + String(isOpen ? "OPEN" : "CLOSED");

        ui.drawMessage(windowStatus.c_str(), 0, 24);

        bool weatherGuard = SettingsManager::getInstance().loadWeatherGuard();
        bool useMetric = SettingsManager::getInstance().loadUnits();

        // Weather message
        String weatherDesc = WeatherAPI::getInstance().getWeatherDescription();
        String weatherMsg;
        if (WeatherAPI::getInstance().isBadWeather() && weatherGuard) {
            weatherMsg = "WEATHER GUARDED: (" + weatherDesc + ")";
        } else {
            weatherMsg = "Weather: " + weatherDesc;
        }
        ui.drawMessage(weatherMsg.c_str(), 0, 36);

        // Target Temp
        float displayPrefTemp = useMetric ? prefTempValue : (prefTempValue * 9.0 / 5.0 + 32.0);
        ui.drawMessage(("Target Temp: " + String(displayPrefTemp, 1) + (useMetric ? " C" : " F")).c_str(), 0, 44);

        // Indoor Temp
        float indoorTemp = TempSensor::getInstance().readTemperature(useMetric);
        ui.drawMessage(("Indoor Temp: " + String(indoorTemp, 1) + (useMetric ? " C" : " F")).c_str(), 0, 52);

        // Outdoor Temp
        float outdoorTemp = WeatherAPI::getInstance().getTemperature(useMetric);
        ui.drawMessage(("Outdoor Temp: " + String(outdoorTemp, 1) + (useMetric ? " C" : " F")).c_str(), 0, 60);

        // Blocking message
        if (isBlocking) {
            ui.drawMessage("Adjusting Window... Input BLOCKED", 0, 68);
        }
    }

    void update() override {
        // Only redraw if needed
        if (needsRedraw) {
            DisplayManager::getInstance().clearBuffer();
            draw();
            DisplayManager::getInstance().sendBuffer();
            needsRedraw = false;
        }

        // Handle open/close every 5 minutes in OPERATION mode
        if (state == OPERATION) {
            unsigned long now = millis();
            unsigned long intervalMs = static_cast<unsigned long>(handleIntervalMinutes * 60 * 1000UL);
        
            if (lastHandleTime == 0 || now - lastHandleTime >= intervalMs) {
                WeatherAPI::getInstance().update();
            
                // If rain/snow but weatherguard off still operate
                // If rain/snow and weatherguard on block operation
                if(!(WeatherAPI::getInstance().isBadWeather()) || !SettingsManager::getInstance().loadWeatherGuard()) {
                    isBlocking = true;
                    WeatherControl::getInstance().handleOpenClose(prefTempValue);
                    isBlocking = false;
                }
            
                lastHandleTime = now;
                needsRedraw = true;
            }
        }
    }

    void onRotation(int rot) override {
        if (state == SET_PREF_TEMP && rot != 0) {
            bool useMetric = SettingsManager::getInstance().loadUnits();

            // Determine increment step
            float step = useMetric ? 0.5f : 1.0f; // 0.5°C or 1°F

            prefTempValue += step * rot;

            // Apply unit-specific limits
            if (useMetric) {
                if (prefTempValue < -20.0f) prefTempValue = -20.0f;
                if (prefTempValue > 40.0f)  prefTempValue = 40.0f;
            } else {
                if (prefTempValue < -4.0f)  prefTempValue = -4.0f;   // -20°C → -4°F
                if (prefTempValue > 104.0f) prefTempValue = 104.0f;  // 40°C → 104°F
            }
        }

        needsRedraw = true;
    }

    void onButtonPress() override {
        needsRedraw = true;
        if( state == SET_PREF_TEMP ){
            state = OPERATION;
        }
        else if ( state == OPERATION || state == NOT_CALIBRATED) {
            state = INIT;
            lastHandleTime = 0;
            prefTempValue = 20.0f;
            // Close The Window before exiting
            WeatherControl::getInstance().closeWindow();
            changeScreen(0);
        }
    }

private:
    enum ScreenState {
        INIT,
        OPERATION,
        NOT_CALIBRATED,
        SET_PREF_TEMP
    };

    ScreenState state;
    unsigned long lastHandleTime;
    float handleIntervalMinutes = 0.5f; 
    float prefTempValue;

    bool isBlocking = false;

    bool needsRedraw = true;
};

#endif
