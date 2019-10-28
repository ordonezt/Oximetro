/*
 * my_dataManagement.c
 *
 *  Created on: 10 oct. 2019
 *      Author: ordonezt
 */

#include "my_include.h"

#define PROM_R 0
#define PROM_SPO2 1
#define PROM PROM_SPO2

const float h[N_RAW] = {
		0.001456221097893,
		0.002411708629448,
		0.002910975063111,
		0.002411708629448,
		0.001456221097893,
};

const uint8_t A = 110;
const uint8_t B = 25;

volatile uint16_t raw[BUFFER_HEIGHT][N_RAW] = {0};
volatile uint16_t smooth[BUFFER_HEIGHT][BUFFER_LENGTH] = {0};
volatile uint16_t gradient[BUFFER_HEIGHT][N_GRADIENT] = {0};

bool isFinger(void)
{
	/*TODO*/
	return false;
}

uint8_t calculateSpO2(void)
{
	return 0;
//	uint8_t i = 0;
//
//#if(PROM == PROM_R)
//
//	float R = 0;
//	uint8_t SpO2 = 0;
//
//	for(i = 0; i < SAMPLES_LENGTH; i++)
//		R += log10(MAX[RED][i] / MIN[RED][i]) / log10(MAX[IR][i] / MIN[IR][i]);
//
//	R = (R / SAMPLES_LENGTH);
//
//	SpO2 = A - (B * R);
//
//#endif
//
//#if(PROM == PROM_SPO2)
//
//	float R = 0;
//	uint16_t SpO2 = 0;
//
//		for(i = 0; i < SAMPLES_LENGTH; i++)
//	{
//		R = log10(MAX[RED][i] / MIN[RED][i]) / log10(MAX[IR][i] / MIN[IR][i]);
//		SpO2 += A - (B * R);
//	}
//
//	SpO2 /= SAMPLES_LENGTH;
//#endif
//
//	return SpO2;
}

uint8_t calculateBPM(void)
{
	/*TODO*/
	return 0;
}

float filter (volatile uint16_t* x,const float* h, uint8_t length)
{
	uint8_t n = 0;
	float y = 0;

	for(n = 0; n < length; n++)
		y += (float)x[n] * h[n];

	return y;
}

void shiftBuffer(volatile uint16_t *buffer, uint8_t length)
{
	uint8_t i;
	for(i = 1; i < length; i++)
		buffer[i] = buffer[i-1];
}

void processData(void)
{
	uint8_t i, flag_peak, counter_max, flag_search_max, min, max = 0, flag_ready;
	//filter raw signal
	shiftBuffer(smooth[led], N_SMOOTH);
	smooth[led][0] = filter(raw[led], h, N_RAW);

	//obtain signal's derivative
	shiftBuffer(gradient[led], N_GRADIENT);
	gradient[led][0] = smooth[led][0] - smooth[led][3];//remember the derivative is shifted by 1 sample

	//check for derivative peak(rising edge)
	flag_peak = 1;
	for(i = 1; i < N_GRADIENT; i++)
		if(gradient[led][i] > gradient[led][0])
		{
			flag_peak = 0;
			break;
		}

	if(flag_peak)
	{
		flag_peak = 0;
		//start search for next relative maximum (on new data)
		max = smooth[led][1];
		flag_search_max = 1;
		counter_max = 0;
		//search for previous relative minimum (on old data)
		min = smooth[led][1];
		for(i = 2; i < (MIN_WINDOW + 2); i++)
			if(smooth[led][i] < min)
				min = smooth[led][i];
	}

	if(flag_search_max)
	{
		if(counter_max >= (MAX_WINDOW - 1))
		{
			flag_search_max = 0;
			flag_ready = 1;
		}
		if(smooth[led][0] > max)
		{
			max = smooth[led][0];
		}//else{flag_search_max=0;flag_ready=1;}
	}
}
