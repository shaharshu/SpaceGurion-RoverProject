#ifndef MOTORCONTROLLER_H
#define MOTORCONTROLLER_H

#include <Arduino.h>
#include "config.h"

// Test helpers (defined in SixMotorController.cpp)
void setupMotors();
void testMotors();
void setMotorsSpeed(int speedArray[6]);
void stopAllMotors();

extern int null_speed_vector[6]; //helper to build a vector speed values for each motor

class DCmotor {
    private:
        int IN1;
        int IN2;
        int MotorId;
    public:
        DCmotor(int MotorId, int IN1Pin, int IN2Pin);
        void setup();
        
        // set move with one parameter - full speed
        void setMove(bool forward, int speed);

        // setMove: forward = true for forward, false for reverse
        void setMove(bool forward);

        // stop the motor
        void stop();
};

#endif
