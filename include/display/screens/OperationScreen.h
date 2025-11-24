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
        setWeatherState();  

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
                UIHelper::getInstance().drawMessage("Set Preferred Temp:", 0, 30);
                String msg = String(prefTempValue, 1) + " C";
                UIHelper::getInstance().drawMessage(msg.c_str(), 0, 45);
                UIHelper::getInstance().drawMessage("Press button to confirm", 0, 60);
                break;
            }

            case OPERATION: {
                UIHelper::getInstance().setFont(FontStyle::RetroTiny);
                UIHelper::getInstance().drawMessage("System Operational", 0, 20);
                if(weatherState == RAINING) {
                    UIHelper::getInstance().drawMessage("Staying closed (RAIN)", 0, 28);
                } else if(weatherState == SNOWING) {
                    UIHelper::getInstance().drawMessage("Staying closed (SNOW)", 0, 28);
                } else {
                    UIHelper::getInstance().drawMessage("Weather: Clear", 0, 28);
                }
                String msg = "Target Temp: " + String(prefTempValue, 1) + " C";
                UIHelper::getInstance().drawMessage(msg.c_str(), 0, 36);
                msg = "Indoor Temp: " + String(TempSensor::getInstance().readTemperature(), 1) + " C";
                UIHelper::getInstance().drawMessage(msg.c_str(), 0, 44);
                msg = "Outdoor Temp: " + String(WeatherAPI::getInstance().getTemperature(), 1) + " C";
                UIHelper::getInstance().drawMessage(msg.c_str(), 0, 52);
                if(isBlocking) {
                    UIHelper::getInstance().drawMessage("Adjusting Window... Input BLOCKED", 0, 60);
                }
                break;
            }
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
                setWeatherState();
                // If raining/snowing, skip opening logic
                if (weatherState == RAINING || weatherState == SNOWING) {
                    needsRedraw = true;
                    lastHandleTime = now;
                    return;
                }

                isBlocking = true;
                DisplayManager::getInstance().clearBuffer();
                draw();
                DisplayManager::getInstance().sendBuffer();

                WeatherControl::getInstance().handleOpenClose(prefTempValue); // blocking

                isBlocking = false;
                lastHandleTime = now;
                needsRedraw = true;
            }
        }
    }

    void setWeatherState() {
        if (WeatherAPI::getInstance().isRaining()) {
            weatherState = RAINING;
        } 
        else if (WeatherAPI::getInstance().isSnowing()) {
            weatherState = SNOWING;
        } 
        else {
            weatherState = CLEAR;
        }
    }

    void onRotation(int rot) override {
        if (state == SET_PREF_TEMP && rot != 0) {
            prefTempValue += 0.5f * rot;
            if (prefTempValue < -20.0f) prefTempValue = -20.0f;
            if (prefTempValue > 40.0f) prefTempValue = 40.0f;
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

    enum WeatherState {
        CLEAR,
        RAINING,
        SNOWING
    };
    WeatherState weatherState = CLEAR;

    ScreenState state;
    unsigned long lastHandleTime;
    float handleIntervalMinutes = 0.5f; 
    float prefTempValue;

    bool isBlocking = false;

    bool needsRedraw = true;
};

#endif
