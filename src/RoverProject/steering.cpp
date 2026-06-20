#include "RCController.h"
#include "MotorController.h"
#include "ServoController.h"
#include "Steering.h"

// ---- Ackerman geometry constants ----
const float ROVER_WIDTH = 537.0; // Track Width (distance between left and right wheels)
const float distance_Lf = 302.0; // length in mm(distance between front and middle wheels)
const float distance_Lr = 278.0; // length in mm(distance between middle and rear wheels)

// ============================================================================
// Steering Class: manages speed and angle for 4 steering servos and 6 motors
// ============================================================================


// Constructor
Steering::Steering(ServoMotor* servo_array, DCmotor* motor_array)
    : servos(servo_array),
      motors(motor_array),
      forward(true)
{
}
    
// Set the target speed (0..MaxSpeed) and steering angle (MIN_ANGLE..MAX_ANGLE)
void Steering::setSpeedAndAngle(int speed, int angle) {
    // Map the commanded steering angle to an Ackermann turn radius.
    float steerDeg = fabs((float)angle);
    if (steerDeg > 80.0) steerDeg = 80.0; // avoid tan() blow-up / sign flip near 90 deg
    R = distance_Lf / tan(steerDeg * PI / 180.0);

    float minRadius = ROVER_WIDTH / 2 + 1.0;
    if (R < minRadius) R = minRadius;

    // calculate angles and speeds using the Ackerman geometry
    calculateSteeringAngles(R);
    calculateMotorSpeeds(R, speed);

           
    // steering Right
    if (angle > 10) {
        // Set all steering steering wheels to the specified angle
        // left side wheels are the oppesite direction of the right side wheels
        servos[0].moveToAngle(-angle_Fo); // front left
        servos[1].moveToAngle(angle_Fi); // front right
        servos[2].moveToAngle(-angle_Ro); // rear left
        servos[3].moveToAngle(angle_Ri); // rear right
        

        // set motors speed (left side: 0,2,4 forward; right side: 1,3,5 inverted)
        motors[0].setMove(forward,speed_front_out);   // front left  (outer)
        motors[2].setMove(forward,speed_middle_out);  // middle left (outer)
        motors[4].setMove(forward,speed_rear_out);    // rear left   (outer)
        // right side motors are the oppesite direction of the left side motors
        motors[1].setMove(!forward,speed_front_in);   // front right (inner)
        motors[3].setMove(!forward,speed_middle_in);  // middle right(inner)
        motors[5].setMove(!forward,speed_rear_in);    // rear right  (inner)
    }
    // steering Left
    else if (angle < -10) {
        // Set all steering steering wheels to the specified angle
        // left side wheels are the oppesite direction of the right side wheels
        servos[0].moveToAngle(angle_Fi); // front left
        servos[1].moveToAngle(-angle_Fo); // front right
        servos[2].moveToAngle(angle_Ri); // rear left
        servos[3].moveToAngle(-angle_Ro); // rear right

        // set motors speed (left side: 0,2,4 forward; right side: 1,3,5 inverted)
        motors[0].setMove(forward,speed_front_in);    // front left  (inner)
        motors[2].setMove(forward,speed_middle_in);   // middle left (inner)
        motors[4].setMove(forward,speed_rear_in);     // rear left   (inner)
        // right side motors are the oppesite direction of the left side motors
        motors[1].setMove(!forward,speed_front_out);  // front right (outer)
        motors[3].setMove(!forward,speed_middle_out); // middle right(outer)
        motors[5].setMove(!forward,speed_rear_out);   // rear right  (outer)
    }

    // forward motion
    else {
        // Set all steering steering wheels straight forward
        servos[0].moveToAngle(0);
        servos[1].moveToAngle(0);
        servos[2].moveToAngle(0);
        servos[3].moveToAngle(0);

        // all motors at the same speed (left side: 0,2,4 forward; right side: 1,3,5 inverted)
        motors[0].setMove(forward,speed);
        motors[2].setMove(forward,speed);
        motors[4].setMove(forward,speed);
        // right side motors are the oppesite direction of the left side motors
        motors[1].setMove(!forward,speed);
        motors[3].setMove(!forward,speed);
        motors[5].setMove(!forward,speed);
    }
}


// ================================================================
// Helper functions
// ================================================================

// calculate the angle of the steering wheels using the Ackerman geometry
void Steering::calculateSteeringAngles(float R) {
    // Inner wheels are closer to the turn center (radius R - W/2) so they must
    // steer more; outer wheels (radius R + W/2) steer less.
    angle_Fi = atan(distance_Lf / (R-ROVER_WIDTH/2)) * 180/PI; // inner front
    angle_Fo = atan(distance_Lf / (R+ROVER_WIDTH/2)) * 180/PI; // outer front
    angle_Ri = atan(distance_Lr / (R-ROVER_WIDTH/2)) * 180/PI; // inner rear
    angle_Ro = atan(distance_Lr / (R+ROVER_WIDTH/2)) * 180/PI; // outer rear
}

void Steering::calculateMotorSpeeds(float R, int speed) {
    // Each wheel's speed is proportional to its turn radius (distance from the
    // turn center) relative to R (the radius of the vehicle's centerline).
    //   inner lateral radius = R - W/2,  outer lateral radius = R + W/2
    //   front corners are offset longitudinally by Lf, rear corners by Lr,
    //   middle wheels sit on the reference axle (no longitudinal offset).
    float inner = R - ROVER_WIDTH / 2;
    float outer = R + ROVER_WIDTH / 2;

    speed_front_in  = speed * sqrt(pow(distance_Lf, 2) + pow(inner, 2)) / R;
    speed_front_out = speed * sqrt(pow(distance_Lf, 2) + pow(outer, 2)) / R;
    speed_rear_in   = speed * sqrt(pow(distance_Lr, 2) + pow(inner, 2)) / R;
    speed_rear_out  = speed * sqrt(pow(distance_Lr, 2) + pow(outer, 2)) / R;
    speed_middle_in  = speed * inner / R;
    speed_middle_out = speed * outer / R;
}






