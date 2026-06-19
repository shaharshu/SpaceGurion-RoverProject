#ifndef STEERING_H
#define STEERING_H

#include <Arduino.h>

// RC Controller mapping functions
void initController(HardwareSerial &serial);
void updateController();
bool controllerConnected();
int readControllerSpeed();
int readControllerSteeringAngle();
void getControllerSpeedAndAngle(int &speed, int &angle);

// Steering class: manages speed and angle for 4 servos and 6 motors
class Steering {
public:
    // Constructor: takes pointers to already-initialized servo and motor arrays
    Steering(ServoMotor* servo_array, DCmotor* motor_array);
    
    void setSpeedAndAngle(int speed, int angle);
  
    
    int getSpeed() const;
    int getAngle() const;
};

#endif // STEERING_H

