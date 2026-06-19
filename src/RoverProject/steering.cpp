#include "RCController.h"
#include "MotorController.h"
#include "ServoController.h"
const float ROVER_W = 537.0; // width in mm(distance between left and right wheels)
const float ROVER_T = 302.0; // length in mm(distance between front and back wheels)
// Single controller object for mapping input to motion commands
static RCController rc;

// Initialize the RC receiver on the selected serial port
void initController(HardwareSerial &serial) {
    rc.begin(serial);
}

// Refresh controller values; call this in loop()
void updateController() {
    rc.update();
}

// Returns true when the RC link is active
bool controllerConnected() {
    return rc.isconnected();
}

// Map the throttle channel to motor speed (0..MaxSpeed)
int readControllerSpeed() {
    return rc.readChannel(2, 0, MaxSpeed);
}

// Map the steering channel to servo angle (MIN_ANGLE..MAX_ANGLE)
int readControllerSteeringAngle() {
    return rc.readChannel(0, MIN_ANGLE, MAX_ANGLE);
}

// Convenience helper: read both values at once
void getControllerSpeedAndAngle(int &speed, int &angle) {
    speed = readControllerSpeed();
    angle = readControllerSteeringAngle();
}

// ============================================================================
// Steering Class: manages speed and angle for 4 steering servos and 6 motors
// ============================================================================

class Steering {
private:
    // Pointers to external servo and motor arrays
    ServoMotor* servos;      // 4 steering servos
    DCmotor* motors;         // 6 drive motors
    
    // Current speed (0..MaxSpeed) and steering angle (MIN_ANGLE..MAX_ANGLE)
    int currentSpeed;
    int currentAngle;
    
public:
    // Constructor: receive pointers to already-initialized servo and motor arrays
    Steering(ServoMotor* servo_array, DCmotor* motor_array)
        : servos(servo_array),
          motors(motor_array),
          currentSpeed(0),
          currentAngle(0)
    {
    }
    
    // Set the target speed (0..MaxSpeed) and steering angle (MIN_ANGLE..MAX_ANGLE)
    void setSpeedAndAngle(int speed, int angle) {
        if (angle == 0) {
        // Set all steering steering wheels straight forward
        servos[FRONT_LEFT].moveToAngle(0, 100);
        servos[FRONT_RIGHT].moveToAngle(0, 100);
        servos[REAR_LEFT].moveToAngle(0, 100);
        servos[REAR_RIGHT].moveToAngle(0, 100);
        
        // Set all 6 drive motors to the exact uniform speed
        for (int i = 0; i < 6; i++) {
            motors[i].setSpeed(speed); 
        }
        return;
    }
      

        
        
        // Calculate individual motor speeds
        int speedFL = (int)(currentSpeed * leftMultiplier);   // Front Left
        int speedFR = (int)(currentSpeed * rightMultiplier);  // Front Right
        int speedML = (int)(currentSpeed * leftMultiplier);   // Middle Left
        int speedMR = (int)(currentSpeed * rightMultiplier);  // Middle Right
        int speedBL = (int)(currentSpeed * leftMultiplier);   // Back Left
        int speedBR = (int)(currentSpeed * rightMultiplier);  // Back Right
        
        // Apply motor speeds (forward motion)
        motors[0].setMove(true, speedFL);
        motors[1].setMove(true, speedFR);
        motors[2].setMove(true, speedML);
        motors[3].setMove(true, speedMR);
        motors[4].setMove(true, speedBL);
        motors[5].setMove(true, speedBR);
    }
    

    
    // Get current speed
    int getSpeed() const {
        return currentSpeed;
    }
    
    // Get current steering angle
    int getAngle() const {
        return currentAngle;
    }
};





