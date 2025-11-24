#ifndef MEMORY_SCREEN_H
#define MEMORY_SCREEN_H

#include <display/IScreen.h>
#include <display/DisplayManager.h>
#include <Arduino.h>

class MemoryScreen : public IScreen {
public:
    MemoryScreen() = default;

    void init() override {
        needsRedraw = true;
    }
    
    void draw() override {
        UIHelper::getInstance().setFont(FontStyle::MenuBold);
        UIHelper::getInstance().drawHeader("Memory Menu");
        UIHelper::getInstance().setFont(FontStyle::MenuSmall);

        uint32_t freeHeap = ESP.getFreeHeap();
        uint32_t usedHeap = ESP.getHeapSize() - freeHeap;
        char buf[32];
        sprintf(buf, "Heap: %d / %d KB", usedHeap / 1024, ESP.getHeapSize() / 1024);
        UIHelper::getInstance().drawMessage(buf, 0, 30);

        uint32_t flashSize = ESP.getFlashChipSize();
        uint32_t sketchSize = ESP.getSketchSize();
        sprintf(buf, "Flash: %d / %d KB", sketchSize / 1024, flashSize / 1024);
        UIHelper::getInstance().drawMessage(buf, 0, 40);
    }

    void update() override {
        if (!needsRedraw) return;

        DisplayManager::getInstance().clearBuffer();
        draw();
        DisplayManager::getInstance().sendBuffer();

        needsRedraw = false;
    }

    void onRotation(int rot) override {
    }   

    void onButtonPress() override {
        changeScreen(7); 
    }
private:
    bool needsRedraw = true;

};

#endif