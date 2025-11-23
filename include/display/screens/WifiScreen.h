#ifndef WIFI_SCREEN_H
#define WIFI_SCREEN_H

#include <display/IScreen.h>
#include <display/DisplayManager.h>
#include "WifiPortal.h"
#include <display/UIHelper.h>

class WifiScreen : public IScreen {
public:
    WifiScreen() = default;
    
    void draw() override {
        UIHelper::getInstance().setFont(FontStyle::MenuBold);
        UIHelper::getInstance().drawHeader("Wifi Menu");
        UIHelper::getInstance().setFont(FontStyle::MenuMedium);
        auto& wifi = WifiPortal::getInstance();
        
        if (wifi.isConnected()) {
            UIHelper::getInstance().drawMessage("Wifi Connected:", 0, 30);
            UIHelper::getInstance().drawMessage(wifi.getSSID().c_str(), 0, 45);
        } else {
            UIHelper::getInstance().drawMessage("No Wifi connected:", 0, 45);
        }
    }

    void drawSetup() {
        auto& wifi = WifiPortal::getInstance();

        UIHelper::getInstance().setFont(FontStyle::MenuBold);
        UIHelper::getInstance().drawHeader("Wifi Setup");
        UIHelper::getInstance().setFont(FontStyle::MenuSmall);
        UIHelper::getInstance().drawMessage("Please Connect to:",0,30);
        UIHelper::getInstance().drawMessage(wifi.portalName,0,45);
    }

    void onRotation(int rot) override {
    }   

    void onButtonPress() override {
        changeScreen(0); 
    }
private:
};

#endif
