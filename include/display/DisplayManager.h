#ifndef DISPLAY_MANAGER_H
#define DISPLAY_MANAGER_H

#include <U8g2lib.h>
#include <Arduino.h>

class IScreen;


class DisplayManager {
public:
    static DisplayManager& getInstance() {
        static DisplayManager instance;
        return instance;
    }
    void setup();
    void update(bool activity);
    
    void changeScreens(IScreen* screen);

    void setFont(const uint8_t* font);
    void drawStr(int x, int y, const char* text);
    void drawBox(int x, int y, int w, int h);
    void drawHLine(int x, int y, int length);

    void sleep();
    void wake();
    void updateSleepTimer() { lastActivityTime = millis(); }
    bool checkSleepTimeout(unsigned long timeoutMs) {
        return (millis() - lastActivityTime) >= timeoutMs;
    }

    void drawMemoryInfo();

    void drawMainMenu(int menuIndex);
    void drawSensorScreen(float temperature);
    void drawLocationScreen(const char* city);
    void drawMessage(const char* msg);
private:
    DisplayManager() = default;
    unsigned long lastActivityTime = 0;
    bool isSleeping = false;
    unsigned long timeTillSleep = 15000;

    void clearBuffer();
    void sendBuffer();
};

#endif
