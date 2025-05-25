/*
 * pid.h
 */

#ifndef INC_PID_H_
#define INC_PID_H_

#include "main.h"

typedef enum
{
	NONE = -1,
	MOVE_ENC = 0,
	MOVE_IR_BOTH = 1,
	MOVE_IR_LEFT = 2,
	MOVE_IR_RIGHT = 3,
	TURN = 4,
	IDLE = 5
}MODE;


#define LEFT_IR_THRESHOLD 0.45
#define RIGHT_IR_THRESHOLD 0.45
#define FRONT_RIGHT_IR_THRESHOLD 0.35
#define FRONT_RIGHT_IR_JUMP 0.4

void resetPID(void);
void updatePID(void);
void setPIDGoalD(int distance);
void setPIDGoalA(int angle);
void setPIDMode(int m);
int8_t PIDdone(void);
void updateIR(float l, float fl, float fr, float r);

#endif /* INC_PID_H_ */
