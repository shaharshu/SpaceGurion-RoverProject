#include "RCController.h"

// Create the controller object
RCController myRemote;

void setup() {
  // Initialize USB Serial for debugging at a fast speed
  Serial.begin(115200);
  while (!Serial) { ; } // Wait for Serial monitor to open

  // Start the RC Receiver on Serial1 (Pin 19)
  myRemote.begin(Serial1);

  Serial.println("--- FlySky i-BUS Test Initialized ---");
}

void loop() {
  // 1. Refresh data from the receiver
  myRemote.update();

  // 2. Read the 4 main channels you defined
  int steer  = myRemote.readChannel(0); // Right Stick X
  int drive  = myRemote.readChannel(1); // Right Stick Y
  int tilt   = myRemote.readChannel(2); // Left Stick Y
  int pan    = myRemote.readChannel(3); // Left Stick X

  // 3. Print the values to the Serial Monitor every 100ms
  static unsigned long lastPrint = 0;
  if (millis() - lastPrint > 100) {
    Serial.print("Drive: "); Serial.print(drive);
    Serial.print(" | Steer: "); Serial.print(steer);
    Serial.print(" | Tilt: ");  Serial.print(tilt);
    Serial.print(" | Pan: ");   Serial.println(pan);
    
    lastPrint = millis();
  }

  // 4. Connection Check: If throttle is 0 or 1000 without movement, signal might be lost
  if (tilt < 500) {
    Serial.println("WARNING: Receiver disconnected or no signal!");
  }
}