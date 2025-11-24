#include "MenuManager.h"

#include "display/screens/MainMenuScreen.h"
#include "display/screens/SensorScreen.h"
#include "display/screens/WifiScreen.h"
#include "display/screens/MemoryScreen.h"
#include "display/screens/CalibrationScreen.h"
#include "display/screens/LocationDataScreen.h"
#include "display/screens/OperationScreen.h"
#include "display/screens/SettingScreen.h"

#include "components/TempSensor.h"
#include "WifiPortal.h"
#include "components/TMC2209.h" 
#include "api/LocationAPI.h"
#include "api/WeatherAPI.h"

MenuManager::MenuManager(Encoder& enc)
    : encoder(enc)
{
    screens.push_back(new MainMenuScreen());       // index 0
    screens.push_back(new OperationScreen());      // index 1
    screens.push_back(new SensorScreen());         // index 2
    screens.push_back(new WifiScreen());           // index 3
    screens.push_back(new CalibrationScreen());    // index 4
    screens.push_back(new LocationDataScreen());   // index 5
    screens.push_back(new MemoryScreen());         // index 6
    screens.push_back(new SettingScreen());        // index 7

    // Install callbacks for screen change
    for (int i = 0; i < screens.size(); i++) {
        screens[i]->setChangeCallback(
            [this](int index) {
                this->showScreen(index);
            }
        );
    }

    showScreen(0);
}

void MenuManager::update() {
    // Connect to Wifi
    if (!WifiPortal::getInstance().isConnected()) {
        static_cast<WifiScreen*>(screens[2])->drawSetup();
        LocationAPI::getInstance().fetchLocation();
        WeatherAPI::getInstance().fetchWeather(LocationAPI::getInstance().getLat(),LocationAPI::getInstance().getLon());
    }

    encoder.update();
    int rotation = encoder.getRotation();
    bool buttonPressed = encoder.isButtonPressed();

    if(rotation != 0) handleRotation(rotation);
    if(buttonPressed) handleButtonPress();

    screens[currentIndex]->update();
}

void MenuManager::handleRotation(int rot) {
    screens[currentIndex]->onRotation(rot);
}


void MenuManager::handleButtonPress() {
    screens[currentIndex]->onButtonPress();
}

void MenuManager::showScreen(int index) {
    currentIndex = index;
    screens[currentIndex]->init();
    DisplayManager::getInstance().clearBuffer();
}