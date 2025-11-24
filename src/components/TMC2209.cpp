#include "components/TMC2209.h"

HardwareSerial TMCSerial(2);
/**
 * @brief Initialize the TMC2209 stepper driver (UART mode + pin setup).
 *
 * - Configures EN, STEP, and DIR pins.
 * - Starts UART on Serial2 for communication.
 * - Creates and configures the TMC2209Stepper instance.
 * - Sets current, microstepping, and driver behavior.
 * - Tests the UART connection and updates @ref tmcConnected.
 *
 * @return void
 */
void TMC2209::setupTMC2209() {
    // Initialize Pins
    pinMode(TMC_EN_PIN, OUTPUT);
    pinMode(TMC_STP_PIN, OUTPUT);
    pinMode(TMC_DIR_PIN, OUTPUT);
    enableMotor(false);

    TMCSerial.begin(115200, SERIAL_8N1, TMC_UART_PIN, TMC_UART_PIN); 
    pinMode(TMC_UART_PIN, INPUT_PULLUP);

    driver = new TMC2209Stepper(&TMCSerial, DRIVER_ADDRESS, R_SENSE);
    driver->begin();
    driver->toff(5);
    driver->rms_current(RMS_CURRENT);
    driver->microsteps(microsteps);
    driver->pdn_disable(true);
    driver->I_scale_analog(false);

    tmcConnected = false; 
    uint32_t drvStatus = driver->DRV_STATUS(); 
    if (drvStatus != 0xFFFFFFFF) tmcConnected = true; 

    // Optional: print registers to confirm UART
    Serial.println("TMC2209 setup complete:");
    Serial.print("GCONF: 0x"); Serial.println(driver->GCONF(), HEX);
    Serial.print("CHOPCONF: 0x"); Serial.println(driver->CHOPCONF(), HEX);
    Serial.print("DRVSTATUS: 0x"); Serial.println(driver->DRV_STATUS(), HEX);
    Serial.print("IHOLD_IRUN: 0x"); Serial.println(driver->IHOLD_IRUN(), HEX);
    Serial.print("TCOOLTHRS: 0x"); Serial.println(driver->TCOOLTHRS(), HEX);
}

/**
 * @brief Read multiple configuration registers and return a formatted report.
 *
 * Returns:
 * - Driver connection status
 * - GCONF register
 * - CHOPCONF register
 * - DRVSTATUS register
 *
 * @return String Readable, formatted status report.
 */
String TMC2209::getTMCStatusReport() {
    if (!tmcConnected) return "🔴 TMC2209 not detected.\n";
    String s;
    s.reserve(128);
    s += "🟢 TMC2209 UART OK\n";
    s += "GCONF:     0x" + String(driver->GCONF(), HEX) + "\n";
    s += "CHOPCONF:  0x" + String(driver->CHOPCONF(), HEX) + "\n";
    s += "DRVSTATUS: 0x" + String(driver->DRV_STATUS(), HEX) + "\n";
    return s;
}

/**
 * @brief Enable or disable the stepper driver.
 *
 * @param on If true, driver is enabled (EN = LOW).  
 *           If false, driver is disabled (EN = HIGH).
 *
 * @return void
 */
void TMC2209::enableMotor(bool on) {
    digitalWrite(TMC_EN_PIN, on ? LOW : HIGH);
}

/**
 * @brief Set the microstep resolution of the stepper driver.
 *
 * Valid values depend on hardware, but the common ones are:
 *   8, 16, 32, 64
 *
 * @param ms Microstep resolution to apply.
 * @return void
 */
void TMC2209::setMicrostepping(uint16_t ms) {
    microsteps = ms;
    if (tmcConnected) driver->microsteps(ms);
}

float TMC2209::stepsToRotations(long totalSteps) {
    long stepsPerRev = lround(STEPS_PER_REVOLUTION * microsteps * GEAR_RATIO);
    return (float)totalSteps / stepsPerRev;
}

void TMC2209::startRotation(float rotations, int rpm) {
    if (rpm <= 0) return;

    dirCW = rotations >= 0;
    if (!dirCW) rotations = -rotations;

    digitalWrite(TMC_DIR_PIN, dirCW ? HIGH : LOW);

    long stepsPerRev = lround(STEPS_PER_REVOLUTION * microsteps * GEAR_RATIO);
    stepsRemaining = lround(rotations * stepsPerRev);

    float stepsPerSecond = (stepsPerRev * (rpm / 60.0f));
    stepDelayUs = lround(1000000.0f / stepsPerSecond);

    enableMotor(true);
    rotating = true;
    nextStepTime = micros();
}
void TMC2209::updateRotation() {
    if (!rotating && !calibrating) return;

    unsigned long now = micros();
    if (now >= nextStepTime) {
        digitalWrite(TMC_STP_PIN, HIGH);
        digitalWrite(TMC_STP_PIN, LOW);
        nextStepTime = now + stepDelayUs;

        if (rotating) {
            stepsRemaining--;
            if (stepsRemaining <= 0) {
                rotating = false;
                enableMotor(false);
            }
        }

        if (calibrating) {
            calibrationSteps++;
        }
    }
}


void TMC2209::startCalibrationRotation(int direction, int rpm) {
    if (direction == 0 || rpm <= 0) return;

    calibrationDir = (direction > 0) ? 1 : -1;
    digitalWrite(TMC_DIR_PIN, (calibrationDir > 0) ? HIGH : LOW);

    long stepsPerRev = lround(STEPS_PER_REVOLUTION * microsteps * GEAR_RATIO);
    float stepsPerSecond = (stepsPerRev * (rpm / 60.0f));
    stepDelayUs = lround(1000000.0f / stepsPerSecond);

    calibrationSteps = 0;
    calibrating = true;
    enableMotor(true);
    nextStepTime = micros();
}


long TMC2209::stopCalibrationRotation() {
    calibrating = false;
    rotating = false;      // ensure state machine stops
    stepsRemaining = 0;    // clear steps
    enableMotor(false);
    return calibrationSteps;
}