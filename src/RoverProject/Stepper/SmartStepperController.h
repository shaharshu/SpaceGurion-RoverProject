#pragma once
#include "StepperController.h"

class SmartStepperController : public StepperController
{
    private:
        int _step_pin;
        int _dir_pin;
    
    protected:
        virtual void doStep() override;

    public:
        SmartStepperController(int step_pin, int dir_pin, int step_per_revolution, int current_step=0, int home=0);
        virtual void SetSpeed(float rpm) override;
        virtual void MoveToStep(int step) override;
};
