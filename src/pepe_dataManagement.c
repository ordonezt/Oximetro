/*
 * my_dataManagement.c
 *
 *  Created on: 10 oct. 2019
 *      Author: ordonezt
 */

#include "my_include.h"

extern RINGBUFF_T txring;

RINGBUFF_T RingBuffADC;

const uint8_t A = 110;
const uint8_t B = 25;

float raw[N_RAW] = {0};
float smooth[BUFFER_LENGTH] = {0};
float gradient[N_GRADIENT] = {0};

volatile uint8_t cuenta_muestras = 0;
uint8_t pos_peak[2] = {0,0};
uint32_t cuenta_impresas = 0;

void process(pulse_t *pulse) //TODO que recibe que devuelve?? RECIBIMOS UN ARRAY DE *pulse y calculamos frecuencia de pulso y oxigeno y dejamos en globales
{
	static uint8_t new_peak = 0;
	uint8_t i, pos_aux = 0;
	float aux = 0;

	//filter raw signal
	shiftBuffer(smooth, N_SMOOTH);
	if (((pulse->muestra-smooth[1]) < 1000)&&(pulse->muestra+smooth[1]>1000) || smooth[1] == 0)
	{
		smooth[0] = filter((float)pulse->muestra, h, taps, N_RAW);
	}
	else
	{
		smooth[0]=smooth[1];
	}
	//smooth[led_index][0] = filter((float)pulse[led_index].muestra, h, taps[led_index], N_RAW);
	//obtain signal's derivative
	shiftBuffer(gradient, N_GRADIENT);
	//se filtró el gradiente también
	gradient[0] = smooth[0] - smooth[2];//filter((float)(smooth[led_index][0] - smooth[led_index][2]), h, taps_grad[led_index], N_RAW);//remember the derivative is shifted by 1 sample

	//check for derivative peak(rising edge)
	for(i = 1, aux = gradient[0]; i < N_GRADIENT; i++){ //TODO pensar bien logica de pos_aux
		if(gradient[i] > aux)
		{
			aux = gradient[i];
			pos_aux = i;
		}
	}

	if(((pulse->pos_Dmax - pos_aux) > 100)&&(pos_aux-pulse->pos_Dmax)>100)
	{
		new_peak = TRUE;
		pulse->Delta = pulse->pos_Dmax-pos_aux;
		pulse->pos_Dmax = pos_aux;
	}
	else
		new_peak = FALSE;	//Dudoso

	if (new_peak) //Antes se usaban los dos, por que?
	{
		flags.beat_detected = TRUE;
		new_peak = FALSE;
		//get_min_max_values(pulse);
	}

	pulse->pos_Dmax++;
	cuenta_impresas++;
}

void get_min_max_values(pulse_t *pulse){
	led_t led_local;
	uint16_t i, min, max, pos_Dmax;

	pos_Dmax = pulse->pos_Dmax;

	min = smooth[pos_Dmax];

	for(i = pos_Dmax; i < (pos_Dmax + MIN_WINDOW); i++){
		if(smooth[i] < min)
			min = smooth[i];
	}

	max = smooth[pos_Dmax];

	for(i = pos_Dmax; i > (pos_Dmax - MAX_WINDOW) && i > 0; i--){
		if(smooth[i] > max)
			max = smooth[i];
	}

	shiftBuffer(pulse->Max, N_PROM);
	shiftBuffer(pulse->Min, N_PROM);

	pulse->Min[0] = min;
	pulse->Max[0] = max;

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

void shiftBuffer(float *buffer, uint16_t length)
{
	uint16_t i;
	for(i = length-1; i > 0; i--)
		buffer[i]=buffer[i-1];
}
