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

uint16_t raw[BUFFER_HEIGHT][N_RAW] = {0};

volatile uint16_t smooth[BUFFER_HEIGHT][BUFFER_LENGTH] = {0};
volatile uint16_t gradient[BUFFER_HEIGHT][N_GRADIENT] = {0};

volatile cuenta_muestras = 0;
uint8_t pos_peak[2] = {0,0};

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

void process(pulse_t *pulse)
{
	static new_peak[2]={0,0};

	uint8_t i, pos_aux = 0, aux = 0;
	uint8_t counter_max, flag_search_max, min, flag_ready;

	pulse->pos_Dmax++;

	//filter raw signal
	shiftBuffer(smooth[pulse->Led], N_SMOOTH);
	smooth[pulse->Led][0] = filter(raw[pulse->Led], h, N_RAW);

	//obtain signal's derivative
	shiftBuffer(gradient[pulse->Led], N_GRADIENT);
	gradient[pulse->Led][0] = smooth[pulse->Led][0] - smooth[pulse->Led][2];//remember the derivative is shifted by 1 sample

	//check for derivative peak(rising edge)
	for(i = 1; i < N_GRADIENT; i++){
		if(gradient[pulse->Led][i] > aux)
		{
			aux = gradient[pulse->Led][i];
			pos_aux = i;
		}
	}

	if(pos_aux != pulse->pos_Dmax){
		new_peakled = 1;
		pulse->pos_Dmax = pos_aux;
	}

	if (new_peak[RED] == 1 && new_peak[IR] == 1) {
		flags.beat_detected = true;
		cuenta_muestras++;
		new_peak[RED] = 0;
		new_peak[IR] = 0;
	}

//	if(new_peak == 1 && pos_peak >= MAX_WINDOW){ // TODO Pensar, sacar afuera de la
//		flags.beat_detected = true;
//		new_peak = 0;
//	}
}

void get_min_max_values(pulse_t *Data[]){
	int i, led;
	uint16_t min, max;

	for (led = RED; led <= IR; led++) {
		min = smooth[led][Data[led]->pos_Dmax];

		for(i = Data[led]->pos_Dmax; i < (Data[led]->pos_Dmax + MIN_WINDOW); i++){
			if(smooth[led][i] < min)
				min = smooth[led][i];
		}

		max = smooth[led][Data[led]->pos_Dmax];

		for(i = Data[led]->pos_Dmax; i > (Data[led]->pos_Dmax - MAX_WINDOW); i--){
			if(smooth[led][i] > max)
				max = smooth[led][i];
		}
		Data[led]->Min = min;
		Data[led]->Max = max;
	}
}
