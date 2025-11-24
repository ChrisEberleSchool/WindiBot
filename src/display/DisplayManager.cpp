#include <display/DisplayManager.h>
#include <display/IScreen.h>
#include "FontManager.h"

U8G2_SH1106_128X64_NONAME_F_HW_I2C display(U8G2_R0, U8X8_PIN_NONE);

void DisplayManager::setup() {
    display.begin();
    display.clearBuffer();
    display.setFont(u8g2_font_inb16_mf);
    display.drawStr(5, 40, "Starting");
    display.sendBuffer();
}
void DisplayManager::update(bool activity) {
    if (activity) {
        if (isSleeping) wake();       
        lastActivityTime = millis();  
        isSleeping = false;
    }

    // Check if timeout reached
    if (!isSleeping && checkSleepTimeout(timeTillSleep)) {
        sleep();
        isSleeping = true;
    }
}

void DisplayManager::changeScreens(IScreen* screen) {
    clearBuffer();        
    if (screen) {
        screen->draw();
    }
    sendBuffer(); 
}

void DisplayManager::setFont(const uint8_t* font) { display.setFont(font); }
void DisplayManager::drawStr(int x, int y, const char* text) { display.drawStr(x, y, text); }
void DisplayManager::drawBox(int x, int y, int w, int h) { display.drawBox(x, y, w, h); }
void DisplayManager::drawHLine(int x, int y, int length) { display.drawHLine(x, y, length); }

void DisplayManager::clearBuffer() { display.clearBuffer(); }
void DisplayManager::sendBuffer()  { display.sendBuffer(); }

void DisplayManager::drawMessage(const char* msg) {
    display.clearBuffer();
    display.setFont(u8g2_font_ncenB14_tr);
    display.drawStr(10, 35, msg);
    display.sendBuffer();
}
void DisplayManager::sleep() {
    display.clearBuffer();        
    display.sendBuffer();
    display.setPowerSave(1);      
}
void DisplayManager::wake() {
    display.setPowerSave(0);      
    display.clearBuffer();
    display.sendBuffer();
}