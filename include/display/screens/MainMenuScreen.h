#ifndef MAIN_MENU_SCREEN_H
#define MAIN_MENU_SCREEN_H

#include <display/IScreen.h>
#include <display/UIHelper.h>
#include "FontManager.h"

class MainMenuScreen : public IScreen {
public:
    MainMenuScreen()
    {
        menuItems[0] = "Sensors";
        menuItems[1] = "Wifi";
        menuItems[2] = "Calibration";
        menuItems[3] = "Location";
        menuItems[4] = "Memory";
        itemCount = 5;
    }

    void draw() override {
        UIHelper::getInstance().setFont(FontStyle::MenuBold);
        UIHelper::getInstance().drawHeader("Main Menu");
        UIHelper::getInstance().setFont(FontStyle::MenuSmall);
        UIHelper::getInstance().drawList(menuItems,itemCount,selectedIndex,0,30,14,scrollOffset,3);
    }

    void update() override {

    }

    int getSelectedIndex() const { return selectedIndex; }

    void onRotation(int rot) override {
        moveSelection(rot);
        DisplayManager::getInstance().clearBuffer();
        draw();
        DisplayManager::getInstance().sendBuffer();
    }

    void onButtonPress() override {
        switch(selectedIndex) {
            case 0: changeScreen(1); break; // sensors
            case 1: changeScreen(2); break; // wifi
            case 2: changeScreen(3); break; // calibration
            case 3: changeScreen(4); break; // location
            case 4: changeScreen(5); break; // memory
        }
    }

private:
    const char* menuItems[10] = {nullptr};
    int itemCount = 0;
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
