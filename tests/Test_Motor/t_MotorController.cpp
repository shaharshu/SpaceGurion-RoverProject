
#include "t_MotorController.h"
/*
 this api is for dc motor control encludes:
 stop - stop the motor
 setMove (forward) - set a DC motor at full speed
 setMove (forward,speed) -set a DC motor at variable speed depends on speed parameter (max speed 255)

 Motor id a number that symbol a specefic dc motor in the structer:
 F_R  0 = O-O = 1  F_L
 M_R  2 = O+O = 3  M_L
 B_R  4 = O-O = 5  B_L

every motor has two control pins: IN1, IN2
there purpose to control the speed  and direction of the motor:
when IN1 is HIGH and IN2 is LOW the motor runs forward
when IN1 is LOW and IN2 is HIGH the motor runs reverse
when both IN1 and IN2 are LOW or HIGH the motor stops
A pwm signal on IN1 controls the speed of the motor

the motor driver experemented with driver L298N 
but in the project we will use DRV8871
 
*/
DCmotor::DCmotor(int MotorId, int IN1Pin, int IN2Pin) {
// initialize pin assignments per motor ID
    this->MotorId = MotorId;
    IN1 = IN1Pin;
    IN2 = IN2Pin;

}

void DCmotor::setup() {
// setup the motor control pins as outputs
    pinMode(IN1, OUTPUT);
    pinMode(IN2, OUTPUT);
    Serial.begin(9600);
}
// stop the motor
void DCmotor::stop() {
    digitalWrite(IN1, LOW);
    digitalWrite(IN2, LOW);
}
// a func that makes dc run at full speed (speed=255)
void DCmotor::setMove(bool forward) {
        if (forward) {
            digitalWrite(IN1, HIGH);
            digitalWrite(IN2, LOW);
        } else {
            digitalWrite(IN1, LOW);
            digitalWrite(IN2, HIGH);
        }
    }

// a func that makes dc run at variable speed
// speed range 0-255
void DCmotor::setMove(bool forward, int speed) {
        if (speed > MaxSpeed) {
            speed = MaxSpeed;
        }
        
        if (forward) {
            analogWrite(IN1, speed);
            digitalWrite(IN2, LOW);
        } else {
            analogWrite(IN1,255-speed);
            digitalWrite(IN2, HIGH);
        }
    }
    
  
  
