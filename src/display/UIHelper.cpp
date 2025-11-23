#include <display/UIHelper.h>
#include <Arduino.h>
#include "FontManager.h"

void UIHelper::drawList(const char* items[], int itemCount, int selectedIndex,
                        int x, int y, int lineHeight,
                        int scrollOffset, int visibleCount)
{
    const int screenHeight = 64;
    const int arrowX = 120;
    const char* arrow = "<";

    int endIndex = scrollOffset + visibleCount;
    if (endIndex > itemCount) endIndex = itemCount;

    for (int i = scrollOffset; i < endIndex; i++) {

        int row = i - scrollOffset;
        int itemY = y + (row * lineHeight);

        // Draw item text
        drawMessage(items[i], x, itemY);

        // Draw arrow next to selected
        if (i == selectedIndex) {
            drawMessage(arrow, arrowX, itemY);
        }
    }
}

void UIHelper::drawMessage(const char* msg, int x, int y) {
    DisplayManager::getInstance().drawStr(x, y, msg);
}

void UIHelper::drawHeader(const char* title) {
    DisplayManager::getInstance().drawHLine(0, 15, 128);
    drawMessage(title, 0, 12);
}

void UIHelper::render() {
    DisplayManager::getInstance().sendBuffer();
}