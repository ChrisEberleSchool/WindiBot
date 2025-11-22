#ifndef WIFI_PORTAL_H
#define WIFI_PORTAL_H

#include <WiFi.h>
#include <WiFiManager.h>
#include <Arduino.h>

class WifiPortal {
public:
    static WifiPortal& getInstance() {
        static WifiPortal instance;
        return instance;
    }

    void setup(const char* apName = "ESP32-Portal", const char* apPassword = nullptr);
    void start(); 

    bool isConnected() { return WiFi.status() == WL_CONNECTED; }
    String getSSID() { return WiFi.SSID(); }
    const char* portalName;
    const char* portalPassword;

private:
    WifiPortal() = default;
    
    WiFiManager wm;
};

#endif
