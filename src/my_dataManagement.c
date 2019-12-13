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
float bpm = 0;
uint32_t deltaN=0;

extern RINGBUFF_T txring;

RINGBUFF_T RingBuffADC[BUFFER_HEIGHT];

const uint8_t A = 110;
const uint8_t B = 25;

float raw[BUFFER_HEIGHT][N_RAW] = {0};
float smooth[BUFFER_HEIGHT][BUFFER_LENGTH] = {0};
float gradient[BUFFER_HEIGHT][N_GRADIENT] = {0};

volatile uint8_t cuenta_muestras = 0;
uint8_t pos_peak[2] = {0,0};

void process(pulse_t pulse[2]) //TODO que recibe que devuelve?? RECIBIMOS UN ARRAY DE *pulse y calculamos frecuencia de pulso y oxigeno y dejamos en globales
{
	static uint8_t new_peak[2]={0,0};
	uint8_t led_index;

	uint8_t i, pos_aux = 0;

	float aux = 0;

	for (led_index = RED; led_index < 2; led_index++)
	{
		//filter raw signal
		shiftBuffer(smooth[led_index], N_SMOOTH);
		smooth[led_index][0] = filter((float)pulse[led_index].muestra, h, taps[led_index], N_RAW);

		//obtain signal's derivative
		shiftBuffer(gradient[led_index], N_GRADIENT);
		gradient[led_index][0] = smooth[led_index][0] - smooth[led_index][2];//remember the derivative is shifted by 1 sample

		//check for derivative peak(rising edge)
		for(i = 1, aux = gradient[led_index][0]; i < N_GRADIENT; i++){ //TODO pensar bien logica de pos_aux
			if(gradient[led_index][i] > aux)
			{
				aux = gradient[led_index][i];
				pos_aux = i;
			}
		}

		if(pos_aux != pulse[led_index].pos_Dmax)
		{
			new_peak[led_index] = TRUE;
			deltaN=pulse[led_index].pos_Dmax-pos_aux;
			bpm = 60000 / (deltaN * SAMPLE_PERIOD);
			pulse[led_index].pos_Dmax = pos_aux;
		}
		else
			new_peak[led_index] = FALSE;	//Dudoso
	}

	if (new_peak[RED] ) //Antes se usaban los dos, por que?
	{
		flags.beat_detected = TRUE;
		new_peak[RED] = 0;
		new_peak[IR] = 0;
		flags.beat_detected = TRUE;
	}

	pulse[RED].pos_Dmax++;
	pulse[IR].pos_Dmax++;
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

uint8_t checkFinger(void)
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
//	return aux > THRESHOLD;


//	flags.is_finger = !Chip_GPIO_GetPinState(LPC_GPIO, DC_LEVEL_PORT, DC_LEVEL_PIN); //Dedo 0, sin dedo 1


//	flags.is_finger = true;
	return TRUE;
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

void shiftBuffer(float *buffer, uint16_t length)
{
	uint16_t i;
	for(i = length-1; i > 0; i--)
		buffer[i]=buffer[i-1];
}
