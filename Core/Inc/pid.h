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
	MOVE_IR = 1,
	TURN = 2,
	IDLE = 3
}MODE;

void resetPID(void);
void updatePID(void);
void setPIDGoalD(int distance);
void setPIDGoalA(int angle);
void setPIDMode(int m);
int8_t PIDdone(void);

#endif /* INC_PID_H_ */
