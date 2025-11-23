#ifndef MENU_MANAGER_H
#define MENU_MANAGER_H

#include <vector>
#include <display/IScreen.h>
#include <components/Encoder.h>

class MenuManager {
public:
    MenuManager(Encoder& enc);

    void update();

private:
    Encoder& encoder;
    // Scren
    std::vector<IScreen*> screens; 
    int currentIndex = 0;

    void handleRotation(int rot);
    void handleButtonPress();
    void showScreen(int index);
};

#endif