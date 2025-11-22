#ifndef MENU_MANAGER_H
#define MENU_MANAGER_H

#include <display/DisplayManager.h>
#include <display/IScreen.h>
#include <components/Encoder.h>

// Forward declare screens
class MainMenuScreen;
class SensorScreen;
class WifiScreen;
class MemoryScreen;
class CalibrationScreen;
class LocationDataScreen;

class MenuManager {
public:
    MenuManager(Encoder& enc);

    void setup();
    void update();

private:
    Encoder& encoder;
    IScreen* currentScreen;

    // Screens
    MainMenuScreen* mainMenu;
    SensorScreen* sensorScreen;
    WifiScreen* wifiScreen;
    MemoryScreen* memoryScreen;
    CalibrationScreen* calibrationScreen;
    LocationDataScreen* locationDataScreen;

    void handleRotation(int rot);
    void handleButtonPress();
    void showScreen(IScreen* screen);
};

#endif
