# 🚀 Rover Project: Computer Unit Plan & Architecture (v1.1)

**Date:** November 27, 2025
**Team Focus:** Computer Unit (Hardware Integration & Software Development)
**Microcontroller:** Arduino Mega
**Steering:** Ackerman Geometry
**Goal:** Modular, Open-Source Mars Rover Replica

---


##  Software Interfaces (API Contract)

This defines the contract between the main system logic (P3) and the individual controllers (P2).

### 1. MotorController (DC Motor Controller)
**Goal:** Centralized control for all **six** DC drive motors.

| Function Name | Return Type | Parameters (Arguments) | Description |
| :--- | :--- | :--- | :--- |
| `MotorController(int numMotors)` | Constructor | `int numMotors` (e.g., 6) | Initializes the controller and sets up necessary pins defined in `config.h`. |
| `void setSpeed(int motorID, float speed)` | `void` | `int motorID` (0-5), `float speed` ($-1.0$ to $1.0$). | Sets the speed and direction of a specific motor. |
| `void stopAll()` | `void` | None | Immediately stops all 6 drive motors (applies brake/coast). |
| `void emergencyStop()` | `void` | None | A failsafe method to stop all motors safely. |

### 2. ServoController (Servo Actuator Controller)
**Goal:** Centralized control for the **four** steering servos.

| Function Name | Return Type | Parameters (Arguments) | Description |
| :--- | :--- | :--- | :--- |
| `ServoController()` | Constructor | None | Initializes the class, attaches servos to pins defined in `config.h`. |
| `void setAngle(int servoID, int angle)` | `void` | `int servoID` (0-4), `int angle` (0 to 180). | Sets the physical angle in degrees for a specific servo. |
| `void calibrateCenter(int servoID, int centerPos)` | `void` | `int servoID`, `int centerPos` (angle). | Used during setup to fine-tune the center position. |

### 3. RCController (Remote Control Input Handler)
**Goal:** Reads raw pulse data and converts it into standardized, normalized control values.

| Function Name | Return Type | Parameters (Arguments) | Description |
| :--- | :--- | :--- | :--- |
| `RCController()` | Constructor | None | Initializes the class and sets up input pins. |
| `bool readChannels()` | `bool` | None | Reads the latest raw data. Returns `true` if the signal is valid, `false` if lost (Failsafe). |
| `float getThrottle()` | `float` | None | Returns the normalized speed value for driving (range [$-1.0$, $1.0$]). |
| `float getSteering()` | `float` | None | Returns the normalized steering value (range [$-1.0$, $1.0$]). |

### 4. SteeringModule (Kinematics Logic)
**Goal:** Implements the core mathematical logic for **Ackerman Steering**.

| Function Name | Return Type | Parameters (Arguments) | Description |
| :--- | :--- | :--- | :--- |
| `SteeringModule()` | Constructor | None | Initializes with physical rover dimensions. |
| `void calculateServoAngles(float steeringInput)` | `void` | `float steeringInput` (range [$-1.0$, $1.0$]). | Calculates the four required servo angles based on Ackerman geometry and stores them internally. |
| `int getAngle(int wheelID)` | `int` | `int wheelID` (0-3 for steering servos). | Returns the last calculated angle (in degrees) for a specific steering servo. |

---
