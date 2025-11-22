#ifndef ISCREEN_H
#define ISCREEN_H

class DisplayManager;

class IScreen {
public:
    virtual ~IScreen() = default;

    // Draw the screen using DisplayManager
    virtual void draw() = 0;

    // Optional update logic
    virtual void update() {}
};

#endif
