#include <Servo.h>
#include "ServoController.h"

ServoMotor myServo(0, SERVO_PIN); // Create a ServoMotor object with ID 0 on defined pin

const unsigned long TEST_DELAY_MS = 500; // ms between individual tests

void checkEqualInt(const char* name, int expected, int actual) {
  Serial.print(name);
  if (expected == actual) {
    Serial.println(": PASS");
  } else {
    Serial.print(": FAIL (expected ");
    Serial.print(expected);
    Serial.print(", got ");
    Serial.print(actual);
    Serial.println(")");
  }
  Serial.println();
  delay(TEST_DELAY_MS);
}

void runServoTests() {
  Serial.println("=== Running ServoMotor Tests ===");

  // Test getID
  checkEqualInt("getID()", 0, myServo.getID());

  // Test init
  myServo.init();
  checkEqualInt("init() sets angle to 0", 0, myServo.getAngle());

  // Test setAngle normal
  myServo.setAngle(45);
  checkEqualInt("setAngle(45)", 45, myServo.getAngle());

  // Test setAngle clamps to MAX_ANGLE
  myServo.setAngle(MAX_ANGLE + 10);
  checkEqualInt("setAngle(clamp high)", MAX_ANGLE, myServo.getAngle());

  // Test setAngle clamps to MIN_ANGLE
  myServo.setAngle(MIN_ANGLE - 10);
  checkEqualInt("setAngle(clamp low)", MIN_ANGLE, myServo.getAngle());

  // Test adjustAngle within range
  myServo.setAngle(0);
  myServo.adjustAngle(30);
  checkEqualInt("adjustAngle(+30)", 30, myServo.getAngle());

  // Test adjustAngle clamps low
  myServo.adjustAngle(-1000);
  checkEqualInt("adjustAngle(clamp low)", MIN_ANGLE, myServo.getAngle());

  // Test adjustAngle clamps high
  myServo.setAngle(0);
  myServo.adjustAngle(1000);
  checkEqualInt("adjustAngle(clamp high)", MAX_ANGLE, myServo.getAngle());

  // Test moveToAngle normal (with wait for completion)
  myServo.setAngle(0);
  myServo.moveToAngle(90, 90); // 90 deg at 90 deg/sec -> ~1000ms
  delay(1200);
  checkEqualInt("moveToAngle(90,90)", 90, myServo.getAngle());

  // Test moveToAngle with zero speed (should move instantly)
  myServo.setAngle(0);
  myServo.moveToAngle(45, 0);
  delay(50);
  checkEqualInt("moveToAngle(45,0) immediate", 45, myServo.getAngle());

  // Test moveToAngle clamps target
  myServo.setAngle(0);
  myServo.moveToAngle(MAX_ANGLE + 50, 90);
  delay(1700);
  checkEqualInt("moveToAngle(clamp target)", MAX_ANGLE, myServo.getAngle());

  // moveToAngle no-op (same angle)
  myServo.setAngle(10);
  myServo.moveToAngle(10, 100);
  delay(100);
  checkEqualInt("moveToAngle(no-op)", 10, myServo.getAngle());

  // rapid consecutive moves
  myServo.setAngle(0);
  myServo.moveToAngle(30, 100);
  myServo.moveToAngle(60, 100);
  delay(800);
  checkEqualInt("rapid moves to 60", 60, myServo.getAngle());

  // very high speed
  myServo.setAngle(0);
  myServo.moveToAngle(30, 10000);
  delay(100);
  checkEqualInt("moveToAngle(30,very fast)", 30, myServo.getAngle());

  // detach preserves angle
  myServo.setAngle(20);
  myServo.detach();
  checkEqualInt("detach() preserves angle", 20, myServo.getAngle());

  // init idempotent
  myServo.init();
  myServo.init();
  checkEqualInt("double init() sets angle 0", 0, myServo.getAngle());

  Serial.println("=== ServoMotor Tests Complete ===");
}

void setup() {
  Serial.begin(115200);
  pinMode(8, OUTPUT);   // set pin 8 as output
  digitalWrite(8, HIGH); // set pin 8 high (5V on Mega/Uno)

  delay(1000);
  runServoTests();
}

void loop() {
  // No repeated actions; tests run once in setup
}
