
#include <ServoEasing.hpp>
#include "ServoController.h"
#include "MotorController.h"
#include "config.h"
#include "Steering.h"
#include "RCController.h"

// ===============================
// Setup variables and objects
// ===============================

// setup rc controller
RCController rc;
int ch0, ch1, ch2, ch3, ch6 = 0;

// setup steering servos array
ServoMotor servos[4] = {
  ServoMotor(0, SERVO_0_PIN), // Front Left
  ServoMotor(1, SERVO_1_PIN), // Front Right
  ServoMotor(2, SERVO_2_PIN), // Rear Left
  ServoMotor(3, SERVO_3_PIN), // Rear Right
};

// setup camera tilt servo
ServoMotor servoCamTilt(4, SERVO_4_PIN); // Camera Tilt

// The DC motors array (motors[6]) is defined in SixMotorController.cpp
// and shared via the `extern DCmotor motors[6];` declaration in MotorController.h.

Steering steering(servos, motors);


void setup() {
  Serial.begin(115200);
  rc.begin(Serial1);

  // setup motors, servos and camera tilt
  setupMotors();
  setupServos();
  setupCameraTilt();
  

}



void loop(){
  // update rc controller
  rc.update();
  ch0 = rc.readChannel(0,MIN_ANGLE,MAX_ANGLE); // Right Stick X - Steering angle
  ch2 = rc.readChannel(2,0,MaxSpeed);          // Left Stick Y - Throttle speed

  // add logic to handle reverse motion
  // if [reverse] then steerting.forward = false
  // else steerting.forward = true

  
}


// ------------------------------------------------------------
// Helper functions
// ------------------------------------------------------------
// setupMotors() is defined in SixMotorController.cpp.

void setupServos() {
  for (int i = 0; i < SERVO_COUNT; i++) {
    servos[i].init();
  }
}

void setupCameraTilt() {
  servoCamTilt.init();
}


/*
ServoMotor testServo(1, 9); // Servo ID 1 on pin 9

void setup() {
  Serial.begin(9600);
  testServo.init();
  delay(1000);

  Serial.println("Servo test: moving to +45 degrees");
  testServo.setAngle(45);
  delay(1000);

  Serial.println("Servo test: moving to -45 degrees");
  testServo.setAngle(-45);
  delay(1000);

  Serial.println("Servo test: returning to 0 degrees");
  testServo.setAngle(0);
}

void loop() {
  static unsigned long lastMove = 0;
  static bool toggle = false;

  if (millis() - lastMove > 3000) {
    lastMove = millis();
    toggle = !toggle;
    int target = toggle ? 45 : -45;
    Serial.print("Servo loop: moving to ");
    Serial.print(target);
    Serial.println(" degrees");
    testServo.moveToAngle(target, SERVO_DEFAULT_MOVE_SPEED_DEG_SEC);
  }
}
*/  