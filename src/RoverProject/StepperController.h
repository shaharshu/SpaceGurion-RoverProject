class StepperController
{
private:
    int _current_step;
    int _steps_per_revolution;
    int _home;

    int _degree_to_steps(float degree);
    float _steps_to_degree(int steps);

public:
    StepperController(int step_per_revolution, int current_step=0, int home=0);
    void MoveDegree(float degree);
    void SetHome();
    void MoveToDegree(float degree);

    virtual void Run() = 0;
    virtual void Stop() = 0;
    virtual bool GotToDestination() = 0;
    virtual float CurrentAngle();
    virtual void MoveSteps() = 0;
    virtual void MoveToStep(int step) = 0;
};
