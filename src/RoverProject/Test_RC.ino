#include "RCController.h"

// Create the controller object
const FailsefeRule myFailsafeRules[] = {
    {4, 1501},
    {5, 1500}
};
const uint8_t rulesCount = sizeof(myFailsafeRules) / sizeof(myFailsafeRules[0]);
RCController RC(myFailsafeRules, rulesCount, 6);

void setup() {
  // Initialize USB Serial for debugging at a fast speed
  Serial.begin(115200);
  RC.begin(Serial1);
  }


void loop() {
  // 1. Refresh data from the receiver
  RC.update();

  // 2. Read the 4 main channels you defined
  int ch0  = RC.readChannel(0,-80,80); // Right Stick X
  int ch1  = RC.readChannel(1,-180,180); // Right Stick Y
  int ch2 = RC.readChannel(2,0,255); // Left Stick Y
  int ch3 = RC.readChannel(3,-45,45); // Left Stick X
  int ch4 = RC.readChannel(4,0,2); // Left Dial
  int ch5 = RC.readChannel(5,0,1); // Right Dial

  // 3. Print the values to the Serial Monitor every 100ms
  static unsigned long lastPrint = 0;
  if (millis() - lastPrint > 500) {
    Serial.print("Right stick Y: ");      Serial.print(ch0);
    Serial.print(" | Right stick X: ");   Serial.println(ch1);
    Serial.print(" | Left stick Y:: ");   Serial.print(ch2);
    Serial.print(" | Left stick X: ");    Serial.println(ch3);
    Serial.print(" | Left Dial: ");       Serial.print(ch4);
    Serial.print(" | Right Dial: ");      Serial.println(ch5);
    Serial.println("--------------------------------------------------");
    Serial.print(" controller is connected:"); Serial.println(RC.isconnected()); // Ensure all data is sent before the next update
    
    lastPrint = millis();
  }


}