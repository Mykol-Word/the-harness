/*
 * controller.c
 */

#include "main.h"
#include "controller.h"
#include "encoders.h"
#include "motors.h"
#include "pid.h"

// Move n encoder counts with encoder angle correction
void move(int n) {
	resetPID();
	setPIDMode(MOVE_ENC);
	setPIDGoalA(0);
	setPIDGoalD(n);

	while(PIDdone() == 0);

	resetPID();
}


//Move n encoder counts with ir angle correction
void move_ir(int n) {
	resetPID();
	setPIDMode(MOVE_IR_BOTH);
	setPIDGoalA(0);
	setPIDGoalD(n);

	while(PIDdone() == 0);

	resetPID();
}

// turns your mouse n encoder counts
void turn(int n) {
	resetPID();
	setPIDMode(TURN);
	setPIDGoalA(n);
	setPIDGoalD(0);

	while(PIDdone() == 0);

	resetPID();
}

//turns your mouse smoothly
void turn_smooth(int d, int a) {
	resetPID();
	setPIDMode(TURN_SMOOTH);
	setPIDGoalA(a);
	setPIDGoalD(d);

	while(PIDdone() == 0);

	resetPID();
}

// brakes or idles your mouse, forcing it to stay in place
void idle() {
	resetPID();
	setPIDMode(IDLE);
	setPIDGoalA(0);
	setPIDGoalD(0);

	while(PIDdone() == 0);

	resetPID();
}
