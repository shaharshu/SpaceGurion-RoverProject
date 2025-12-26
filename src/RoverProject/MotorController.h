#ifndef ROVERPROJECT_MOTORCONTROLLER_H
#define ROVERPROJECT_MOTORCONTROLLER_H

#include <Arduino.h>
// configure and defined
#define DCmotor_F_L_1 9
#define DCmotor_F_L_2 10
#define MaxSpeed 255

class DCmotor {
    private:
        int IN1;
        int IN2;
    public:
        DCmotor(int MotorId);
        void setup();
      
        // set move with one parameter - full speed
        void setMove(bool forward, int speed);

        // setMove: forward = true for forward, false for reverse
        void setMove(bool forward);

        // stop the motor
        void stop();
};

#endif
