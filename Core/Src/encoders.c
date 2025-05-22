/*
 * encoders.c
 */

#include "main.h"
#include "encoders.h"

// Returns the right encoder value
int16_t getRightEncoderCounts() {
	return (int16_t) TIM3->CNT;
}

// Returns the left encoder value
int16_t getLeftEncoderCounts() {
	return (int16_t) TIM4->CNT;
}

// Reset encoder counts to 0
void resetEncoders() {
	TIM3->CNT = (int16_t) 0;
	TIM4->CNT = (int16_t) 0;
}
