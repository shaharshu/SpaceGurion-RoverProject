// ============================================================================
// Figure-8 steering test - runs instead of RC driving when runFigureEightTest is true
// (RoverProject.ino). The rover drives on its own: straight, a right loop, straight,
// a left loop, straight, then stops. Speed and steering change gradually so every
// transition is easy to see, and each step prints what to expect.
// There is NO remote stop in this mode: keep the area clear, or lift the wheels for a
// dry run first. Press reset or cut power to abort.
// ============================================================================

constexpr int figureEightSpeed = 120;                    // 0..MaxSpeed
constexpr int figureEightTurnAngle = 30;                 // steering command for the loops (straight deadband is +-10)
constexpr unsigned long figureEightStartDelayMs = 5000;  // time to step away after reset
constexpr unsigned long figureEightTransitionMs = 3000;  // ramp time between steps
constexpr unsigned long figureEightTurnMs = 8000;        // tune so each loop closes into a circle
constexpr unsigned long figureEightUpdateMs = 50;
constexpr unsigned long figureEightPrintMs = 1000;       // progress print interval during ramps

struct FigureEightStep {
  const char* name;
  int speed;
  int angle;
  unsigned long holdMs;
  const char* expect;
};

const FigureEightStep figureEightSteps[] = {
  {"Forward straight", figureEightSpeed, 0, 4000,
   "wheels straight, all motors equal, rover drives straight"},
  {"Right loop", figureEightSpeed, figureEightTurnAngle, figureEightTurnMs,
   "front wheels right, rear wheels left, left motors faster, rover circles right"},
  {"Forward straight (middle of the 8)", figureEightSpeed, 0, 2000,
   "wheels straighten, all motors equal, rover drives straight"},
  {"Left loop", figureEightSpeed, -figureEightTurnAngle, figureEightTurnMs,
   "front wheels left, rear wheels right, right motors faster, rover circles left"},
  {"Forward straight", figureEightSpeed, 0, 2000,
   "wheels straighten, all motors equal, rover drives straight"},
  {"Stop", 0, 0, 0,
   "rover slows down and stops, wheels straight"},
};
constexpr int figureEightStepCount = sizeof(figureEightSteps) / sizeof(figureEightSteps[0]);

int fig8Step = -1; // -1 = start delay
unsigned long fig8StepStart = 0;
unsigned long fig8LastUpdate = 0;
unsigned long fig8LastPrint = 0;
int fig8FromSpeed = 0;
int fig8FromAngle = 0;
bool fig8Holding = false;
bool fig8Done = false;

void figureEightBegin() {
  Serial.println(F("=== FIGURE-8 TEST: no remote stop - keep clear. Press reset or cut power to abort. ==="));
  Serial.print(F("Starting in "));
  Serial.print(figureEightStartDelayMs / 1000);
  Serial.println(F(" s..."));
  fig8StepStart = millis();
}

void startFigureEightStep(int index) {
  fig8Step = index;
  fig8StepStart = millis();
  fig8LastPrint = fig8StepStart;
  fig8Holding = false;

  const FigureEightStep& step = figureEightSteps[index];
  Serial.print(F("[8-TEST "));
  Serial.print(index + 1);
  Serial.print('/');
  Serial.print(figureEightStepCount);
  Serial.print(F("] "));
  Serial.print(step.name);
  Serial.print(F(": speed "));
  Serial.print(fig8FromSpeed);
  Serial.print(F(" -> "));
  Serial.print(step.speed);
  Serial.print(F(", angle "));
  Serial.print(fig8FromAngle);
  Serial.print(F(" -> "));
  Serial.print(step.angle);
  Serial.print(F(" over "));
  Serial.print(figureEightTransitionMs / 1000);
  Serial.println(F(" s"));
  Serial.print(F("  expect: "));
  Serial.println(step.expect);
}

void figureEightUpdate() {
  if (fig8Done) return;
  const unsigned long now = millis();

  if (fig8Step < 0) {
    if (now - fig8StepStart >= figureEightStartDelayMs) startFigureEightStep(0);
    return;
  }
  if (now - fig8LastUpdate < figureEightUpdateMs) return;
  fig8LastUpdate = now;

  const FigureEightStep& step = figureEightSteps[fig8Step];
  const unsigned long elapsed = now - fig8StepStart;

  if (elapsed < figureEightTransitionMs) {
    // ramp linearly from the previous command to this step's command
    const int speed = fig8FromSpeed + (long)(step.speed - fig8FromSpeed) * (long)elapsed / (long)figureEightTransitionMs;
    const int angle = fig8FromAngle + (long)(step.angle - fig8FromAngle) * (long)elapsed / (long)figureEightTransitionMs;
    steering.setSpeedAndAngle(speed, angle);
    if (now - fig8LastPrint >= figureEightPrintMs) {
      fig8LastPrint = now;
      Serial.print(F("  ramping: speed="));
      Serial.print(speed);
      Serial.print(F(" angle="));
      Serial.println(angle);
    }
    return;
  }

  if (!fig8Holding) {
    fig8Holding = true;
    steering.setSpeedAndAngle(step.speed, step.angle);
    Serial.print(F("  reached speed="));
    Serial.print(step.speed);
    Serial.print(F(" angle="));
    Serial.print(step.angle);
    Serial.print(F(", holding "));
    Serial.print(step.holdMs / 1000.0, 1);
    Serial.println(F(" s"));
  }
  if (elapsed - figureEightTransitionMs < step.holdMs) return;

  fig8FromSpeed = step.speed;
  fig8FromAngle = step.angle;
  if (fig8Step + 1 < figureEightStepCount) {
    startFigureEightStep(fig8Step + 1);
  } else {
    stopAllMotors();
    fig8Done = true;
    Serial.println(F("=== FIGURE-8 TEST COMPLETE: motors stopped. ==="));
  }
}
