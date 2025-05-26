/*
 * pid.c
 */

#include "main.h"
#include "pid.h"
#include "motors.h"
#include "encoders.h"
#include "irs.h"

float kP_dist = 0;
float kD_dist = 0;

float kP_angl_enc = 0;
float kD_angl_enc = 0;

float kP_angl_ir = 0;
float kD_angl_ir = 0;

int success_counts = 1000;
int counter = 0;

float angl_enc_goal = 0;
float angl_ir_goal = 0;
float dist_goal = 0;

float dist_error = 0;
float angl_enc_error = 0;
float angl_ir_error = 0;

float old_dist_error = 0;
float old_angl_enc_error = 0;
float old_angl_ir_error = 0;

float dist_response = 0;
float angl_enc_response = 0;
float angl_ir_response = 0;

float left_ir = 0;
float front_left_ir = 0;
float front_right_ir = 0;
float right_ir = 0;

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
	kP_angl_enc = 0;
	kD_angl_enc = 0;
	kP_angl_ir = 0;
	kD_angl_ir = 0;

	angl_enc_goal = 0;
	angl_ir_goal = 0;
	dist_goal = 0;

	dist_error = 0;
	angl_enc_error = 0;
	angl_ir_error = 0;

	old_dist_error = 0;
	old_angl_enc_error = 0;
	old_angl_ir_error = 0;

	dist_response = 0;
	angl_enc_response = 0;
	angl_ir_response = 0;

	counter = 0;
	mode = NONE;
}

// Uses appropriate PID controller depending on mode
void updatePID() {
	int L_encoder_cnt = getLeftEncoderCounts();
	int R_encoder_cnt = getRightEncoderCounts();

	if(mode == MOVE_ENC || mode == MOVE_IR_BOTH || mode == MOVE_IR_LEFT || mode == MOVE_IR_RIGHT ) {
		if(right_ir >= RIGHT_IR_THRESHOLD && left_ir >= LEFT_IR_THRESHOLD) setPIDMode(MOVE_IR_BOTH);
		else if(left_ir >= LEFT_IR_THRESHOLD) setPIDMode(MOVE_IR_LEFT);
		else if(right_ir >= RIGHT_IR_THRESHOLD) setPIDMode(MOVE_IR_RIGHT);
		else setPIDMode(MOVE_ENC);
	}

	//update errors
	old_dist_error = dist_error;
	old_angl_enc_error = angl_enc_error;
	old_angl_ir_error = angl_ir_error;

	dist_error = dist_goal - (R_encoder_cnt + L_encoder_cnt) / 2.0;
	angl_enc_error = angl_enc_goal - (R_encoder_cnt - L_encoder_cnt);

	float ir_right_error = 1.0 - right_ir;
	float ir_left_error = 1.0 - left_ir;

	switch(mode) {
		case(MOVE_IR_BOTH):
			angl_ir_error = (ir_right_error - ir_left_error);
			break;
		case(MOVE_IR_LEFT):
			angl_ir_error = -(2 * ir_left_error);
			break;
		case(MOVE_IR_RIGHT):
			angl_ir_error = (2 * ir_right_error);
			break;
		default:
			angl_ir_error = 0;
	}


	//PID Functions
	dist_response = dist_error * kP_dist + (dist_error - old_dist_error) * kD_dist;
	angl_enc_response = angl_enc_error * kP_angl_enc + (angl_enc_error - old_angl_enc_error) * kD_angl_enc;
	angl_ir_response = angl_ir_error * kP_angl_ir + (angl_ir_error - old_angl_ir_error) * kD_angl_ir;

	switch(mode) {
		case(MOVE_ENC):
			setMotorRPWM(0.35 + angl_enc_response);
			setMotorLPWM(0.35 - angl_enc_response);
			break;
		case(MOVE_IR_BOTH):
		case(MOVE_IR_LEFT):
		case(MOVE_IR_RIGHT):
			setMotorRPWM(CLAMP(0.35 - angl_ir_response, 0.3, 0.4));
			setMotorLPWM(CLAMP(0.35 + angl_ir_response, 0.3, 0.4));
			break;
		case(TURN):
		case(IDLE):
			setMotorRPWM(angl_enc_response + dist_response);
			setMotorLPWM(-angl_enc_response + dist_response);
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

// Set angl_enc_goal to angle encoder counts
void setPIDGoalA(int angle) {
	angl_enc_goal = angle;
}

void setPIDMode(int new_mode) {
	//Change constants
	switch(new_mode){
		case(MOVE_ENC):
			mode = new_mode;
			kP_dist = 0;
			kD_dist = 0;
			kP_angl_enc = 0.04;
			kD_angl_enc = 0.4;
			kP_angl_ir = 0;
			kD_angl_ir = 0;
			success_counts = 1;
			break;
		case(MOVE_IR_BOTH):
		case(MOVE_IR_LEFT):
		case(MOVE_IR_RIGHT):
			mode = new_mode;
			kP_dist = 0;
			kD_dist = 0;
			kP_angl_enc = 0;
			kD_angl_enc = 0;
			kP_angl_ir = 0.03;
			kD_angl_ir = 6000000;
			break;
		case(TURN):
			mode = new_mode;
			kP_dist = 0.013;
			kD_dist = 0;
			kP_angl_enc = 0.017;
			kD_angl_enc = 0.3;
			kP_angl_ir = 0;
			kD_angl_ir = 0;
			success_counts = 75;
			break;
		case(IDLE):
			mode = new_mode;
			kP_dist = 0.03;
			kD_dist = 0;
			kP_angl_enc = 0.03;
			kD_angl_enc = 0.3;
			kP_angl_ir = 0;
			kD_angl_ir = 0;
			success_counts = 75;
			break;
		case(NONE):
		default:
			// This should never happen
			mode = NONE;
			kP_dist = 0;
			kD_dist = 0;
			kP_angl_enc = 0;
			kD_angl_enc = 0;
			kP_angl_ir = 0;
			kD_angl_ir = 0;
			break;
	}
}

// Return 1 if PID is done, 0 otherwise. Controller/mode specific.
int8_t PIDdone(void) {
	updateIR(readLeftIR(),readFrontLeftIR(),readFrontRightIR(), readRightIR());
	switch(mode){
		case(MOVE_ENC):
		case(MOVE_IR_BOTH):
		case(MOVE_IR_LEFT):
		case(MOVE_IR_RIGHT):
			if(front_right_ir > FRONT_RIGHT_IR_JUMP) {
				return front_right_ir >= 0.9;
			}
			else {
				return dist_error < 0 || front_right_ir >= 0.9;
			}
			break;
		case(TURN):
			if((int)angl_enc_error % 10 == 0) HAL_GPIO_WritePin(LED_2_GPIO_Port, LED_2_Pin, GPIO_PIN_SET);
			else HAL_GPIO_WritePin(LED_2_GPIO_Port, LED_2_Pin, GPIO_PIN_RESET);
			if(angl_enc_error <= 10 && angl_enc_error >= -10) {
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

void updateIR(float l, float fl, float fr, float r) {
	left_ir = l;
	front_left_ir = fl;
	front_right_ir = fr;
	right_ir = r;
}

