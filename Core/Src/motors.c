/*
 * motors.c
 */

#include "motors.h"


// This function returns PWM_MAX if pwm > PWM_MAX, -PWM_MAX if pwm < -PWM_MAX, and pwm otherwise.
float limitPWM(float pwm) {
	if (pwm > PWM_MAX)
		return PWM_MAX;
	else if (pwm < -PWM_MAX)
		return -PWM_MAX;
	else
		return pwm;
}

// This function controls the left motor speed and direction.
void setMotorLPWM(float pwm) {
	if (pwm >= 0) { // move forward
		TIM2->CCR2 = 0;
		TIM2->CCR3 = (uint32_t) (limitPWM(pwm) * MAX_TIMER_COUNTS);
	}
	else { // move backward
		TIM2->CCR3 = 0;
		TIM2->CCR2 = (uint32_t) (-limitPWM(pwm) * MAX_TIMER_COUNTS);
	}

}

// This function controls the right motor speed and direction.
void setMotorRPWM(float pwm) {
	if (pwm >= 0) { // move forward
		TIM2->CCR1 = 0;
		TIM2->CCR4 = (uint32_t) (limitPWM(pwm) * MAX_TIMER_COUNTS);
	}
	else { // move backward
		TIM2->CCR4 = 0;
		TIM2->CCR1 = (uint32_t) (-limitPWM(pwm) * MAX_TIMER_COUNTS);
	}
}

// Set all motor speeds to 0
void resetMotors() {
	TIM2->CCR1 = 0;
	TIM2->CCR2 = 0;
	TIM2->CCR3 = 0;
	TIM2->CCR4 = 0;
}
