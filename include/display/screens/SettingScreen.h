#ifndef SETTING_SCREEN_H
#define SETTING_SCREEN_H

#include "components/TMC2209.h"
#include <display/IScreen.h>
#include <display/DisplayManager.h>
#include <display/UIHelper.h>
#include <WeatherControl.h>
#include <SettingsManager.h>

class MenuManager;


class SettingScreen : public IScreen {
public:
    SettingScreen() {}
    UIHelper& ui = UIHelper::getInstance();

    void init() override {
        needsRedraw = true;
    }
    
    void draw() override {
        ui.setFont(FontStyle::MenuBold);
        ui.drawHeader("Settings");
        ui.setFont(FontStyle::MenuSmall);

        updateMenuItems();
        ui.drawList(menuItems, itemCount, selectedIndex, 0, 30, 14, scrollOffset, itemCount);
    }

    void update() override {
        if (!needsRedraw) return;

        DisplayManager::getInstance().clearBuffer();
        draw();
        DisplayManager::getInstance().sendBuffer();

        needsRedraw = false;
    }

    void updateMenuItems() {
        menuItems[0] = "Back:";
        static char buf1[20];
        static char buf2[20];
        static char buf3[20];
        static char buf4[20];

        sprintf(buf1, "Weather Guard: %s", SettingsManager::getInstance().loadWeatherGuard() ? "ON" : "OFF");
        sprintf(buf2, "Offline Mode: %s", SettingsManager::getInstance().loadOfflineMode() ? "ON" : "OFF");
        sprintf(buf3, "Units: %s", SettingsManager::getInstance().loadUnits() ? "MET" : "IMP");
        sprintf(buf4, "Memory Info:");
        

        menuItems[1] = buf1;
        menuItems[2] = buf2;
        menuItems[3] = buf3;
        menuItems[4] = buf4;
    }


    void onRotation(int rot) override {
        moveSelection(rot);
        needsRedraw = true;
    }

    

    void onButtonPress() override {
        switch (selectedIndex) {
            case 0: // Back
                changeScreen(0);
                break;
            case 1: // Weather code blocking
                SettingsManager::getInstance().saveWeatherGuard(!SettingsManager::getInstance().loadWeatherGuard());
                needsRedraw = true;
                break;
            case 2: // Offline mode
                SettingsManager::getInstance().saveOfflineMode(!SettingsManager::getInstance().loadOfflineMode());
                needsRedraw = true;
                break;
            case 3: // Units
                SettingsManager::getInstance().saveUnits(!SettingsManager::getInstance().loadUnits());
                needsRedraw = true;
                break;
            case 4: // Memory Info
                changeScreen(6);
                break;
        }
    }

private:
    Preferences prefs;

    bool needsRedraw = true;

    const char* menuItems[10] = {nullptr};
    int itemCount = 5;
    int selectedIndex = 0;
    int scrollOffset = 0;

    void moveSelection(int delta) {
        selectedIndex += delta;

        // Wrap list index
        if (selectedIndex >= itemCount) selectedIndex = 0;
        if (selectedIndex < 0) selectedIndex = itemCount - 1;

        // Visible window size
        const int visibleCount = 3;

        // Auto-scroll down
        if (selectedIndex >= scrollOffset + visibleCount) {
            scrollOffset = selectedIndex - visibleCount + 1;
        }

        // Auto-scroll up
        if (selectedIndex < scrollOffset) {
            scrollOffset = selectedIndex;
        }
    }
};

#endif