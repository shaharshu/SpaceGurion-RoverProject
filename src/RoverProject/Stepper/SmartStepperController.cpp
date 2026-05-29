#include "SmartStepperController.h"
#include <Arduino.h>

SmartStepperController::SmartStepperController(int step_pin, int step_per_revolution, int current_step=0, int home=0)
    : StepperController(step_per_revolution, current_step, home)
{
    this->_pin = step_pin;
    digitalWrite(this->_pin, LOW);
}

void SmartStepperController::doStep()
{
    digitalWrite(this->_pin, HIGH);
    sleep(1);
    digitalWrite(this->_pin, LOW);
}
