#ifndef SERVO_MOTOR_H
#define SERVO_MOTOR_H

// 1. Include the configuration to access the pin and limit definitions
#include "config.h" 
#include <Servo.h>
class ServoMotor {
public:
    // Constructor: Used to create and initialize a new servo object
    ServoMotor(int pin);

    // Public method: Used by the user to set the servo's position
    void setAngle(int angle);

    // Public method: Used to initialize the servo (e.g., attach it to the hardware)
    void begin();

private:
    // Private member: Stores the hardware pin number this servo is using
    int _pin;

    // Private member (optional): Stores the current angle
    int _currentAngle;

};

#endif // SERVO_MOTOR_H