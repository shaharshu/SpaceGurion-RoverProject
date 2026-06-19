
#include <ServoEasing.hpp>
#include "ServoController.h"
#include "MotorController.h"

ServoMotor testServo(1, 22); // Servo ID 1 on pin 22

void setup() {
  Serial.begin(9600);
  testServo.init();
  setupMotors();
}



void loop(){
  testMotors();

  Serial.println("Servo test: moving to +45 degrees");
  testServo.moveToAngle(45, 90);
  delay(1000);

  Serial.println("Servo test: moving to -45 degrees");
  testServo.moveToAngle(-45, 90);
  delay(1000);

  Serial.println("Servo test: returning to 0 degrees");
  testServo.moveToAngle(0, 90);
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
    testServo.moveToAngle(target, 60); // Move to target angle at 60 degrees per second
  }
}
*/  