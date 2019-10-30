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
	static uint8_t Dmax[2], pos_peak[2];
	static uint8_t Dmin;

	uint8_t i, new_peak = 0, pos_aux = 0, aux = 0;
	uint8_t counter_max, flag_search_max, min, flag_ready;

	pos_peak[led]++;

	//filter raw signal
	shiftBuffer(smooth[led], N_SMOOTH);
	smooth[led][0] = filter(raw[led], h, N_RAW);

	//obtain signal's derivative
	shiftBuffer(gradient[led], N_GRADIENT);
	gradient[led][0] = smooth[led][0] - smooth[led][2];//remember the derivative is shifted by 1 sample

	//check for derivative peak(rising edge)
	for(i = 1; i < N_GRADIENT; i++){
		if(gradient[led][i] > aux)
		{
			aux = gradient[led][i];
			pos_aux = i;
		}
	}

	if(pos_aux != pos_peak[led]){
		new_peakled = 1;
		pos_peak[led] = pos_aux;
	}
	if(new_peak == 1 && pos_peak >= MAX_WINDOW){ // TODO Pensar, sacar afuera de la
		flags.beat_detected = true;
		new_peak = 0;
	}
}

void get_min_max_values(void){
	int i;

	min = smooth[led][pos_peak];

	for(i = pos_peak; i < (pos_peak + MIN_WINDOW); i++){
		if(smooth[led][i] < min)
			min = smooth[led][i];
	}

	max = smooth[led][pos_peak];

	for(i = pos_peak; i > (pos_peak - MAX_WINDOW); i--){
		if(smooth[led][i] > max)
			max = smooth[led][i];
	}
}
