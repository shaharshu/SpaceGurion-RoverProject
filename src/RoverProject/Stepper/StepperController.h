#pragma once

class StepperController
{
    private:
        int _current_step;
        int _steps_per_revolution;
        int _home_step;

        int degreeToStep(float degree);
        float stepsToDegree(int steps);

    protected:
        int _target_position;

        virtual void doStep() = 0;

    public:
        StepperController(int step_per_revolution, int current_step=0, int home=0);
        virtual void MoveDegrees(float degrees);
        virtual void SetHome();
        virtual void MoveToDegree(float degree);
        virtual float CurrentAngle();
        virtual int CurrentStep();
        virtual void MoveSteps(int steps);
        virtual void MoveToStep(int step);
        virtual void Run();
        virtual void Stop();
        virtual bool GotToDestination();
        virtual void RunBlocking();
};
