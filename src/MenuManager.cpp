#include "MenuManager.h"
#include "display/screens/MainMenuScreen.h"
#include "display/screens/SensorScreen.h"
#include "display/screens/WifiScreen.h"
#include "display/screens/MemoryScreen.h"
#include "display/screens/CalibrationScreen.h"
#include "display/screens/LocationDataScreen.h"

#include "components/TempSensor.h"
#include "WifiPortal.h"
#include "components/TMC2209.h" 
#include "api/LocationAPI.h"
#include "api/WeatherAPI.h"

MenuManager::MenuManager(Encoder& enc)
    : encoder(enc), currentScreen(nullptr)
{
    // Instantiate screens using singletons where needed
    mainMenu           = new MainMenuScreen();
    sensorScreen       = new SensorScreen();
    wifiScreen         = new WifiScreen();
    memoryScreen       = new MemoryScreen();
    calibrationScreen  = new CalibrationScreen();
    locationDataScreen = new LocationDataScreen();

    // Start with main menu
    showScreen(mainMenu);
}

void MenuManager::setup() {
    showScreen(mainMenu);
}

void MenuManager::update() {
    // Connect to Wifi
    if (!WifiPortal::getInstance().isConnected()) {
        wifiScreen->drawSetup();
        LocationAPI::getInstance().fetchLocation();
        WeatherAPI::getInstance().fetchWeather(LocationAPI::getInstance().getLat(),LocationAPI::getInstance().getLon());
    }
    encoder.update();
    int rotation = encoder.getRotation();
    bool buttonPressed = encoder.isButtonPressed();

    if(rotation != 0) handleRotation(rotation);
    if(buttonPressed) handleButtonPress();
}

void MenuManager::handleRotation(int rot) {
    // Only main menu scrolls
    if(currentScreen == mainMenu) {
        mainMenu->moveSelection(rot);
        showScreen(mainMenu);  // redraw with new selection
    }
}

void MenuManager::handleButtonPress() {
    if(currentScreen == mainMenu) {
        int selected = mainMenu->getSelectedIndex();
        switch(selected) {
            case 0: showScreen(sensorScreen); break;
            case 1: showScreen(wifiScreen); break;
            case 2: showScreen(calibrationScreen); break;
            case 3: showScreen(locationDataScreen); break;
            case 4: showScreen(memoryScreen); break;
        }
    } else {
        // Any screen button press returns to main menu
        showScreen(mainMenu);
    }
}

void MenuManager::showScreen(IScreen* screen) {
    currentScreen = screen;
    DisplayManager::getInstance().changeScreens(currentScreen);
}
