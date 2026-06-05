#include <Stepper.h>
//#include "../../src/RoverProject/Stepper/QuadPinsStepperController.h"
#include "SmartStepperController.h"

// For 28BYJ-48 with ~2048 steps per revolution (gear + half-steps)
const int STEPS_PER_REV = 2048;
// For enma17 SPR is 200
const int ENMA_STEPS_PER_REV = 3200;

// IN1, IN2, IN3, IN4 connected to pins 8, 9, 10, 11
#define IN1 8
#define IN2 9
#define IN3 10
#define IN4 11
#define SMART_PIN 12

#define dirPin 2
#define stepPin 3
#define motorInterfaceType 1

// Stepper myStepper(STEPS_PER_REV, IN1, IN3, IN2, IN4);
//Stepper myStepper(ENMA_STEPS_PER_REV, stepPin, dirPin);
SmartStepperController myStepper(stepPin, dirPin, ENMA_STEPS_PER_REV);
// Note: the order (8, 10, 9, 11) often works better for this motor.
// If yours vibrates or goes backwards, try (8, 9, 10, 11) instead.
//QuadPinsStepperController myStepper2([IN1, IN3, IN2, IN4], STEPS_PER_REV);
//SmartStepperController mySmartStepper(SMART_PIN, STEPS_PER_REV);

int SPR = 0;

void move_stepper_by_degree(int degrees){
  float steps = degrees / 360.0 * STEPS_PER_REV;
  //myStepper.step((int)round(steps));
  myStepper.MoveSteps((int)round(steps));
  delay(1000);
}

void setup() {
  // Set speed in RPM (start with something slow)
  Serial.begin(9600);
  myStepper.SetSpeed(100);
  pinMode(stepPin, OUTPUT);
  pinMode(dirPin, OUTPUT);

  SPR = ENMA_STEPS_PER_REV;
}

void loop() {
  Serial.println("Two revolution clockwise");
  //myStepper.step(200);   // 360° one way
  myStepper.MoveSteps(SPR*2);   // 720° one way
  myStepper.RunBlocking();
  delay(1000);
  
  Serial.println("Half revolution clockwise");
  //myStepper.step(SPR/2);   // 180 one way
  myStepper.MoveSteps(SPR/2);   // 180 one way
  myStepper.RunBlocking();
  delay(1000);

  Serial.println("One revolution counterclockwise");
  //myStepper.step(-SPR);  // 360° the other way
  myStepper.MoveSteps(-SPR);  // 360° the other way
  myStepper.RunBlocking();
  delay(1000);

  
  Serial.println("Two revolution clockwise");
  //myStepper.step(200);   // 360° one way
  myStepper.MoveDegrees(720);   // 720° one way
  myStepper.RunBlocking();
  delay(1000);
  
  Serial.println("Half revolution clockwise");
  //myStepper.step(SPR/2);   // 180 one way
  myStepper.MoveDegrees(180);   // 180 one way
  myStepper.RunBlocking();
  delay(1000);

  Serial.println("One revolution counterclockwise");
  //myStepper.step(-SPR);  // 360° the other way
  myStepper.MoveDegrees(-360);  // 360° the other way
  myStepper.RunBlocking();
  delay(1000);
}
