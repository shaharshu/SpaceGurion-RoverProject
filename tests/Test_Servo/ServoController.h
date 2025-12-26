#ifndef SERVO_MOTOR_H
#define SERVO_MOTOR_H

#include <Servo.h>
//#include "config.h" 


// --- Configuration Constants ---
#define SERVO_PIN 9              // Digital pin connected to the servo signal wire
#define BAUD_RATE 9600           // Serial communication speed
#define MAX_PULSE_WIDTH 2500     // Maximum pulse width (microseconds)
#define MIN_PULSE_WIDTH 500      // Minimum pulse width (microseconds)
#define MIN_ANGLE -135           // Minimum servo angle (degrees)
#define MAX_ANGLE 135            // Maximum servo angle (degrees)


class ServoMotor {
public:
    // Constructor: Initialize with pin number
    ServoMotor(int id, int pin);

    // Initialize the servo (attach to pin, set to default position)
    void init();
    
    // Set servo to a specific angle (MIN_ANGLE..MAX_ANGLE)
    void setAngle(int angle);

    // Adjust current angle by `delta` degrees (positive or negative). Result is clamped to MIN_ANGLE..MAX_ANGLE.
    void adjustAngle(int delta);
    
    // Get the current angle
    int getAngle();
    
    // Move servo smoothly to target angle at specified speed (deg/sec)
    void moveToAngle(int targetAngle, int speedDegSec);
    
    // Detach servo to save power
    void detach();

    int getID();

private:
    Servo _servo;           // Servo object from Servo library
    int _id;                // Unique ID for the servo
    int _pin;               // Pin number
    int _currentAngle;      // Current angle position

};

#endif // SERVO_MOTOR_H