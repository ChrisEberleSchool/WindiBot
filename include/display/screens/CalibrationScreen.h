#ifndef CALIBRATION_SCREEN_H
#define CALIBRATION_SCREEN_H

#include "components/TMC2209.h"
#include <display/IScreen.h>
#include <display/DisplayManager.h>
#include <display/UIHelper.h>
#include <WeatherControl.h>
class MenuManager;


class CalibrationScreen : public IScreen {
public:
    CalibrationScreen() : state(ASK_CONFIRM), selectedIndex(0){}
    UIHelper& ui = UIHelper::getInstance();
    
    void draw() override {
        ui.setFont(FontStyle::MenuBold);
        ui.drawHeader("Calibration Menu");
        ui.setFont(FontStyle::MenuSmall);
        
        const char* confirmItems[] = { "Yes", "No" };

        if (state == ASK_CONFIRM) {
            ui.drawMessage("Calibrate?",0,30);
            ui.drawList(confirmItems, 2, selectedIndex,0, 45, 15, 0, 2);
        }
        else if (state == CALIBRATING) {
            ui.drawMessage("Calibrating...",0,30);
            ui.drawMessage("Motor Turning CCW", 0, 45);
            ui.drawMessage("Press to stop", 0, 60);
        }
        else if (state == CLOSING) {
            ui.drawMessage("Closing Window...",0,30);
            ui.drawMessage("Motor Turning CW", 0, 45);
        }
        else if (state == CALIBRATED) {
            ui.drawMessage("Calibrating Complete!",0,30);
            ui.drawMessage("Press to return", 0, 60);
        }
    }

    void update() override {
        DisplayManager::getInstance().clearBuffer();
        draw();
        DisplayManager::getInstance().sendBuffer();
    }

    void onRotation(int rot) override {
        if (state == ASK_CONFIRM) {
            selectedIndex -= rot; 
            if (selectedIndex < 0) selectedIndex = 0;
            if (selectedIndex > 1) selectedIndex = 1;
            DisplayManager::getInstance().clearBuffer();
            draw();
            DisplayManager::getInstance().sendBuffer();
        }
    }   

    void onButtonPress() override {
        if(state == ASK_CONFIRM) {
            if(selectedIndex == 0) { // Yes
                state = CALIBRATING;
                // Draw Calibration Prompt
                DisplayManager::getInstance().clearBuffer();
                draw();
                DisplayManager::getInstance().sendBuffer();
                // calibration setup NEG OPENS WINDOW
                TMC2209::getInstance().startCalibrationRotation(-1, 12);

                // routine
                while (state == CALIBRATING) {
                    
                    // update the rotation
                    TMC2209::getInstance().updateRotation();
                    // check for encoder press
                    Encoder::getInstance().update();
                    if (Encoder::getInstance().isButtonPressed()) {
                        TMC2209::getInstance().stopCalibrationRotation();
                        state = CLOSING;
                        DisplayManager::getInstance().clearBuffer();
                        draw();
                        DisplayManager::getInstance().sendBuffer();
                        WeatherControl::getInstance().saveRotations(TMC2209::getInstance().stepsToRotations(TMC2209::getInstance().calibrationSteps));
                        TMC2209::getInstance().startRotation(WeatherControl::getInstance().loadRotations(), 12);
                        while(TMC2209::getInstance().rotating) {
                            TMC2209::getInstance().updateRotation();
                        }
                        WeatherControl::getInstance().saveWindowState(false);
                        state = CALIBRATED;
                        return;
                    }
                }
            } else {
                changeScreen(0);
            }
        } 
        else if(state == CALIBRATED) {
            state = ASK_CONFIRM;
            changeScreen(0);
        }
    }

private:
    enum ScreenState {
        ASK_CONFIRM,
        CALIBRATING,
        CLOSING,
        CALIBRATED
    };

    ScreenState state;
    // For Yes/No list
    int selectedIndex;
};

#endif