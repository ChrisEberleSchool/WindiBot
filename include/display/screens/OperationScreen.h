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
    
    void draw() override {
        UIHelper::getInstance().setFont(FontStyle::MenuBold);
        UIHelper::getInstance().drawHeader("Operation Menu");
        UIHelper::getInstance().setFont(FontStyle::MenuSmall);
        
        switch(state) {
            case INIT: {
                if (WeatherControl::getInstance().loadRotations() <= 0.0f) {
                    UIHelper::getInstance().drawMessage("Please Calibrate Before Use", 0, 30);
                    UIHelper::getInstance().drawMessage("Press To Return...", 0, 45);
                    state = NOT_CALIBRATED;
                } else if (WeatherControl::getInstance().loadPrefTemp() == 0.0f) {
                    prefTempValue = 20.0f; // default starting value
                    state = SET_PREF_TEMP;
                } else {
                    state = OPERATION;
                }
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
                UIHelper::getInstance().drawMessage("System Operational", 0, 30);
                String msg = "Target Temp: " + String(prefTempValue, 1) + " C";
                UIHelper::getInstance().drawMessage(msg.c_str(), 0, 40);
                msg = "Indoor Temp: " + String(TempSensor::getInstance().readTemperature(), 1) + " C";
                UIHelper::getInstance().drawMessage(msg.c_str(), 0, 50);
                msg = "Outdoor Temp: " + String(WeatherAPI::getInstance().getTemperature(), 1) + " C";
                UIHelper::getInstance().drawMessage(msg.c_str(), 0, 60);
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
                WeatherControl::getInstance().handleOpenClose(); // blocking
                lastHandleTime = now;
                needsRedraw = true;
            }
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
        if (state == NOT_CALIBRATED || state == INIT) {
            state = INIT;
            changeScreen(0);
            WeatherControl::getInstance().savePrefTemp(0.0f);
        }
        else if( state == SET_PREF_TEMP ) {
            WeatherControl::getInstance().savePrefTemp(prefTempValue);
            state = OPERATION;
        }
        else if ( state == OPERATION) {
            state = INIT;
            changeScreen(0);
            WeatherControl::getInstance().savePrefTemp(0.0f);
        }
        needsRedraw = true;
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

    bool needsRedraw = true;
};

#endif
