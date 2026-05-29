#include "QuadPinsStepperController.h"
#include <Arduino.h>

QuadPinsStepperController::QuadPinsStepperController(int* pins, int step_per_revolution, int current_step=0, int home=0)
    : StepperController(step_per_revolution, current_step, home)
{
    this->_pinA = pins[0];
    this->_pinB = pins[1];
    this->_pinC = pins[2];
    this->_pinD = pins[3];
}

void QuadPinsStepperController::doStep()
{
    //TODO: This uses arduino's digitalWrite, but should get some Board instance in DI.
    switch (this->CurrentStep() % 4) {
      case 0:  // 1010
        digitalWrite(this->_pinA, HIGH);
        digitalWrite(this->_pinB, LOW);
        digitalWrite(this->_pinC, HIGH);
        digitalWrite(this->_pinD, LOW);
      break;
      case 1:  // 0110
        digitalWrite(this->_pinA, LOW);
        digitalWrite(this->_pinB, HIGH);
        digitalWrite(this->_pinC, HIGH);
        digitalWrite(this->_pinD, LOW);
      break;
      case 2:  //0101
        digitalWrite(this->_pinA, LOW);
        digitalWrite(this->_pinB, HIGH);
        digitalWrite(this->_pinC, LOW);
        digitalWrite(this->_pinD, HIGH);
      break;
      case 3:  //1001
        digitalWrite(this->_pinA, HIGH);
        digitalWrite(this->_pinB, LOW);
        digitalWrite(this->_pinC, LOW);
        digitalWrite(this->_pinD, HIGH);
      break;
    }
}
