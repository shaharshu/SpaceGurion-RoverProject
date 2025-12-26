#include "t_MotorController.h"

// Create six DCmotor objects and store them in an array
// Each motor is initialized with its specific control pins
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
void setMotorsSpeed(int speedArray[6]) {
	for (int i = 0; i < 6; ++i) {
		int speed = speedArray[i];
		if(speed==999){
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