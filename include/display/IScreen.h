#ifndef ISCREEN_H
#define ISCREEN_H

#include <functional>

class IScreen {
public:
    virtual ~IScreen() = default;

    virtual void init() {};
    virtual void draw() = 0;
    virtual void update() {}

    // Handle Encoder Inputs
    virtual void onRotation(int rot) {}
    virtual void onButtonPress() {}

    // Screen change callback
    using ChangeScreenCallback = std::function<void(int)>;

    void setChangeCallback(ChangeScreenCallback cb) {
        changeScreen = cb;
    }
protected:
    ChangeScreenCallback changeScreen;
};

#endif
