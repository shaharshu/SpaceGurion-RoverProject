#include "StepperController.h"

StepperController::StepperController(int step_per_revolution, int current_step = 0, int home = 0)
{
    this->_current_step = current_step;
    this->_steps_per_revolution = step_per_revolution;
    this->_home = home;
}

int StepperController::_degree_to_steps(float degree)
{
    return (degree / 360.0) * this->_steps_per_revolution;
}

float StepperController::_steps_to_degree(int steps)
{
    return (steps / this->_steps_per_revolution) * 360.0;
}

void StepperController::MoveDegree(float degree)
{

}

void StepperController::SetHome()
{

}

void StepperController::MoveToDegree(float degree)
{
    
}
