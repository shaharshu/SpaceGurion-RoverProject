#ifndef T_MOTORCONTROLLER_H
#define T_MOTORCONTROLLER_H

#include <Arduino.h>
// configure and defined

#define DCmotor_F_L_pin1 9 //pWM pin
#define DCmotor_F_L_pin2 10

#define DCmotor_F_R_pin1 12 //pWM pin
#define DCmotor_F_R_pin2 13

#define DCmotor_M_L_pin1 9 //pWM pin
#define DCmotor_M_L_pin2 10

#define DCmotor_M_R_pin1 9 //pWM pin
#define DCmotor_M_R_pin2 10

#define DCmotor_B_L_pin1 9 //pWM pin
#define DCmotor_B_L_pin2 10

#define DCmotor_B_R_pin1 9 //pWM pin
#define DCmotor_B_R_pin2 10

#define MaxSpeed 255
#define null 999


// Test helpers (defined in t_SexMotorController.cpp)
void setupMotors();
void testMotors();
void setMotorsSpeed();
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
