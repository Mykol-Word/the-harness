/*
 * irs.h
 */

#ifndef INC_IRS_H_
#define INC_IRS_H_

// The number of samples to take
#define NUM_SAMPLES 16

// Distinguishes IRs
typedef enum
{
	IR_LEFT = 0,
	IR_FRONT_LEFT = 1,
	IR_FRONT_RIGHT = 2,
	IR_RIGHT = 3
}IR;

// IR normalization constant. Normalizes to 1.0
#define IR_LEFT_C 1580.0;
#define IR_FRONT_LEFT_C 895.0;
#define IR_FRONT_RIGHT_C 1863.0;
#define IR_RIGHT_C 1529.0;

int readIR(IR ir);
float readLeftIR(void);
float readFrontLeftIR(void);
float readFrontRightIR(void);
float readRightIR(void);
int analogRead(IR ir);

#endif /* INC_IRS_H_ */
