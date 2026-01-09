#pragma once
#include "StepperController.h"

class QuadPinsStepperController : public StepperController
{
    private:
        int _pinA;
        int _pinB;
        int _pinC;
        int _pinD;
    
    protected:
        virtual void doStep() override;

    public:
        QuadPinsStepperController(int* pins, int step_per_revolution, int current_step=0, int home=0);
};
