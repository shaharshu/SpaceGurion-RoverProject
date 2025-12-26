#include <Servo.h>
#include "ServoController.h"

/*
// --- Global Variables ---
Servo myServo;        // Create a Servo object
int targetAngle = 90; // The angle the servo should be moving toward
int currentAngle = 90; // The servo's current position

unsigned long previousMillis = 0; // Will store the last time the servo was moved
long moveIntervalMs = 10; // The calculated time interval (in milliseconds) needed per 1-degree step.

// --- Serial Input Variables ---
String serialData = ""; // A string to hold incoming serial data
bool newData = false;   // Flag to indicate a new, complete data packet has arrived
*/
ServoMotor myServo(0, SERVO_PIN); // Create a ServoMotor object with ID 0 on defined pin

void setup() {

  // Start serial communication
    Serial.begin(BAUD_RATE);

    Serial.println("=== Servo Remote Control Test ===");
    Serial.println("Simulating remote control input...");
    Serial.println();
    
    // Initialize the servo
    myServo.init();
    Serial.println("Servo initialized at center position");
    Serial.println();
    
    delay(500);

    myServo.init();
    

} 


void loop() {
  // Simple angle sweep test: step through a list of angles between -135° and 135°
  static const int testAngles[] = {-135, -90, -45, 0, 45, 90, 135};
  const int numAngles = sizeof(testAngles) / sizeof(testAngles[0]);
  static int idx = 0;


  int angle = testAngles[idx];
  myServo.setAngle(angle);

  // Print status
  Serial.print("Test Angle: ");
  Serial.print(angle);
  Serial.print("°  ->  Servo Angle: ");
  Serial.print(myServo.getAngle());
  Serial.println("°");

  delay(3000); // Hold at each angle for 1 second

  idx++;
  if (idx >= numAngles) {
    idx = 0;
    Serial.println("--- Test cycle complete, repeating ---");

    // adjustAngle tests: apply small positive then negative deltas and report
    Serial.println("--- Running adjustAngle tests ---");
    int before = myServo.getAngle();

    myServo.adjustAngle(15);
    Serial.print("adjustAngle(+15): before ");
    Serial.print(before);
    Serial.print(" -> after ");
    Serial.println(myServo.getAngle());
    delay(500);

    myServo.adjustAngle(-30);
    Serial.print("adjustAngle(-30): now ");
    Serial.println(myServo.getAngle());
    delay(500);

    Serial.println("--- adjustAngle tests complete ---");

    delay(500);
  }
}


// Simulates remote control PWM values (1000-2000 microseconds)
// This mimics typical RC receiver output with smooth sinusoidal movement
int simulateRemoteInput() {
    static unsigned long startTime = millis();
    
    unsigned long now = millis();
    float timeInSeconds = (now - startTime) / 1000.0;
    
    // Use sine wave for continuous smooth movement
    // Period of 6 seconds (full left-right-left cycle)
    float sineWave = sin(timeInSeconds * PI / 3.0); // PI/3 = 6 second period
    
    // Map sine wave (-1 to 1) to RC range (1000-2000µs)
    int remoteValue = 1500 + (int)(sineWave * 500);
    
    // Print direction indicator occasionally
    static int lastDirection = 0;
    int currentDirection = (sineWave > 0.1) ? 1 : (sineWave < -0.1) ? -1 : 0;
    
    if (currentDirection != lastDirection && currentDirection != 0) {
        Serial.println();
        if (currentDirection > 0) {
            Serial.println(">>> Remote: Sweeping RIGHT");
        } else {
            Serial.println(">>> Remote: Sweeping LEFT");
        }
        Serial.println();
        lastDirection = currentDirection;
    }
    
    return remoteValue;
}