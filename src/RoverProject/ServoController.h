#ifndef SERVO_MOTOR_H
#define SERVO_MOTOR_H

#include <Arduino.h>
#include "config.h"
//#include <Servo.h>

class ServoEasing;

class ServoMotor {
public:
    // Constructor: Initialize with pin number
    ServoMotor(int id, int pin);
    ~ServoMotor();

    // Initialize the servo (attach to pin, set to default position)
    void init();
    
    // Set servo to a specific angle (MIN_ANGLE..MAX_ANGLE)
    void setAngle(int angle);

    // Adjust current angle by `delta` degrees (positive or negative). Result is clamped to MIN_ANGLE..MAX_ANGLE.
    void adjustAngle(int delta);

    // Adjust current angle by `delta` degrees using a move speed (deg/sec).
    // This performs a timed transition via moveToAngle().
    void adjustAngle(int delta, int speedDegSec);
    
    // Get the current angle
    int getAngle();
    
    // Move servo smoothly to target angle at specified speed (deg/sec)
    void moveToAngle(int targetAngle, int speedDegSec=SERVO_DEFAULT_MOVE_SPEED_DEG_SEC);
    
    // Detach servo to save power
    void detach();

    int getID();

private:
    ServoEasing* _servo;           // Servo object from Servo library
    int _id;                // Unique ID for the servo
    int _pin;               // Pin number
    int _currentAngle;      // Current angle position

};

#endif // SERVO_MOTOR_H