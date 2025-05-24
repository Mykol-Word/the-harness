/*
 * irs.c
 */

#include "main.h"
#include "irs.h"
#include "delay.h"

// This is the buffer that will get filled up with all the measurements
uint16_t adc_buf[NUM_SAMPLES];
// "boolean" variable to keep say when the ADC has finished filling up the buffer
uint8_t complete = 0;

// Reads IR value specified
int readIR(IR ir)
{
	// Turn on appropriate emitter
	switch (ir)
	{
	case IR_LEFT:
		HAL_GPIO_WritePin(LeftEmitter_GPIO_Port, LeftEmitter_Pin, GPIO_PIN_SET);
		break;
	case IR_FRONT_LEFT:
		HAL_GPIO_WritePin(FrontLeftEmitter_GPIO_Port, FrontLeftEmitter_Pin, GPIO_PIN_SET);
		break;
	case IR_FRONT_RIGHT:
		HAL_GPIO_WritePin(FrontRightEmitter_GPIO_Port, FrontRightEmitter_Pin, GPIO_PIN_SET);
		break;
	case IR_RIGHT:
		HAL_GPIO_WritePin(RightEmitter_GPIO_Port, RightEmitter_Pin, GPIO_PIN_SET);
		break;
	}

	// Let photodiode react
	delayMicroseconds(30);
	// Read photodiode value
	int read = analogRead(ir);

	// Turn off emitters
	HAL_GPIO_WritePin(LeftEmitter_GPIO_Port, LeftEmitter_Pin, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(FrontLeftEmitter_GPIO_Port, FrontLeftEmitter_Pin, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(FrontRightEmitter_GPIO_Port, FrontRightEmitter_Pin, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(RightEmitter_GPIO_Port, RightEmitter_Pin, GPIO_PIN_RESET);

	return read;
}

// Reads specific IR
float readLeftIR(void)
{
	return readIR(IR_LEFT) / IR_LEFT_C;
}

float readFrontLeftIR(void)
{
	return readIR(IR_FRONT_LEFT) / IR_FRONT_LEFT_C;
}

float readFrontRightIR(void)
{
	return readIR(IR_FRONT_RIGHT) / IR_FRONT_RIGHT_C;
}


float readRightIR(void)
{
	return readIR(IR_RIGHT) / IR_RIGHT_C;
}

// Reads the specific channel of the ADC corresponding to the correct IR
int analogRead(IR ir)
{
    ADC_ChannelConfTypeDef sConfig = {0}; // this initializes the IR ADC [Analog to Digital Converter]
    ADC_HandleTypeDef *hadc1_ptr = Get_HADC1_Ptr(); // this is a pointer to your hal_adc

    // picks the IR direction to choose the right ADC.
    switch(ir)
    {
        case IR_LEFT:
            sConfig.Channel = ADC_CHANNEL_5;
            break;
        case IR_FRONT_LEFT:
            sConfig.Channel = ADC_CHANNEL_6;
            break;
        case IR_FRONT_RIGHT:
            sConfig.Channel = ADC_CHANNEL_7;
            break;
        case IR_RIGHT:
            sConfig.Channel = ADC_CHANNEL_14;
            break;
        default:
            return 0;
    }

    sConfig.Rank = 1;
    sConfig.SamplingTime = ADC_SAMPLETIME_3CYCLES;

    // make sure everything was set up correctly
    if (HAL_ADC_ConfigChannel(hadc1_ptr, &sConfig) != HAL_OK)
    {
        return 0;
    }

    complete = 0;

    // start filling up the ADC buffer
    HAL_ADC_Start_DMA(hadc1_ptr, (uint32_t*)adc_buf, NUM_SAMPLES);

    // wait for the buffer to become full
    while (complete == 0)
    {
        continue;
    }

    uint32_t sum = 0;
    // calculate the sum of the measurements in order to calculate the average
    uint16_t measurement = 0;
    while(measurement < NUM_SAMPLES) //this takes multiple measurements
    {
        sum += adc_buf[measurement];
        ++measurement;
    }

    return sum/NUM_SAMPLES;
}

// stops the ADC and changes our "complete" variable to be "true"
void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef* hadc) {
    // stop the ADC
    HAL_ADC_Stop_DMA(hadc);
    complete = 1;
}
