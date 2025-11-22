#include "components/Encoder.h"

void Encoder::setup() {
    pinMode(ENCODER_CLK_PIN, INPUT_PULLUP);
    pinMode(ENCODER_DT_PIN, INPUT_PULLUP);
    pinMode(ENCODER_SW_PIN, INPUT_PULLUP);

    lastCLKState = digitalRead(ENCODER_CLK_PIN);
    lastDTState = digitalRead(ENCODER_DT_PIN);
    lastEncoderTime = 0;
    lastStableTime = 0;
    lastButtonTime = 0;

    rotation = 0;
    buttonPressed = false;
}

void Encoder::update() {
    int r = checkRotation();
    if (r != 0) rotation = r;

    if (checkButtonPress()) buttonPressed = true;
}

int Encoder::getRotation() {
    int r = rotation;
    rotation = 0;  // reset after reading
    return r;
}

bool Encoder::isButtonPressed() {
    bool b = buttonPressed;
    buttonPressed = false;  // reset after reading
    return b;
}

int Encoder::checkRotation() {
    unsigned long now = millis();
    int currentCLK = digitalRead(ENCODER_CLK_PIN);
    int currentDT  = digitalRead(ENCODER_DT_PIN);

    int result = 0;

    if (currentCLK != lastCLKState && (now - lastEncoderTime) > ENCODER_DEBOUNCE_MS) {
        lastEncoderTime = now;

        if (currentCLK == LOW && (now - lastStableTime) > ENCODER_LOCKOUT_MS) {
            lastStableTime = now;
            result = (currentDT != currentCLK) ? 1 : -1;
        }
    }

    lastCLKState = currentCLK;
    lastDTState  = currentDT;

    return result;
}

bool Encoder::checkButtonPress() {
    unsigned long now = millis();
    static bool lastState = HIGH;
    bool currentState = digitalRead(ENCODER_SW_PIN);
    bool pressed = false;

    if (currentState != lastState && currentState == LOW && (now - lastButtonTime) > BUTTON_DEBOUNCE_MS) {
        lastButtonTime = now;
        pressed = true;
    }

    lastState = currentState;
    return pressed;
}
