#ifndef UIHELPER_H
#define UIHELPER_H

#include "DisplayManager.h"
#include "FontManager.h" 

class UIHelper {
public:
    static UIHelper& getInstance() {
        static UIHelper instance;
        return instance;
    }

    // Set font through DisplayManager
    void setFont(FontStyle style) {
        DisplayManager::getInstance().setFont(getFont(style));
    }
    // Draw a vertical scrollable list
    void drawList(const char* items[], int itemCount, int selectedIndex,
                        int x, int y, int lineHeight,
                        int scrollOffset, int visibleCount);

    // Draw a message box
    void drawMessage(const char* msg, int x = 0, int y = 30);

    // Draw a header/title
    void drawHeader(const char* title);

    void render();

private:
    UIHelper() {}                          
    UIHelper(const UIHelper&) = delete;        
    void operator=(const UIHelper&) = delete; 
};

#endif
