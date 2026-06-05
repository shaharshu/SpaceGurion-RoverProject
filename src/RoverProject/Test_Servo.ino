
#include "ServoController.h"

#define SERVO_PIN 9              // Digital pin connected to the servo signal wire
ServoMotor myServo(0, SERVO_PIN); // Create a ServoMotor object with ID 0 on defined pin

const unsigned long TEST_DELAY_MS = 2000; // ms between individual tests

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
  delay(1000); // Wait for servo to initialize
  checkEqualInt("init() sets angle to 0", 0, myServo.getAngle());

  // Test setAngle within range
  myServo.setAngle(45); 
  delay(1000); 
  checkEqualInt("setAngle(45)", 45, myServo.getAngle());

  // Test setAngle below minimum
  myServo.setAngle(-200); 
  delay(1000);
  checkEqualInt("setAngle(-200) clamps to MIN_ANGLE", MIN_ANGLE, myServo.getAngle());

  // Test setAngle above maximum
  myServo.setAngle(200);  
  delay(1000);
  checkEqualInt("setAngle(200) clamps to MAX_ANGLE", MAX_ANGLE, myServo.getAngle());

  // Test moveToAngle with speed
  myServo.moveToAngle(90, 90); // Move to 90 degrees at 90 deg/sec
  delay(1500); // Wait for movement to complete
  checkEqualInt("moveToAngle(90, 90)", 90, myServo.getAngle());
  
  myServo.moveToAngle(-90, 45); // Move to -90 degrees at 45 deg/sec
  delay(1500); // Wait for movement to complete
  checkEqualInt("moveToAngle(-90, 45)", -90, myServo.getAngle());

  // adjustAngle with speed
  myServo.adjustAngle(30, 60); // Adjust by +30 degrees at 60 deg/sec
  delay(1000); // Wait for movement to complete   
  checkEqualInt("adjustAngle(30, 60)", -60, myServo.getAngle());
  myServo.adjustAngle(-60, 120); // Adjust by -60 degrees at 120 deg/sec
  delay(1000); // Wait for movement to complete
  checkEqualInt("adjustAngle(-60, 120)", -120, myServo.getAngle());

  myServo.moveToAngle(90, 90); // Move to 90 degrees at 90 deg/sec
  delay(1500); // Wait for movement to complete
  checkEqualInt("moveToAngle(90, 90)", 90, myServo.getAngle());
  

  // test steering
  myServo.setAngle(0); // Center
  delay(1000);
  myServo.moveToAngle(45, 90); // Move to 45 degrees at 90 deg/sec
  delay(1500); // Wait for movement to complete
  myServo.moveToAngle(-45, 90); // Move to -45 degrees at 90 deg/sec
  delay(1500); // Wait for movement to complete
  myServo.moveToAngle(0, 90); // Move back to center at 90 deg/sec
  delay(1500); // Wait for movement to complete

  
  myServo.moveToAngle(90, 180); // Move to 90 degrees at 180 deg/sec
  delay(1500); // Wait for movement to complete
  myServo.moveToAngle(-90, 180); // Move to -90 degrees at 180 deg/sec
  delay(1500); // Wait for movement to complete
  myServo.moveToAngle(0, 180); // Move back to center at 180 deg/sec
  delay(1500); // Wait for movement to complete

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
    
// Nothing to do in loop
}
