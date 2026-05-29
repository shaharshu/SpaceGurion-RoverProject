#pragma once
#include "StepperController.h"

class SmartStepperController : public StepperController
{
    private:
        int _pinA;
        int _pinB;
        int _pinC;
        int _pinD;
    
    protected:
        virtual void doStep() override;

    public:
        SmartStepperController(int step_pin, int step_per_revolution, int current_step=0, int home=0);
};
