#include <Servo.h>

Servo Yoni; // define a servo variable

void setup() {
  Yoni.attach(7);          // define an digital output pin to the servo variable
  yoni.write(90); // set an initial angle (optional)
}

void loop() {
  servo.write(135); // change the desierd angle
  delay(1000);
  servo.write(45);
  delay(1000);
}
