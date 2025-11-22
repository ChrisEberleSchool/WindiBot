#ifndef ENCODER_H
#define ENCODER_H

#include "../config/Pins.h"
#include <Arduino.h>

class Encoder {
public:
    static Encoder& getInstance() {
        static Encoder instance;
        return instance;
    }
    void setup();
    void update();             // call every loop
    int getRotation();         // returns last rotation (+1/-1/0) and resets
    bool isButtonPressed();    // returns true once per press and resets

private:
    Encoder() {}                             // private constructor
    Encoder(const Encoder&) = delete;        // disable copy
    void operator=(const Encoder&) = delete; // disable assignment

    int checkRotation();
    bool checkButtonPress();

    // hardware tracking
    int lastCLKState;
    int lastDTState;
    unsigned long lastEncoderTime;
    unsigned long lastStableTime;
    unsigned long lastButtonTime;

    // internal state
    int rotation = 0;         // accumulated rotation
    bool buttonPressed = false;


    const unsigned long ENCODER_DEBOUNCE_MS = 2;
    const unsigned long ENCODER_LOCKOUT_MS  = 50;
    const unsigned long BUTTON_DEBOUNCE_MS  = 250;
};

#endif
