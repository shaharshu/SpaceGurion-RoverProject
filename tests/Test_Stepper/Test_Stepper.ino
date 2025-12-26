#include <Stepper.h>

// For 28BYJ-48 with ~2048 steps per revolution (gear + half-steps)
const int STEPS_PER_REV = 2048;

// IN1, IN2, IN3, IN4 connected to pins 8, 9, 10, 11
#define IN1 8
#define IN2 9
#define IN3 10
#define IN4 11

Stepper myStepper(STEPS_PER_REV, IN1, IN3, IN2, IN4);
// Note: the order (8, 10, 9, 11) often works better for this motor.
// If yours vibrates or goes backwards, try (8, 9, 10, 11) instead.

void move_stepper_by_degree(int degrees){
  float steps = degrees / 360.0 * STEPS_PER_REV;
  myStepper.step((int)round(steps));
  delay(1000);
}


void setup() {
  // Set speed in RPM (start with something slow)
  myStepper.setSpeed(10); // 10 RPM
  Serial.begin(9600);
}

void loop() {
  Serial.println("One revolution clockwise");
  myStepper.step(STEPS_PER_REV);   // 360° one way
  delay(1000);
  
  Serial.println("Half revolution clockwise");
  myStepper.step(STEPS_PER_REV/2);   // 360° one way
  delay(1000);

  Serial.println("One revolution counterclockwise");
  myStepper.step(-STEPS_PER_REV);  // 360° the other way
  delay(1000);


  Serial.print("MY Half revolution counterclockwise, steps=");
  float degree_per_step = 360 / STEPS_PER_REV;
  Serial.println(180/360.0);
  Serial.println(180/360.0 * STEPS_PER_REV);
  float steps = - (180 / 360.0) * STEPS_PER_REV;
  Serial.print(steps);
  Serial.print(" rounded = ");
  Serial.println(round(steps));
  myStepper.step(round(steps));  // 360° the other way
  delay(2000);

  Serial.print("MY Half revolution counterclockwise -180 deg");
  move_stepper_by_degree(-180);
  //13564654654
}
