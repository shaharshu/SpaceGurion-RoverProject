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
  int throttle   = myRemote.readChannel(2); // Left Stick Y
  int left_sides    = myRemote.readChannel(3); // Left Stick X
  int ch5 = myRemote.readChannel(4);
  int ch6 = myRemote.readChannel(5);

  // 3. Print the values to the Serial Monitor every 100ms
  static unsigned long lastPrint = 0;
  if (millis() - lastPrint > 500) {
    Serial.print("Drive: "); Serial.print(drive);
    Serial.print(" | Steer: "); Serial.print(steer);
    Serial.print(" | Throttle: ");  Serial.print(throttle);
    Serial.print(" | Left_sides: ");   Serial.println(left_sides);
    Serial.print(" | Ch5: ");   Serial.println(ch5);
    Serial.print(" | Ch6: ");   Serial.println(ch6);
    
    lastPrint = millis();
  }


}