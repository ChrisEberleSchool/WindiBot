#ifndef CALIBRATION_SCREEN_H
#define CALIBRATION_SCREEN_H

#include <display/IScreen.h>
#include <display/DisplayManager.h>

class CalibrationScreen : public IScreen {
public:
    CalibrationScreen() = default;  // add this if missing

    void draw() override {
        UIHelper::getInstance().setFont(FontStyle::MenuBold);
        UIHelper::getInstance().drawHeader("Calibration Menu");
        UIHelper::getInstance().setFont(FontStyle::MenuSmall);
    }

private:
};

#endif