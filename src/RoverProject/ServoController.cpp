#include "ServoController.h"
#include <ServoEasing.h>

// Constructor
ServoMotor::ServoMotor(int id, int pin) {
    _id = id;
    _pin = pin;
    _currentAngle = 0 ; // Start at middle position
    _servo = new ServoEasing();
}

ServoMotor::~ServoMotor() {
    delete _servo;
}

// Initialize the servo
void ServoMotor::init() {
    _servo->attach(_pin, MIN_PULSE_WIDTH, MAX_PULSE_WIDTH);
    _currentAngle = 0; // Start at middle position

    int pulse = map(0, MIN_ANGLE, MAX_ANGLE, MIN_PULSE_WIDTH, MAX_PULSE_WIDTH);
    _servo->writeMicroseconds(pulse); // Move to middle position
}

// Set servo to specific angle immediately
void ServoMotor::setAngle(int angle) {
    // Clamp angle to valid range to prevent unsafe pulse widths
    if (angle < MIN_ANGLE) angle = MIN_ANGLE;
    if (angle > MAX_ANGLE) angle = MAX_ANGLE;
    if (!_servo->attached()) {
        _servo->attach(_pin, MIN_PULSE_WIDTH, MAX_PULSE_WIDTH);
    }
    int pulse = map(angle, MIN_ANGLE, MAX_ANGLE, MIN_PULSE_WIDTH, MAX_PULSE_WIDTH); // Map angle to pulse width
    _servo->writeMicroseconds(pulse); 
    _currentAngle = angle;
}


void ServoMotor::moveToAngle(int targetAngle, int speedDegSec)
{
    if (targetAngle < MIN_ANGLE) targetAngle = MIN_ANGLE;
    if (targetAngle > MAX_ANGLE) targetAngle = MAX_ANGLE;

    if (!_servo->attached()) {
        _servo->attach(_pin, MIN_PULSE_WIDTH, MAX_PULSE_WIDTH);
    }

    int angleDiff = targetAngle - _currentAngle;
    if (angleDiff == 0) return; // Already at target

    // If speed is zero or negative, move immediately
    if (speedDegSec <= 0) {
        setAngle(targetAngle);
        return;
    }

    _servo->setSpeed(speedDegSec); // Set speed in degrees per second

    int servoTarget = map(targetAngle, MIN_ANGLE, MAX_ANGLE, 0, 180); // Map target angle to servo library's 0-180 domain
    _servo->startEaseTo(servoTarget);

    // Track the requested final angle in the class domain.
    _currentAngle = targetAngle;
}

// Adjust the current angle by the specified delta (degrees). Clamps to MIN_ANGLE..MAX_ANGLE.
void ServoMotor::adjustAngle(int delta) {
    int newAngle = getAngle() + delta;
    if (newAngle < MIN_ANGLE) newAngle = MIN_ANGLE;
    if (newAngle > MAX_ANGLE) newAngle = MAX_ANGLE;
    setAngle(newAngle);
}

// Adjust the current angle by the specified delta (degrees) with a speed.
// Uses moveToAngle so the transition occurs over time instead of instantly.
void ServoMotor::adjustAngle(int delta, int speedDegSec) {
    int newAngle = getAngle() + delta;
    if (newAngle < MIN_ANGLE) newAngle = MIN_ANGLE;
    if (newAngle > MAX_ANGLE) newAngle = MAX_ANGLE;
    moveToAngle(newAngle, speedDegSec);
}

// Get current angle.
int ServoMotor::getAngle() {
    return _currentAngle;
}

// Get servo ID
int ServoMotor::getID() {
    return _id;
}

// Detach servo to save power
void ServoMotor::detach() {
    _servo->detach();
}
