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
    delay(1000);
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

void DisplayManager::drawMainMenu(int menuIndex) {
    const char* menuItems[4] = {"Sensors", "WiFi", "Calibrate", "Memory"};
    display.clearBuffer();
    display.setFont(u8g2_font_helvB12_tr);
    display.drawStr(10, 12, "Main Menu");
    display.drawHLine(0, 15, 128);

    for (int i = 0; i < 4; i++) { // updated loop to 4
        int y = 35 + (i * 15);
        if (i == menuIndex) {
            display.setDrawColor(1);
            display.drawBox(0, y - 12, 128, 14);
            display.setDrawColor(0);
            display.setCursor(10, y);
            display.print(menuItems[i]);
            display.setDrawColor(1);
        } else {
            display.setCursor(10, y);
            display.print(menuItems[i]);
        }
    }
    display.sendBuffer();
}

void DisplayManager::setFont(const uint8_t* font) { display.setFont(font); }
void DisplayManager::drawStr(int x, int y, const char* text) { display.drawStr(x, y, text); }
void DisplayManager::drawBox(int x, int y, int w, int h) { display.drawBox(x, y, w, h); }
void DisplayManager::drawHLine(int x, int y, int length) { display.drawHLine(x, y, length); }

void DisplayManager::clearBuffer() { display.clearBuffer(); }
void DisplayManager::sendBuffer()  { display.sendBuffer(); }


void DisplayManager::drawSensorScreen(float temperature) {
    display.clearBuffer();
    display.setFont(u8g2_font_ncenB14_tr);
    display.drawStr(0, 15, "Temperature:");
    display.setFont(u8g2_font_fur20_tr);
    char tempStr[16];
    sprintf(tempStr, "%.1f C", temperature);
    display.drawStr(10, 50, tempStr);
    display.sendBuffer();
}

void DisplayManager::drawLocationScreen(const char* city) {
    display.clearBuffer();
    display.setFont(u8g2_font_ncenB14_tr);

    int y = 15;
    display.drawStr(0, y, "Loc:");
    display.setFont(u8g2_font_ncenB10_tr); // smaller font for details
    y += 20;
    display.drawStr(0, y, city);
    y += 15;
    display.sendBuffer();
}

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

void DisplayManager::drawMemoryInfo() {
    display.clearBuffer();
    display.setFont(u8g2_font_ncenB08_tr); // smaller font to fit both lines

    // --- Heap memory ---
    uint32_t freeHeap = ESP.getFreeHeap();
    uint32_t usedHeap = ESP.getHeapSize() - freeHeap;
    char buf[32];
    sprintf(buf, "Heap: %d / %d KB", usedHeap / 1024, ESP.getHeapSize() / 1024);
    drawStr(0, 20, buf);

    // --- Flash memory ---
    uint32_t flashSize = ESP.getFlashChipSize();
    uint32_t sketchSize = ESP.getSketchSize();
    sprintf(buf, "Flash: %d / %d KB", sketchSize / 1024, flashSize / 1024);
    drawStr(0, 40, buf);

    sendBuffer();
}
