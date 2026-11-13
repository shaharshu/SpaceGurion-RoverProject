// ============================================================================
// Bench tests - run instead of RC driving when runBenchTests is true (RoverProject.ino).
// RAISE THE ROVER so the wheels are off the ground and keep clear before powering up.
// Output: Serial Monitor at 115200. Each check prints PASS/FAIL (software state);
// the physical movement must be confirmed by eye.
//
// Phases (each can be disabled below), each runs immediate control first, then simulated remote:
//   1. DC motors - simulated throttle, driving straight
//   2. Servos    - each servo alone: limits and angle adjustments, then simulated remote
//   3. Steering  - all steering servos through Steering, motors off: left/right limits
//   4. Drive     - steering + DC motors together
// ============================================================================

constexpr bool testDcMotors  = true;
constexpr bool testEachServo = true;
constexpr bool testSteering  = true;
constexpr bool testDrive     = true;

// Limit used when testing a servo alone. Steering never commands more than ~90 deg;
// raise toward MAX_ANGLE only if the linkage can physically turn that far.
constexpr int servoTestLimit = 90;
// Commands beyond MIN_ANGLE/MAX_ANGLE to verify clamping. This drives the servo through its
// full 270 deg range, so enable only with the servo disconnected from the wheel.
constexpr bool testServoClamp = false;

constexpr int driveTestSpeed = 100;

constexpr unsigned long motorTestStepDurationMs = 3000;
constexpr unsigned long servoImmediateStepMs    = 1000;
constexpr unsigned long servoSmoothStepMs       = 2000;
constexpr unsigned long steeringStepMs          = 2000;
constexpr unsigned long driveStepMs             = 3000;

#define COUNT_OF(a) (sizeof(a) / sizeof((a)[0]))

unsigned int benchChecks = 0;
unsigned int benchFailures = 0;

void reportCheck(bool ok) {
  ++benchChecks;
  if (!ok) ++benchFailures;
  Serial.println(ok ? F(" -> PASS") : F(" -> FAIL"));
}

// ------------------------------------------------------------
// Phase 1: DC motors (simulated throttle, straight)
// ------------------------------------------------------------
const int simulatedThrottleReads[] = {1000, 1200, 1500, 1800, 2000, 1000};
const int expectedThrottleSpeeds[] = {0, 47, 127, 207, 255, 0};

unsigned long runDcMotorStep(unsigned int step) {
  if (step >= COUNT_OF(simulatedThrottleReads)) return 0;
  if (step == 0) Serial.println(F("== Phase 1: DC motors - simulated throttle, straight =="));

  const int rawThrottle = simulatedThrottleReads[step];
  const int speed = RCController::mapChannelValue(2, rawThrottle, 0, MaxSpeed);

  Serial.print(F("[DC TEST "));
  Serial.print(step + 1);
  Serial.print('/');
  Serial.print(COUNT_OF(simulatedThrottleReads));
  Serial.print(F("] simulated CH2="));
  Serial.print(rawThrottle);
  Serial.print(F(" -> speed="));
  Serial.print(speed);
  Serial.print(F("/255 expected="));
  Serial.print(expectedThrottleSpeeds[step]);
  reportCheck(speed == expectedThrottleSpeeds[step]);

  if (speed == 0) {
    stopAllMotors();
  } else {
    motors[0].setMove(true, speed);
    motors[2].setMove(true, speed);
    motors[4].setMove(true, speed);
    motors[1].setMove(false, speed);
    motors[3].setMove(false, speed);
    motors[5].setMove(false, speed);
  }
  return motorTestStepDurationMs;
}

// ------------------------------------------------------------
// Phase 2: each servo alone
// ------------------------------------------------------------
ServoMotor* const benchServos[] = {&servos[0], &servos[1], &servos[2], &servos[3], &servoCamTilt};
const char* const benchServoNames[] = {"Front Left", "Front Right", "Rear Left", "Rear Right", "Camera Tilt"};

// type: 'S' = setAngle(value), 'A' = adjustAngle(value), 'M' = adjustAngle(value, speed) (smooth)
struct ServoCommand { char type; int value; int expected; };

const ServoCommand servoImmediateSteps[] = {
  {'S', 0, 0},
  {'S', servoTestLimit, servoTestLimit},    // upper limit
  {'S', -servoTestLimit, -servoTestLimit},  // lower limit
  {'S', 0, 0},
  {'A', 15, 15},
  {'A', 15, 30},
  {'A', -45, -15},
  {'A', 15, 0},
  {'M', 30, 30},
  {'M', -30, 0},
};
const ServoCommand servoClampSteps[] = {
  {'S', MAX_ANGLE + 45, MAX_ANGLE},
  {'S', MIN_ANGLE - 45, MIN_ANGLE},
  {'S', 0, 0},
};

const int simulatedServoReads[] = {1500, 1750, 2000, 1250, 1000, 1500};
const int expectedServoAngles[] = {0, servoTestLimit / 2, servoTestLimit,
                                   servoTestLimit / 2 - servoTestLimit, -servoTestLimit, 0};

unsigned long runServoStep(unsigned int step) {
  const unsigned int baseCount = COUNT_OF(servoImmediateSteps);
  const unsigned int immediateCount = baseCount + (testServoClamp ? COUNT_OF(servoClampSteps) : 0);
  const unsigned int stepsPerServo = immediateCount + COUNT_OF(simulatedServoReads);
  const unsigned int servoIndex = step / stepsPerServo;
  if (servoIndex >= COUNT_OF(benchServos)) return 0;
  if (step == 0) Serial.println(F("== Phase 2: each servo alone =="));

  ServoMotor& servo = *benchServos[servoIndex];
  unsigned int sub = step % stepsPerServo;

  if (sub < immediateCount) {
    if (sub == 0) {
      Serial.print(F("-- "));
      Serial.print(benchServoNames[servoIndex]);
      Serial.println(F(": immediate control --"));
    }
    const ServoCommand& cmd = (sub < baseCount) ? servoImmediateSteps[sub] : servoClampSteps[sub - baseCount];
    switch (cmd.type) {
      case 'S':
        servo.setAngle(cmd.value);
        Serial.print(F("setAngle("));
        break;
      case 'A':
        servo.adjustAngle(cmd.value);
        Serial.print(F("adjustAngle("));
        break;
      default:
        servo.adjustAngle(cmd.value, SERVO_DEFAULT_MOVE_SPEED_DEG_SEC);
        Serial.print(F("adjustAngle smooth("));
        break;
    }
    Serial.print(cmd.value);
    Serial.print(F(") angle="));
    Serial.print(servo.getAngle());
    Serial.print(F(" expected="));
    Serial.print(cmd.expected);
    reportCheck(servo.getAngle() == cmd.expected);
    return servoImmediateStepMs;
  }

  sub -= immediateCount;
  if (sub == 0) {
    Serial.print(F("-- "));
    Serial.print(benchServoNames[servoIndex]);
    Serial.println(F(": simulated remote (CH0 -> moveToAngle) --"));
  }
  const int raw = simulatedServoReads[sub];
  const int angle = RCController::mapChannelValue(0, raw, -servoTestLimit, servoTestLimit);
  servo.moveToAngle(angle);
  Serial.print(F("simulated CH0="));
  Serial.print(raw);
  Serial.print(F(" -> angle="));
  Serial.print(angle);
  Serial.print(F(" expected="));
  Serial.print(expectedServoAngles[sub]);
  reportCheck(angle == expectedServoAngles[sub] && servo.getAngle() == angle);
  return servoSmoothStepMs;
}

// ------------------------------------------------------------
// Shared by phases 3 and 4: apply a steering command and check the four servo angles
// ------------------------------------------------------------
bool applyAndCheckSteering(int speed, int angle) {
  steering.setSpeedAndAngle(speed, angle);

  const int fl = servos[0].getAngle();
  const int fr = servos[1].getAngle();
  const int rl = servos[2].getAngle();
  const int rr = servos[3].getAngle();
  Serial.print(F(" | FL="));
  Serial.print(fl);
  Serial.print(F(" FR="));
  Serial.print(fr);
  Serial.print(F(" RL="));
  Serial.print(rl);
  Serial.print(F(" RR="));
  Serial.print(rr);

  bool ok;
  if (angle > 10) {
    // right turn: front wheels positive, rear negative, right side (inner) steers more
    ok = fl > 0 && fr > 0 && rl < 0 && rr < 0 && fr > fl && -rr > -rl;
    Serial.print(F(" | expect: front wheels right, rear wheels left"));
    if (speed > 0) Serial.print(F(", left motors faster"));
  } else if (angle < -10) {
    // left turn: front wheels negative, rear positive, left side (inner) steers more
    ok = fl < 0 && fr < 0 && rl > 0 && rr > 0 && -fl > -fr && rl > rr;
    Serial.print(F(" | expect: front wheels left, rear wheels right"));
    if (speed > 0) Serial.print(F(", right motors faster"));
  } else {
    ok = fl == 0 && fr == 0 && rl == 0 && rr == 0;
    Serial.print(F(" | expect: all wheels straight"));
    if (speed > 0) Serial.print(F(", all motors equal"));
  }
  return ok;
}

// ------------------------------------------------------------
// Phase 3: steering left/right limits (motors off)
// ------------------------------------------------------------
const int steeringTestAngles[] = {0, 20, 45, MAX_ANGLE, 0, -20, -45, MIN_ANGLE, 0};
const int simulatedSteeringReads[] = {1500, 1600, 1800, 2000, 1500, 1400, 1200, 1000, 1500};
const int expectedSteeringAngles[] = {0, 23, 82, 135, 0, -24, -83, -135, 0}; // for MIN_ANGLE/MAX_ANGLE = -135/135

unsigned long runSteeringStep(unsigned int step) {
  const unsigned int immediateCount = COUNT_OF(steeringTestAngles);
  if (step >= immediateCount + COUNT_OF(simulatedSteeringReads)) return 0;
  if (step == 0) Serial.println(F("== Phase 3: steering limits (motors off) =="));

  bool mappingOk = true;
  int angle;
  if (step < immediateCount) {
    if (step == 0) Serial.println(F("-- immediate control --"));
    angle = steeringTestAngles[step];
    Serial.print(F("setSpeedAndAngle(0, "));
    Serial.print(angle);
    Serial.print(')');
  } else {
    const unsigned int sub = step - immediateCount;
    if (sub == 0) Serial.println(F("-- simulated remote --"));
    const int raw = simulatedSteeringReads[sub];
    angle = RCController::mapChannelValue(0, raw, MIN_ANGLE, MAX_ANGLE);
    mappingOk = angle == expectedSteeringAngles[sub];
    Serial.print(F("simulated CH0="));
    Serial.print(raw);
    Serial.print(F(" -> angle="));
    Serial.print(angle);
    Serial.print(F(" expected="));
    Serial.print(expectedSteeringAngles[sub]);
  }
  const bool steeringOk = applyAndCheckSteering(0, angle);
  reportCheck(mappingOk && steeringOk);
  return steeringStepMs;
}

// ------------------------------------------------------------
// Phase 4: drive (steering + DC motors)
// ------------------------------------------------------------
const int driveTestCommands[][2] = {   // {speed, angle}
  {driveTestSpeed, 0},
  {driveTestSpeed, 30},
  {driveTestSpeed, MAX_ANGLE},
  {driveTestSpeed, 0},
  {driveTestSpeed, -30},
  {driveTestSpeed, MIN_ANGLE},
  {0, 0},
};
const int simulatedDriveReads[][2] = { // {CH2 throttle, CH0 steering}
  {1000, 1500},
  {1500, 1500},
  {1500, 1750},
  {1500, 2000},
  {1500, 1250},
  {1500, 1000},
  {2000, 1500},
  {1000, 1500},
};
const int expectedDriveCommands[][2] = { // {speed, angle} for MaxSpeed = 255, MIN_ANGLE/MAX_ANGLE = -135/135
  {0, 0},
  {127, 0},
  {127, 67},
  {127, 135},
  {127, -68},
  {127, -135},
  {255, 0},
  {0, 0},
};

unsigned long runDriveStep(unsigned int step) {
  const unsigned int immediateCount = COUNT_OF(driveTestCommands);
  if (step >= immediateCount + COUNT_OF(simulatedDriveReads)) return 0;
  if (step == 0) Serial.println(F("== Phase 4: drive (steering + DC motors) =="));

  bool mappingOk = true;
  int speed;
  int angle;
  if (step < immediateCount) {
    if (step == 0) Serial.println(F("-- immediate control --"));
    speed = driveTestCommands[step][0];
    angle = driveTestCommands[step][1];
    Serial.print(F("setSpeedAndAngle("));
    Serial.print(speed);
    Serial.print(F(", "));
    Serial.print(angle);
    Serial.print(')');
  } else {
    const unsigned int sub = step - immediateCount;
    if (sub == 0) Serial.println(F("-- simulated remote --"));
    speed = RCController::mapChannelValue(2, simulatedDriveReads[sub][0], 0, MaxSpeed);
    angle = RCController::mapChannelValue(0, simulatedDriveReads[sub][1], MIN_ANGLE, MAX_ANGLE);
    mappingOk = speed == expectedDriveCommands[sub][0] && angle == expectedDriveCommands[sub][1];
    Serial.print(F("simulated CH2="));
    Serial.print(simulatedDriveReads[sub][0]);
    Serial.print(F(" CH0="));
    Serial.print(simulatedDriveReads[sub][1]);
    Serial.print(F(" -> speed="));
    Serial.print(speed);
    Serial.print(F(" angle="));
    Serial.print(angle);
  }
  const bool steeringOk = applyAndCheckSteering(speed, angle);
  reportCheck(mappingOk && steeringOk);
  return driveStepMs;
}

// ------------------------------------------------------------
// Test runner (non-blocking)
// ------------------------------------------------------------
constexpr unsigned int benchPhaseCount = 4;
unsigned int benchPhase = 0;
unsigned int benchStep = 0;
unsigned long benchStepStart = 0;
unsigned long benchStepDuration = 0;
bool benchComplete = false;

// Runs one step and returns how long to wait before the next one (0 = phase finished or disabled).
unsigned long runBenchStep(unsigned int phase, unsigned int step) {
  switch (phase) {
    case 0: return testDcMotors  ? runDcMotorStep(step)  : 0;
    case 1: return testEachServo ? runServoStep(step)    : 0;
    case 2: return testSteering  ? runSteeringStep(step) : 0;
    case 3: return testDrive     ? runDriveStep(step)    : 0;
    default: return 0;
  }
}

void startBenchStep() {
  while (benchPhase < benchPhaseCount) {
    benchStepDuration = runBenchStep(benchPhase, benchStep);
    if (benchStepDuration > 0) {
      benchStepStart = millis();
      return;
    }
    // phase finished: leave the rover stopped and straight before the next one
    stopAllMotors();
    steering.setSpeedAndAngle(0, 0);
    ++benchPhase;
    benchStep = 0;
  }

  benchComplete = true;
  stopAllMotors();
  Serial.print(F("=== BENCH TESTS COMPLETE: "));
  Serial.print(benchChecks - benchFailures);
  Serial.print('/');
  Serial.print(benchChecks);
  Serial.println(F(" checks passed. Motors stopped. ==="));
}

void benchTestsBegin() {
  Serial.println(F("=== BENCH TESTS: raise the rover so the wheels are off the ground and keep clear ==="));
  benchPhase = 0;
  benchStep = 0;
  startBenchStep();
}

void benchTestsUpdate() {
  if (benchComplete || millis() - benchStepStart < benchStepDuration) return;
  ++benchStep;
  startBenchStep();
}
