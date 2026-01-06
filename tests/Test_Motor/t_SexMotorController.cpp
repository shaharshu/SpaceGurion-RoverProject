#include "t_MotorController.h"
/*
 Create six DCmotor objects and store them in an array
symbolizing the six motors of the rover:

 F_L  0 = O-O = 1  F_R
 M_L  2 = O+O = 3  M_R	
 B_L  4 = O-O = 5  B_R

 Each motor is initialized with its specific control pins
  
 */

/// Helper array to hold speed values for each motor
int null_speed_vector[6] = {null,null,null,null,null,null};

DCmotor motors[6] = {

	DCmotor(0, DCmotor_F_L_pin1, DCmotor_F_L_pin2),
	DCmotor(1, DCmotor_F_R_pin1, DCmotor_F_R_pin2),
	DCmotor(2, DCmotor_M_L_pin1, DCmotor_M_L_pin2),
	DCmotor(3, DCmotor_M_R_pin1, DCmotor_M_R_pin2),
	DCmotor(4, DCmotor_B_L_pin1, DCmotor_B_L_pin2),
	DCmotor(5, DCmotor_B_R_pin1, DCmotor_B_R_pin2)

};

// Helper to initialize all motors
void setupMotors() {
	for (int i = 0; i < 6; ++i) {
		motors[i].setup();
	}
}

// Simple test routine: set each motor forward at full speed briefly
void testMotors() {
	for (int i = 0; i < 6; ++i) {
		motors[i].setMove(true, MaxSpeed);
        delay(500);
		Serial.print(i);
        Serial.println(" DC motor in work");
		motors[i].stop();
        delay(500);
	}
}
// Set speeds for all motors based on an input array
// Positive values for forward, negative for reverse
// A value of null=999 means to skip setting of the i motor
void setMotorsSpeed(int speedArray[6]) {
	for (int i = 0; i < 6; ++i) {
		int speed = speedArray[i];
		if(speed==null){
			continue;
		}

		if (speed >= 0) {
			motors[i].setMove(true, speed);
		} else {
			motors[i].setMove(false, -speed);
		}
	}
}
// Stop all motors
void stopAllMotors() {
	for (int i = 0; i < 6; ++i) {
		motors[i].stop();
	}
}