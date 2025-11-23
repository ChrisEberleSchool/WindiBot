#include "components/TMC2209.h"

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
    digitalWrite(TMC_EN_PIN, HIGH); 

    // Begin listening UART
    Serial2.begin(115200, SERIAL_8N1, TMC_UART_PIN, TMC_UART_PIN);

    // Populate driver reference
    driver = new TMC2209Stepper(&Serial2, R_SENSE, DRIVER_ADDRESS);
    driver->begin();
    driver->toff(5);
    driver->rms_current(RMS_CURRENT);
    driver->microsteps(microsteps);

    // Set & Test tmcConnected bool
    tmcConnected = driver->test_connection() == 0;
    if (tmcConnected) {
        Serial.println("🟢 TMC2209 UART OK");
    } else {
        Serial.println("🔴 TMC2209 UART FAIL");
    }
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
 * @brief Generates a single step pulse with a blocking microsecond delay.
 *
 * This is a direct/manual low-level step interface.
 *
 * @param us_delay Microseconds to delay between pulses.
 * @return void
 */
void TMC2209::stepPulse(int us_delay) {
    digitalWrite(TMC_STP_PIN, HIGH);
    digitalWrite(TMC_STP_PIN, LOW);
    delayMicroseconds(us_delay);
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

/**
 * @brief Rotate the stepper motor a fixed number of rotations at a set RPM.
 *
 * This function is *blocking*—it does not return until the motion completes.
 *
 * Motion behavior:
 * - Determines direction based on sign of @p rotations.
 * - Converts rotations → steps (includes microsteps + gear ratio).
 * - Computes step delay from requested RPM.
 * - Toggles DIR pin and outputs raw STEP pulses.
 *
 * @param rotations Desired shaft rotations (positive = CW, negative = CCW).
 * @param rpm       Motor speed in RPM (must be > 0).
 *
 * @return void
 */
void TMC2209::rotate(float rotations, int rpm) {
    // for whatever reason rotations are doubled so this is a quick hack fix
    rotations /= 2.0f;
    // Don't allow zero or negative RPM
    if (rpm <= 0) return; 

    // Determine direction and make rotations positive
    bool dirCW = rotations >= 0;
    if (!dirCW) rotations = -rotations;

    digitalWrite(TMC_DIR_PIN, dirCW ? HIGH : LOW);

    // Steps per one full motor revolution (with microsteps and gear)
    long stepsPerRev = lround(STEPS_PER_REVOLUTION * microsteps * GEAR_RATIO);
    long totalSteps = lround(rotations * stepsPerRev);
    
    // Steps per second to achieve requested RPM at the output shaft
    float revolutionsPerSecond = rpm / 60.0f;
    float stepsPerSecond = stepsPerRev * revolutionsPerSecond;
    int stepDelayUs = lround(1000000.0f / stepsPerSecond);
  
    enableMotor(true);
    for (long i = 0; i < totalSteps; i++) {
        stepPulse(stepDelayUs);
    }
    enableMotor(false);
}

void TMC2209::startRotation(float rotations, int rpm) {
    rotations /= 2.0f;
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

    // Steps per second based on RPM
    long stepsPerRev = lround(STEPS_PER_REVOLUTION * microsteps * GEAR_RATIO);
    float stepsPerSecond = stepsPerRev * (rpm / 60.0f);
    stepDelayUs = lround(1000000.0f / stepsPerSecond);

    calibrationSteps = 0;
    calibrating = true;
    enableMotor(true);
    nextStepTime = micros();
}


long TMC2209::stopCalibrationRotation() {
    calibrating = false;
    enableMotor(false);
    return calibrationSteps; // total steps moved during calibration
}