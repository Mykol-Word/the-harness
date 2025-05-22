/*
 * controller.c
 */

#include "main.h"
#include "controller.h"
#include "encoders.h"
#include "motors.h"
#include "pid.h"

// Move n encoder counts
void move(int n) {
	resetPID();
	setPIDMode(MOVE_ENC); // Later add new move type or conditional in this for MOVE_IR
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

// brakes or idles your mouse, forcing it to stay in place
void idle() {
	resetPID();
	setPIDMode(IDLE);
	setPIDGoalA(0);
	setPIDGoalD(0);

	while(PIDdone() == 0);

	resetPID();
}
