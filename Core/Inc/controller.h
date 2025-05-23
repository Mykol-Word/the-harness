/*
 * controller.h
 */

#ifndef INC_CONTROLLER_H_
#define INC_CONTROLLER_H_

#include "main.h"

void move(int n);
void move_ir(int n);
void turn(int n);
void turn_smooth(int d, int a);
void idle();

#endif /* INC_CONTROLLER_H_ */
