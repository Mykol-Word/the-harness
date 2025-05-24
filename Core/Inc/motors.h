/*
 * motors.h
 */

#include "main.h"

#ifndef INC_MOTORS_H_
#define INC_MOTORS_H_

#define PWM_MAX 0.43 // Max speed
#define MAX_TIMER_COUNTS 3199
#define CLAMP(val, min, max) ((val) < (min) ? (min) : ((val) > (max) ? (max) : (val)))

float limitPWM(float pwm);
void setMotorRPWM(float pwm);
void setMotorLPWM(float pwm);
void resetMotors();

#endif /* INC_MOTORS_H_ */
