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
    // Sweep servo from MIN_ANGLE to MAX_ANGLE
   
}