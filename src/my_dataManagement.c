/*
 * my_dataManagement.c
 *
 *  Created on: 10 oct. 2019
 *      Author: ordonezt
 */

#include "my_include.h"

#define PROM_R 0
#define PROM_SPO2 1
#define PROM PROM_R

float freq = 0;
uint32_t deltaN=0;

extern RINGBUFF_T txring;

const float h[N_RAW] =
{
		0.038626528332096295,
		0.14300345854663776,
		0.233752240231106,
		0.28624515754407337,
		0.233752240231106,
		0.14300345854663776,
		0.038626528332096295,
};

const uint8_t A = 110;
const uint8_t B = 25;

float raw[BUFFER_HEIGHT][N_RAW] = {0};
float smooth[BUFFER_HEIGHT][BUFFER_LENGTH] = {0};
float gradient[BUFFER_HEIGHT][N_GRADIENT] = {0};

volatile uint8_t cuenta_muestras = 0;
uint8_t pos_peak[2] = {0,0};

void checkFinger(void)
{
//	uint8_t i;
//	float aux = 0;
//
//	for(i = 0; i < N_FINGER; i++)
//		aux += abs(smooth[IR][i] - DC_LEVEL);
//
//	aux /= N_FINGER;
//
//	if(aux > THRESHOLD)
//		flags.is_finger = true;
//	else
//		flags.is_finger = false;



//	flags.is_finger = !Chip_GPIO_GetPinState(LPC_GPIO, DC_LEVEL_PORT, DC_LEVEL_PIN); //Dedo 0, sin dedo 1


	flags.is_finger = true;
}

uint8_t calculateSpO2(pulse_t *Data[])
{
	uint8_t i = 0;

#if(PROM == PROM_R)

	float R = 0;
	uint8_t SpO2 = 0;

	for(i = 0; i < N_PROM; i++)
		R += log10(Data[RED]->Max[i] / Data[RED]->Min[i]) / log10(Data[IR]->Max[i] / Data[IR]->Min[i]);

	R = (R / N_PROM);

	SpO2 = A - (B * R);

#endif

#if(PROM == PROM_SPO2)

	float R = 0;
	uint16_t SpO2 = 0;

		for(i = 0; i < SAMPLES_LENGTH; i++)
	{
		R = log10(MAX[RED][i] / MIN[RED][i]) / log10(MAX[IR][i] / MIN[IR][i]);
		SpO2 += A - (B * R);
	}

	SpO2 /= SAMPLES_LENGTH;
#endif

	return SpO2;
}

uint8_t calculateBPM(void)
{
	return 60000 / (deltaN * SAMPLE_PERIOD);
}

float filter (volatile float* x,const float* h, uint8_t length)
{
	uint8_t n = 0;
	float y = 0;

	for(n = 0; n < length; n++)
		y += ((float)x[n]) * h[n];

	return y;
}

void shiftBuffer(float *buffer, uint16_t length)
{
	uint16_t i;
	for(i = length-1; i > 0; i--)
		buffer[i] = buffer[i-1];
}

void process(pulse_t *pulse)
{
	static uint8_t new_peak[2]={0,0};

	uint8_t i, pos_aux = 0;
	uint8_t n = '\n';

	float aux = 0, bpm = 0;

	//filter raw signal
	shiftBuffer(smooth[pulse->Led], N_SMOOTH);
	smooth[pulse->Led][0] = filter(raw[pulse->Led], h, N_RAW);

	//obtain signal's derivative
	shiftBuffer(gradient[pulse->Led], N_GRADIENT);
	gradient[pulse->Led][0] = smooth[pulse->Led][0] - smooth[pulse->Led][2];//remember the derivative is shifted by 1 sample

	//check for derivative peak(rising edge)
	for(i = 1, aux = gradient[pulse->Led][0]; i < N_GRADIENT; i++){ //TODO pensar bien logica de pos_aux
		if(gradient[pulse->Led][i] > aux)
		{
			aux = gradient[pulse->Led][i];
			pos_aux = i;
		}
	}

	if(pos_aux != pulse->pos_Dmax){
		//if((pulse->pos_Dmax - pos_aux) > 5 ){}
		new_peak[pulse->Led] = 1;
		//freq=1000/(float)(SAMPLE_PERIOD*(pulse->pos_Dmax-pos_aux));
		deltaN=pulse->pos_Dmax-pos_aux;
		bpm = 60000 / (deltaN * SAMPLE_PERIOD);
		RingBuffer_Insert(&txring, (uint8_t*) &bpm);
		RingBuffer_Insert(&txring, &n);
		pulse->pos_Dmax = pos_aux;
	}

	if (new_peak[RED] == 1 && new_peak[IR] == 1) {
		flags.beat_detected = true;
		new_peak[RED] = 0;
		new_peak[IR] = 0;
		cuenta_muestras = 0;
		Chip_GPIO_SetPinToggle(LPC_GPIO, STATE_PORT, STATE_PIN);
	}
//	if (flags.beat_detected){
//		cuenta_muestras++;
//	}


//	if(new_peak == 1 && pos_peak >= MAX_WINDOW){ // TODO Pensar, sacar afuera de la
//		flags.beat_detected = true;
//		new_peak = 0;
//	}

	pulse->pos_Dmax++;
}

void get_min_max_values(pulse_t *Data[]){
	led_t led_local;
	uint16_t i, min, max, pos_Dmax;

	for (led_local = RED; led_local <= IR; led_local++) {

		pos_Dmax = Data[led_local]->pos_Dmax;

		min = smooth[led_local][pos_Dmax];

		for(i = pos_Dmax; i < (pos_Dmax + MIN_WINDOW); i++){
			if(smooth[led_local][i] < min)
				min = smooth[led_local][i];
		}

		max = smooth[led_local][pos_Dmax];

		for(i = pos_Dmax; i > (pos_Dmax - MAX_WINDOW) && i > 0; i--){
			if(smooth[led_local][i] > max)
				max = smooth[led_local][i];
		}

		shiftBuffer(Data[led_local]->Max, N_PROM);
		shiftBuffer(Data[led_local]->Min, N_PROM);

		Data[led_local]->Min[0] = min;
		Data[led_local]->Max[0] = max;
	}
}
//
//float get_R_value(pulse_t *Data[2]){
//
//}
