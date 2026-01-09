#include "StepperController.h"

StepperController::StepperController(int step_per_revolution, int current_step = 0, int home = 0)
{
    this->_current_step = current_step;
    this->_steps_per_revolution = step_per_revolution;
    this->_home_step = home;
    this->_target_position = 0;
}

int StepperController::degreeToStep(float degree)
{
    return (degree / 360.0) * this->_steps_per_revolution;
}

float StepperController::stepsToDegree(int steps)
{
    return (steps / this->_steps_per_revolution) * 360.0;
}

float StepperController::CurrentAngle()
{
    return this->stepsToDegree(this->_current_step - this->_home_step);
}

int StepperController::CurrentStep()
{
    return this->_current_step - this->_home_step;
}

void StepperController::MoveDegrees(float degrees)
{
    this->MoveSteps(this->degreeToStep(degrees));
}

void StepperController::SetHome()
{
    this -> _home_step = this->_current_step;
}

void StepperController::MoveToDegree(float degree)
{
    if(degree == this->CurrentAngle())
        return;
    
    this->MoveToStep(this->degreeToStep(degree));
}


void StepperController::MoveSteps(int steps)
{
    this->MoveToStep(this->_current_step + steps);
}

void StepperController::MoveToStep(int step)
{
    if(this->_target_position == step)
        return;
            
    this->_target_position = step;
}

void StepperController::Run()
{
    if (this->_target_position == this->_current_step)
        return;
    
    if (this->_target_position > this->_current_step)
        this->_current_step++; 
    else
        this->_current_step--;

    this->doStep();
}

void StepperController::Stop()
{
    this->_target_position = this->CurrentStep();
}

bool StepperController::GotToDestination()
{
    return this->CurrentStep() == this->_target_position;
}

void StepperController::RunBlocking()
{
    while (!this->GotToDestination())
    this->Run();
}
