#ifndef TMC2209_H
#define TMC2209_H

#include <Arduino.h>
#include <TMCStepper.h>
#include "../config/Pins.h"
extern HardwareSerial TMCSerial;

class TMC2209 {
public:
    static TMC2209& getInstance() {
        static TMC2209 instance;
        return instance;
    }

    // public vars
    TMC2209Stepper* driver = nullptr;

    // Setup Functions
    void setupTMC2209();
    
    // Non-blocking Operation Functions
    void startRotation(float rotations, int rpm);
    void startCalibrationRotation(int direction, int rpm);
    long stopCalibrationRotation();
    void updateRotation();

    float stepsToRotations(long totalSteps);


    // GETTERS
    String getTMCStatusReport();
    // SETTERS
    void setMicrostepping(uint16_t ms);

    TMC2209(const TMC2209&) = delete;
    void operator=(const TMC2209&) = delete;

    long calibrationSteps = 0;
    // Non-blocking rotation state machine
    bool rotating = false;
    bool calibrating = false;
private:
    TMC2209() = default;   // Only singleton constructor
    ~TMC2209() = default;  // prevent external destruction
    

    // Private functions
    void enableMotor(bool on);

    // Private Const Vars
    static constexpr float R_SENSE = 0.11f;
    static constexpr uint8_t DRIVER_ADDRESS = 0b00;

    // Private Vars
    int RMS_CURRENT = 900;
    float GEAR_RATIO = 5.18; 
    int STEPS_PER_REVOLUTION = 100;
    uint16_t microsteps = 16; 

    // UART status
    bool tmcConnected = false;

    
    bool dirCW = true;
    long stepsRemaining = 0;       
    int stepDelayUs = 0;           
    unsigned long nextStepTime = 0;


    
    int calibrationDir = 1; // 1 = CW, -1 = CCW
};

#endif