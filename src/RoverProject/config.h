#ifndef ROVER_CONFIG_H
#define ROVER_CONFIG_H

#include <Arduino.h>

// --- DC motor pins (IN1 = PWM where noted) ---
#define DCmotor_F_L_pin1 7  // PWM pin
#define DCmotor_F_L_pin2 6

#define DCmotor_F_R_pin1 13 // PWM pin
#define DCmotor_F_R_pin2 10

#define DCmotor_M_L_pin1 5  // PWM pin
#define DCmotor_M_L_pin2 4

#define DCmotor_M_R_pin1 9  // PWM pin
#define DCmotor_M_R_pin2 8

#define DCmotor_B_L_pin1 3  // PWM pin
#define DCmotor_B_L_pin2 2

#define DCmotor_B_R_pin1 12 // PWM pin
#define DCmotor_B_R_pin2 11

#define MaxSpeed 255
#define null 999

// --- Servo (5 servos, signal pins 22–26) ---
#define SERVO_COUNT 5

#define SERVO_0_PIN 22
#define SERVO_1_PIN 23
#define SERVO_2_PIN 24
#define SERVO_3_PIN 25
#define SERVO_4_PIN 26

#define SERVO_PIN SERVO_0_PIN // single-servo tests / default instance

#define MAX_PULSE_WIDTH 2500 // Maximum pulse width (microseconds)
#define MIN_PULSE_WIDTH 500  // Minimum pulse width (microseconds)
#define MIN_ANGLE -135         // Minimum servo angle (degrees)
#define MAX_ANGLE 135          // Maximum servo angle (degrees)
#define SERVO_DEFAULT_MOVE_SPEED_DEG_SEC 60

// --- RC / iBus (Serial1 RX = pin 19 on Arduino Mega) ---
#define noiseMargine 20
#define controllerMinValue 1000
#define controllerMaxValue 2000
#define controllerMidValue ((controllerMinValue + controllerMaxValue) / 2)
#define linkTimeoutLimit 200

// --- Stepper (STEP/DIR driver, e.g. SmartStepperController) ---
#define STEPPER_STEP_PIN 27
#define STEPPER_DIR_PIN 28
#define MICROSEC_IN_MIN 60000000

#endif // ROVER_CONFIG_H
