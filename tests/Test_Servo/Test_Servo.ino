#include <Servo.h>

// --- Configuration ---
const int servoPin = 9;   // Digital pin connected to the servo signal wire
const long baudRate = 9600; // Serial communication speed
const int maxSpeedDegSec = 1000; // Maximum allowed speed (e.g., 100 deg/sec)
const int minSpeedDegSec = 1;   // Minimum allowed speed (e.g., 1 deg/sec)

// --- Global Variables ---
Servo myServo;        // Create a Servo object
int targetAngle = 90; // The angle the servo should be moving toward
int currentAngle = 90; // The servo's current position

unsigned long previousMillis = 0; // Will store the last time the servo was moved
long moveIntervalMs = 10; // The calculated time interval (in milliseconds) needed per 1-degree step.

// --- Serial Input Variables ---
String serialData = ""; // A string to hold incoming serial data
bool newData = false;   // Flag to indicate a new, complete data packet has arrived

void setup() {
  myServo.attach(servoPin);    // Attaches the servo on the specified pin
  myServo.write(currentAngle); // Set the initial servo position
  Serial.begin(baudRate);
  Serial.println("--- Servo Speed (deg/sec) & Position Control ---");
  Serial.println("Send data as: <angle>,<speed_deg_per_sec>");
  Serial.println("Example: 180,50 (Angle 180 degrees, Speed 50 deg/sec)");
}

void loop() {
  // 1. Read Serial Data
  readSerialData();

  // 2. Process New Data (Angle and Speed)
  if (newData) {
    processData();
    newData = false; // Reset the flag
  }

  // 3. Non-Blocking Servo Movement
  moveServo();
}

/**
 * @brief Reads incoming data from the serial port.
 */
void readSerialData() {
  while (Serial.available() > 0) {
    char inChar = Serial.read();
    
    if (inChar == '\n') {
      newData = true;
      break;
    }
    
    if (inChar != '\r') {
      serialData += inChar;
    }
  }
}

/**
 * @brief Parses the serial string to extract angle and speed (deg/sec).
 */
void processData() {
  int commaIndex = serialData.indexOf(',');

  if (commaIndex != -1) {
    String angleString = serialData.substring(0, commaIndex);
    String speedString = serialData.substring(commaIndex + 1);

    int newAngle = angleString.toInt();
    int newSpeedDegSec = speedString.toInt(); // Speed is now in deg/sec

    // --- 1. Validate and Update Angle ---
    if (newAngle >= 0 && newAngle <= 180) {
      targetAngle = newAngle;
      Serial.print("New Target Angle set to: ");
      Serial.println(targetAngle);
    } else {
      Serial.print("Invalid Angle (0-180): ");
      Serial.println(newAngle);
    }
    
    // --- 2. Validate, Convert, and Update Speed ---
    if (newSpeedDegSec >= minSpeedDegSec && newSpeedDegSec <= maxSpeedDegSec) {
      
      // Calculate the required time interval (ms) for a 1-degree step.
      // Interval (ms/deg) = 1000 / Speed (deg/s)
      moveIntervalMs = 1000L / newSpeedDegSec;
      
      Serial.print("New Speed set to: ");
      Serial.print(newSpeedDegSec);
      Serial.print(" deg/sec. (Interval: ");
      Serial.print(moveIntervalMs);
      Serial.println(" ms/degree)");
    } else {
      Serial.print("Invalid Speed (");
      Serial.print(minSpeedDegSec);
      Serial.print("-");
      Serial.print(maxSpeedDegSec);
      Serial.print(" deg/sec): ");
      Serial.println(newSpeedDegSec);
    }
    
  } else {
    Serial.println("Error: Data format incorrect. Use <angle>,<speed_deg_per_sec>");
  }

  // Clear the buffer for the next input
  serialData = "";
}

/**
 * @brief Handles the smooth, non-blocking movement of the servo motor using millis().
 */
void moveServo() {
  unsigned long currentMillis = millis();

  // Only execute the movement logic if the servo is not yet at the target
  // AND the required time (moveIntervalMs) for the next step has passed.
  if (currentAngle != targetAngle && (currentMillis - previousMillis >= moveIntervalMs)) {
    
    previousMillis = currentMillis; // Reset the timer for the next step
    
    // Determine the direction of movement
    if (currentAngle < targetAngle) {
      currentAngle++; // Move one degree closer to the target
    } else if (currentAngle > targetAngle) {
      currentAngle--; // Move one degree closer to the target
    }
    
    // Write the new position to the servo
    myServo.write(currentAngle);
  }
}