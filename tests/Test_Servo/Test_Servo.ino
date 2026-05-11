#include <Servo.h>
#include "ServoController.h"

ServoMotor myServo(0, SERVO_PIN); // Create a ServoMotor object with ID 0 on defined pin

void setup() {

  // Start serial communication
    Serial.begin(115200);
    
    pinMode(8, OUTPUT);   // set pin 8 as output
    digitalWrite(8, HIGH); // set pin 8 high (5V on Mega/Uno)


    Serial.println("=== Servo Remote Control Test ===");
    
    // Initialize the servo
    myServo.init();
    int angle = myServo.getAngle();
    Serial.print("Servo initialized at center position at angle: "); ; Serial.println(angle);
    Serial.println();
    delay(5000);

    myServo.setAngle(0);
    angle = myServo.getAngle();
    Serial.print("Servo moves to angle: "); Serial.println(angle);
    Serial.println();
    delay(5000);

    myServo.setAngle(90);
    angle = myServo.getAngle();
    Serial.print("Servo moves to angle: "); Serial.println(angle);
    Serial.println();
    delay(5000);
    
    myServo.init();
    angle = myServo.getAngle();
    Serial.print("Servo initialized to angle: "); Serial.println(angle);
    Serial.println();
    delay(5000);


} 


void loop() {
    // Test 1: Slow incremental adjustments (simulating gentle receiver input)
    // Tests adjustAngle with small steps, like slow joystick movements
    Serial.println("TEST 1: Slow incremental adjustments (+5° steps, simulating gentle receiver input)");
    myServo.setAngle(0); // Start from center
    Serial.println("Starting from center (0°)");
    for(int i = 0; i < 10; i++) {
        myServo.adjustAngle(5); // Small positive adjustments
        Serial.print("Step "); Serial.print(i+1); Serial.print(": ");
        Serial.println(myServo.getAngle());
        delay(500); // Slow pace between adjustments
    }
    delay(5000);  // 5 second delay between tests

    // Test 2: Fast incremental adjustments (simulating quick receiver input)
    // Tests adjustAngle with larger steps, like fast joystick movements
    Serial.println("TEST 2: Fast incremental adjustments (-10° steps, simulating quick receiver input)");
    Serial.println("Starting from current position");
    for(int i = 0; i < 8; i++) {
        myServo.adjustAngle(-10); // Larger negative adjustments
        Serial.print("Step "); Serial.print(i+1); Serial.print(": ");
        Serial.println(myServo.getAngle());
        delay(200); // Faster pace between adjustments
    }
    delay(5000);  // 5 second delay between tests

    // Test 3: Smooth movement to target with slow speed
    // Tests moveToAngle with low speed (30°/sec), simulating precise positioning
    Serial.println("TEST 3: Smooth movement to 90° at slow speed (30°/sec)");
    Serial.print("Starting from: "); Serial.println(myServo.getAngle());
    myServo.moveToAngle(90, 30); // Move to 90° at 30°/second
    Serial.print("Final position: "); Serial.println(myServo.getAngle());
    delay(5000);  // 5 second delay between tests

    // Test 4: Smooth movement to target with fast speed
    // Tests moveToAngle with high speed (120°/sec), simulating quick repositioning
    Serial.println("TEST 4: Smooth movement to -45° at fast speed (120°/sec)");
    Serial.print("Starting from: "); Serial.println(myServo.getAngle());
    myServo.moveToAngle(-45, 120); // Move to -45° at 120°/second
    Serial.print("Final position: "); Serial.println(myServo.getAngle());
    delay(5000);  // 5 second delay between tests

    // Test 5: Mixed receiver-like input pattern
    // Tests combination of adjustments and movements, simulating real receiver usage
    Serial.println("TEST 5: Mixed receiver input pattern (varying adjustments and smooth moves)");
    Serial.println("Simulating: quick adjustments, then smooth positioning, then fine tuning");
    myServo.adjustAngle(20); Serial.print("Quick +20°: "); Serial.println(myServo.getAngle());
    delay(300);
    myServo.adjustAngle(-5); Serial.print("Fine -5°: "); Serial.println(myServo.getAngle());
    delay(300);
    myServo.moveToAngle(30, 60); Serial.print("Smooth to 30°: "); Serial.println(myServo.getAngle());
    delay(1000);
    myServo.adjustAngle(2); Serial.print("Micro +2°: "); Serial.println(myServo.getAngle());
    delay(5000);  // 5 second delay between tests

    Serial.println("\n=== Receiver simulation test cycle complete ===\n");
}