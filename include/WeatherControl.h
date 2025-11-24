#ifndef WEATHER_CONTROL_H
#define WEATHER_CONTROL_H

#include <vector>
#include <Preferences.h>
#include <components/TMC2209.h>
#include <components/Encoder.h>
#include <components/TempSensor.h>
#include <api/WeatherAPI.h>

class WeatherControl {
public:
    static WeatherControl& getInstance() {
        static WeatherControl instance;
        return instance;
    }

    void handleOpenClose(float prefTemp) {
        // quickly grab vars
        bool windowState = loadWindowState();
        float rotations = loadRotations();
        float curInsideTemp = TempSensor::getInstance().readTemperature();
        float curOutsideTemp = WeatherAPI::getInstance().getTemperature();

        // case 0: raining (CLOSE WINDOW & RETURN)
        if(WeatherAPI::getInstance().isRaining()) {
            // Check if the window is closed
            closeWindow();
            return;
        }
        // case 1: indoor temp is to low
        if (curInsideTemp < prefTemp) {
            // open window if outside is hotter than indoor temp
            if(curOutsideTemp > curInsideTemp) {
                openWindow();
                return;
            }
            // close window if outside is colder than inside
            if(curOutsideTemp < curInsideTemp) {
                closeWindow();
                return;
            }
        }
        // case 2: indoor temp is to high
        if (curInsideTemp > prefTemp) {
            // open window if outsider is cooler
            if(curOutsideTemp < curInsideTemp){
                openWindow();
            }
            // close window if outside is hotter
            if(curOutsideTemp > curInsideTemp){
                closeWindow();
            }
        }
    }
    
    bool closeWindow() {
        bool windowState = loadWindowState();
        float rotations = loadRotations();

        // Nothing to do
        if (!windowState || rotations <= 0.0f) {
            return false;
        }

        // Close the window
        TMC2209::getInstance().startRotation(rotations, 12);
        while (TMC2209::getInstance().rotating) {
            TMC2209::getInstance().updateRotation();
        }

        // Save state as closed
        saveWindowState(false);
        return true;
    }

    bool openWindow() {
        bool windowState = loadWindowState();
        float rotations = loadRotations();

        // Nothing to do
        if (windowState || rotations <= 0.0f) {
            return false;
        }

        // Open the window
        TMC2209::getInstance().startRotation(-rotations, 12);
        while (TMC2209::getInstance().rotating) {
            TMC2209::getInstance().updateRotation();
        }

        // Save state as open
        saveWindowState(true);
        return true;
    }


    // persistent data

    void saveRotations(float rots) {
        prefs.begin("calibration", false);
        prefs.putFloat("rotations", rots);
        prefs.end();
    }

    float loadRotations() {
        prefs.begin("calibration", true);
        float rots = prefs.getFloat("rotations", 0);
        prefs.end();
        return rots;
    }

    void saveWindowState(bool open) {
        prefs.begin("calibration", false);
        prefs.putBool("windowOpen", open);
        prefs.end();
    }

    bool loadWindowState() {
        prefs.begin("calibration", true);
        bool open = prefs.getBool("windowOpen", false);
        prefs.end();
        return open;
    }

private:
    WeatherControl() {}
    WeatherControl(const WeatherControl&) = delete;
    void operator=(const WeatherControl&) = delete;

    Preferences prefs;
};

#endif