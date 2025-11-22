#include "WifiPortal.h"

void WifiPortal::setup(const char* apName, const char* apPassword) {
    portalName = apName;
    portalPassword = apPassword;
    // Start in station mode
    WiFi.mode(WIFI_STA); 
}

void WifiPortal::start() {
    Serial.println("Initializing Wi-Fi...");

    // Attempt to connect using saved credentials first
    bool connected = (portalPassword) ?
        wm.autoConnect(portalName, portalPassword) :
        wm.autoConnect(portalName);

    if (!connected) {
        Serial.println("Failed to connect or timed out, starting config portal...");
        connected = (portalPassword) ?
            wm.startConfigPortal(portalName, portalPassword) :
            wm.startConfigPortal(portalName);

        if (!connected) {
            Serial.println("User did not provide Wi-Fi credentials. Staying offline.");
            return;
        }
    }

    // At this point we are connected
    Serial.println("Connected to Wi-Fi!");
    Serial.print("SSID: ");
    Serial.println(WiFi.SSID());
    Serial.print("IP: ");
    Serial.println(WiFi.localIP());
}