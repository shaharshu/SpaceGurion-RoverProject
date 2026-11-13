
#include <ServoEasing.hpp>
#include "ServoController.h"
#include "MotorController.h"
#include "config.h"
#include "Steering.h"
#include "RCController.h"

// ===============================
// Setup variables and objects
// ===============================

constexpr bool runStraightMotorTest = true;
constexpr unsigned long motorTestStepDurationMs = 3000;
const int simulatedThrottleReads[] = {1000, 1200, 1500, 1800, 2000, 1000};
const unsigned int motorTestStepCount =
    sizeof(simulatedThrottleReads) / sizeof(simulatedThrottleReads[0]);
unsigned int motorTestStep = 0;
unsigned long motorTestStart = 0;
bool motorTestComplete = false;

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

int mapSimulatedThrottleRead(int inputValue) {
  if (inputValue < controllerMinValue + noiseMargine) {
    return 0;
  }
  if (inputValue > controllerMaxValue - noiseMargine) {
    return MaxSpeed;
  }
  return map(inputValue,
             controllerMinValue + noiseMargine,
             controllerMaxValue - noiseMargine,
             0,
             MaxSpeed);
}

void runMotorTestStep(unsigned int step) {
  const int rawThrottle = simulatedThrottleReads[step];
  const int speed = mapSimulatedThrottleRead(rawThrottle);

  Serial.print(F("[DC TEST "));
  Serial.print(step + 1);
  Serial.print('/');
  Serial.print(motorTestStepCount);
  Serial.print(F("] simulated CH2="));
  Serial.print(rawThrottle);
  Serial.print(F(" -> speed="));
  Serial.print(speed);
  Serial.println(F("/255"));

  if (speed == 0) {
    stopAllMotors();
    return;
  }
  motors[0].setMove(true, speed);
  motors[2].setMove(true, speed);
  motors[4].setMove(true, speed);
  motors[1].setMove(false, speed);
  motors[3].setMove(false, speed);
  motors[5].setMove(false, speed);
}

void setup() {
  Serial.begin(115200);
  setupMotors();

  if (runStraightMotorTest) {
    Serial.println(F("DC motor test: simulated throttle steps; raise wheels and keep clear."));
    motorTestStart = millis();
    runMotorTestStep(motorTestStep);
  } else {
    rc.begin(Serial1);
    setupServos();
    setupCameraTilt();
    steering.setSpeedAndAngle(0, 0);
  }
}


void loop(){
  if (runStraightMotorTest) {
    if (!motorTestComplete &&
        millis() - motorTestStart >= motorTestStepDurationMs) {
      ++motorTestStep;
      if (motorTestStep == motorTestStepCount) {
        stopAllMotors();
        motorTestComplete = true;
        Serial.println(F("[DC TEST COMPLETE] Motors stopped."));
      } else {
        motorTestStart = millis();
        runMotorTestStep(motorTestStep);
      }
    }
    return;
  }

  rc.update();
  ch0 = rc.readChannel(0, MIN_ANGLE, MAX_ANGLE); // Right Stick X - Steering angle
  ch2 = rc.readChannel(2, 0, MaxSpeed);          // Left Stick Y - Throttle speed
  steering.setSpeedAndAngle(ch2, ch0); // Set speed and angle based on controller input

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