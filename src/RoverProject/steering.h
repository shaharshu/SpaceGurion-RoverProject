#ifndef STEERING_H
#define STEERING_H

#include <Arduino.h>

// Forward declarations: Steering only stores pointers to these types,
// so the full definitions aren't needed in this header.
class ServoMotor;
class DCmotor;

// RC Controller mapping functions
void initController(HardwareSerial &serial);
void updateController();
bool controllerConnected();
int readControllerSpeed();
int readControllerSteeringAngle();
void getControllerSpeedAndAngle(int &speed, int &angle);

// Steering class: manages speed and angle for 4 servos and 6 motors
class Steering {
    private:
        // Pointers to external servo and motor arrays
        ServoMotor* servos;      // 4 steering servos
        DCmotor* motors;         // 6 DC motors
        
        // Current speed (0..MaxSpeed) and steering angle (MIN_ANGLE..MAX_ANGLE)
        bool forward;
        
        float R; // Radius of the turn

        float angle_Fi; // Inner front wheel angle
        float angle_Fo; // Outer front wheel angle
        float angle_Ri; // Inner rear wheel angle
        float angle_Ro; // Outer rear wheel angle

        float speed_front_in;   // Inner front corner speed (uses distance_Lf)
        float speed_front_out;  // Outer front corner speed (uses distance_Lf)
        float speed_rear_in;    // Inner rear corner speed  (uses distance_Lr)
        float speed_rear_out;   // Outer rear corner speed  (uses distance_Lr)
        float speed_middle_in;  // Inner middle speed
        float speed_middle_out; // Outer middle speed
            
    public:
        // Constructor: takes pointers to already-initialized servo and motor arrays
        Steering(ServoMotor* servo_array, DCmotor* motor_array);
        
        void setSpeedAndAngle(int speed, int angle);
    
        void calculateSteeringAngles(float R);
        void calculateMotorSpeeds(float R, int speed);
        
};

#endif // STEERING_H

