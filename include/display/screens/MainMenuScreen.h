#ifndef MAIN_MENU_SCREEN_H
#define MAIN_MENU_SCREEN_H

#include <display/IScreen.h>
#include <display/DisplayManager.h>
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
        // 3 visible rows starting at scrollOffset
        UIHelper::getInstance().drawList(menuItems, itemCount, selectedIndex,
                    0,        // x
                    30,       // y start
                    14,       // line height
                    scrollOffset, 
                    3);       // visible count
    }


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

    int getSelectedIndex() const { return selectedIndex; }

private:
    const char* menuItems[10] = {nullptr};
    int itemCount = 0;
    int selectedIndex = 0;
    int scrollOffset = 0;
};

#endif
