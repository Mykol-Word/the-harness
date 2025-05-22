/*
 * pid.c
 */

#include "main.h"
#include "pid.h"
#include "motors.h"
#include "encoders.h"

float kP_dist = 0;
float kD_dist = 0;

float kP_angl = 0;
float kD_angl = 0;

int success_counts = 1000;
int counter = 0;

float angl_goal = 0;
float dist_goal = 0;

float dist_error = 0;
float angl_error = 0;

float old_dist_error = 0;
float old_angl_error = 0;

float dist_response = 0;
float angl_response = 0;

MODE mode = NONE;

// reset motors, encoders, goals, etc.
void resetPID() {
	/*
	 * For assignment 3.1: This function does not need to do anything
	 * For assignment 3.2: This function should reset all the variables you define in this file to help with PID to their default
	 *  values. You should also reset your motors and encoder counts (if you tell your rat to turn 90 degrees, there will be a big
	 * difference in encoder counts after it turns. If you follow that by telling your rat to drive straight without first
	 * resetting the encoder counts, your rat is going to see a huge angle error and be very unhappy).
	 *
	 * You should additionally set your distance and error goal values (and your oldDistanceError and oldAngleError) to zero.
	 */
	resetMotors();
	resetEncoders();

	kP_dist = 0;
	kD_dist = 0;
	kP_angl = 0;
	kD_angl = 0;

	angl_goal = 0;
	dist_goal = 0;

	dist_error = 0;
	angl_error = 0;

	old_dist_error = 0;
	old_angl_error = 0;

	dist_response = 0;
	angl_response = 0;

	counter = 0;
	mode = NONE;
}

// Uses appropriate PID controller depending on mode
void updatePID() {
	int L_encoder_cnt = getLeftEncoderCounts();
	int R_encoder_cnt = getRightEncoderCounts();

	//update errors
	old_dist_error = dist_error;
	old_angl_error = angl_error;

	dist_error = dist_goal - (R_encoder_cnt + L_encoder_cnt) / 2.0;
	angl_error = angl_goal - (R_encoder_cnt - L_encoder_cnt);

	//PID Functions
	dist_response = dist_error * kP_dist + (dist_error - old_dist_error) * kD_dist;
	angl_response = angl_error * kP_angl + (angl_error - old_angl_error) * kD_angl;

	switch(mode) {
		case(MOVE_ENC):
			setMotorRPWM(0.25 + angl_response);
			setMotorLPWM(0.25 - angl_response);
			break;
		case(MOVE_IR):
			//not implemented
			resetMotors();
		case(TURN):
		case(IDLE):
			setMotorRPWM(angl_response + dist_response);
			setMotorLPWM(-angl_response + dist_response);
			break;
		case(NONE):
		default:
			// This should never happen
			resetMotors();
			break;
	}
}

//Set dist_goal to distance encoder counts
void setPIDGoalD(int distance) {
	dist_goal = distance;
}

// Set angl_goal to angle encoder counts
void setPIDGoalA(int angle) {
	angl_goal = angle;
}

void setPIDMode(int new_mode) {
	//Change constants
	switch(new_mode){
		case(MOVE_ENC):
			mode = new_mode;
			kP_dist = 0.013;
			kD_dist = 0;
			kP_angl = 0.04;
			kD_angl = 0.4;
			success_counts = 1;
			break;
		case(MOVE_IR):
			// not implemented
			break;
		case(TURN):
			mode = new_mode;
			kP_dist = 0.013;
			kD_dist = 0;
			kP_angl = 0.015;
			kD_angl = 0.3;
			success_counts = 1000;
			break;
		case(IDLE):
			mode = new_mode;
			kP_dist = 0.03;
			kD_dist = 0;
			kP_angl = 0.03;
			kD_angl = 0.3;
			success_counts = 50000;
			break;
		case(NONE):
		default:
			// This should never happen
			mode = NONE;
			kP_dist = 0;
			kD_dist = 0;
			kP_angl = 0;
			kD_angl = 0;
			break;
	}
}

// Return 1 if PID is done, 0 otherwise. Controller/mode specific.
int8_t PIDdone(void) {
	switch(mode){
		case(MOVE_ENC):
			if(dist_error < 0) {
				return 1;
			}
			return 0;
			break;
		case(MOVE_IR):
			// not implemented
			return 0;
		case(TURN):
			if(angl_error <= 10 && angl_error >= -10) {
				counter += 1;
				if (counter >= success_counts) return 1;
			}
			else {
				counter = 0;
			}
			return 0;
			break;
		case(IDLE):
			counter += 1;
			if (counter >= success_counts) {
				return 1;
			}
			return 0;
			break;
		case(NONE):
		default:
			// This should never happen.
			return 0;
	}

}
