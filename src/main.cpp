#include <Arduino.h>
#include <display/DisplayManager.h>
#include <MenuManager.h>
#include "components/TempSensor.h"
#include "WifiPortal.h"
#include "components/Encoder.h"
#include "components/TMC2209.h"
#include "api/LocationAPI.h"
#include "api/WeatherAPI.h"
#include "display/UIHelper.h"

// Global pointer
MenuManager* menu;

void setup() 
{
    Serial.begin(115200);

    // Initialize hardware
    DisplayManager::getInstance().setup();
    Encoder::getInstance().setup();
    TempSensor::getInstance().setup();
    WifiPortal::getInstance().setup();
    WifiPortal::getInstance().start();
    LocationAPI::getInstance().fetchLocation();
    WeatherAPI::getInstance().fetchWeather(LocationAPI::getInstance().getLat(),LocationAPI::getInstance().getLon());
    TMC2209::getInstance().setupTMC2209();
    
    
    // Create MenuManager now that DisplayManager is ready
    menu = new MenuManager(Encoder::getInstance());

    menu->setup();
}

void loop() {
    menu->update();
}