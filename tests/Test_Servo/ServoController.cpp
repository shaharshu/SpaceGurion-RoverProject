#include <Arduino.h>
#include <ServoEasing.hpp>
#include "ServoController.h"


// Constructor
ServoMotor::ServoMotor(int id, int pin) {
    _id = id;
    _pin = pin;
    _currentAngle = 0 ; // Start at middle position
}

// Initialize the servo
void ServoMotor::init() {
    _servo.attach(_pin);
    _currentAngle = 0;
    int pulse = map(0, MIN_ANGLE, MAX_ANGLE, MIN_PULSE_WIDTH, MAX_PULSE_WIDTH);;
    _servo.writeMicroseconds(pulse);
    delay(15); // Give servo time to reach initial position
}

// Set servo to specific angle immediately
void ServoMotor::setAngle(int angle) {
    // Clamp angle to allowed range
    if (angle < MIN_ANGLE) angle = MIN_ANGLE;
    if (angle > MAX_ANGLE) angle = MAX_ANGLE;
    int pulse = map(angle, MIN_ANGLE, MAX_ANGLE, MIN_PULSE_WIDTH, MAX_PULSE_WIDTH); // Map angle to pulse width
    _servo.writeMicroseconds(pulse);
    _currentAngle = angle;
}

// Adjust the current angle by the specified delta (degrees). Clamps to MIN_ANGLE..MAX_ANGLE.
void ServoMotor::adjustAngle(int delta) {
    int newAngle = _currentAngle + delta;
    if (newAngle < MIN_ANGLE) newAngle = MIN_ANGLE;
    if (newAngle > MAX_ANGLE) newAngle = MAX_ANGLE;
    setAngle(newAngle);
}

// Get current angle
int ServoMotor::getAngle() {
    return _currentAngle;
}

void ServoMotor::moveToAngle(int targetAngle, int speedDegSec)
{
    if (targetAngle < MIN_ANGLE) targetAngle = MIN_ANGLE;
    if (targetAngle > MAX_ANGLE) targetAngle = MAX_ANGLE;

    int angleDiff = abs(targetAngle - _currentAngle);
    if (angleDiff == 0) return; // Already at target

    // If speed is zero or negative, move immediately
    if (speedDegSec <= 0) {
        setAngle(targetAngle);
        return;
    }

    int durationMs = (angleDiff * 1000) / speedDegSec; // Calculate duration based on speed
    if (durationMs < 1) durationMs = 1;
    _servo.easeTo(targetAngle, durationMs);

    _currentAngle = targetAngle;
}

// Get servo ID
int ServoMotor::getID() {
    return _id;
}

// Detach servo to save power
void ServoMotor::detach() {
    _servo.detach();
}
