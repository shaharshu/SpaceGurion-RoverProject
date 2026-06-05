#include "SmartStepperController.h"
#include <Arduino.h>

SmartStepperController::SmartStepperController(int step_pin, int direction_pin, int step_per_revolution, int current_step=0, int home=0)
    : StepperController(step_per_revolution, current_step, home)
{
    this->_step_pin = step_pin;
    this->_dir_pin = direction_pin;
    digitalWrite(this->_step_pin, LOW);
    //this->SetSpeed(100);
}

void SmartStepperController::SetSpeed(float rpm)
{
    this->_delay = (MICROSEC_IN_MIN) / (2 * rpm * this->_steps_per_revolution);
    Serial.print("SetSpeed called. RPM: ");
    Serial.print(rpm);
    Serial.print(" | Calculated _delay: ");
    Serial.println(this->_delay);
}

void SmartStepperController::doStep()
{
    digitalWrite(this->_step_pin, HIGH);
    delayMicroseconds(this->_delay);
    digitalWrite(this->_step_pin, LOW);
    delayMicroseconds(this->_delay);
}

void SmartStepperController::MoveToStep(int step)
{
    StepperController::MoveToStep(step);
    if (this->_target_position > this->_current_step)
        digitalWrite(this->_dir_pin, HIGH); 
    else
        digitalWrite(this->_dir_pin, LOW);
}
