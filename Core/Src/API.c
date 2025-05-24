#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "irs.h"
#include "pid.h"

#define BUFFER_SIZE 32

int API_wallFront() {
    return readFrontRightIR() > FRONT_RIGHT_IR_THRESHOLD;
}

int API_wallRight() {
	return readRightIR() > RIGHT_IR_THRESHOLD;
}

int API_wallLeft() {
	return readLeftIR() > LEFT_IR_THRESHOLD;
}

