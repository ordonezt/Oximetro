/*
 * my_dataManagement.c
 *
 *  Created on: 10 oct. 2019
 *      Author: ordonezt
 */
#define ENVOLVENTE	0
#define MAXIMO		1
#define MODO ENVOLVENTE
#include "my_include.h"

extern RINGBUFF_T txring;

RINGBUFF_T RingBuffADC[BUFFER_HEIGHT];
RINGBUFF_T RingBuffSmooth[BUFFER_HEIGHT];

const uint8_t A = 110;
const uint8_t B = 25;

float smooth[BUFFER_HEIGHT][BUFFER_LENGTH] = {0};
float ThresholdSup = 0;
float ThresholdInf = 0;

volatile uint8_t cuenta_muestras = 0;
uint32_t cuenta_impresas = 0;

void process(pulse_t pulse[2]) //TODO que recibe que devuelve?? RECIBIMOS UN ARRAY DE *pulse y calculamos frecuencia de pulso y oxigeno y dejamos en globales
{
	uint8_t led_index;

	float data;
	uint32_t indice;

	for (led_index = RED; led_index < 2; led_index++)
	{
		data = filter((float)pulse[led_index].muestra, h, taps[led_index], N_RAW);
		RingBuffer_Insert(&RingBuffSmooth[led], &data);
	}

	if(flags.iniciando == TRUE && RingBuffer_IsFull(&RingBuffSmooth[RED]) && RingBuffer_IsFull(&RingBuffSmooth[IR])){
		flags.iniciando = FALSE;
		//Resetear el buffer smooth
		RingBuffSmooth[IR].tail = RingBuffSmooth[IR].head;
		RingBuffSmooth[RED].tail = RingBuffSmooth[RED].head;
		SetearThreshold(BUFFER_LENGTH);
//		RingBuffSmooth[IR].tail = 0;
//		RingBuffSmooth[IR].head = 0;
		pulse[IR].porDebajo = FALSE;
		pulse[IR].posCruce = 0;
	}


	if(!flags.iniciando){
		RingBuffer_Pop(&RingBuffSmooth[RED], &data); //Descartamos el valor del led rojo
		RingBuffer_Pop(&RingBuffSmooth[IR], &data);

#if MODO == MAXIMO
		if(pulse[IR].porDebajo && data > ThresholdSup){
			indice = RingBuffSmooth[IR].tail - 1;
			if(pulse[IR].posCruce){ //TODO ver que pasa cuando por casualidad el cruce está en cero
				pulse[IR].Delta = indice - pulse[IR].posCruce;
				flags.beat_detected;
				SetearThreshold(pulse[IR].Delta);
			}
			pulse[IR].posCruce = indice;
			pulse[IR].porDebajo = FALSE;
		}
#endif
#if MODO == ENVOLVENTE
		SetearThreshold2(data);
		if(pulse[IR].porDebajo && data > ThresholdSup){
			indice = RingBuffSmooth[IR].tail - 1;
			if(pulse[IR].posCruce){ //TODO ver que pasa cuando por casualidad el cruce está en cero
				pulse[IR].Delta = indice - pulse[IR].posCruce;
				flags.beat_detected;
			}
			pulse[IR].posCruce = indice;
			pulse[IR].porDebajo = FALSE;
		}
#endif
		if(data < ThresholdInf){
			pulse[IR].porDebajo = TRUE;
		}
	}

	cuenta_impresas++;
}

void SetearThreshold(uint32_t len){
	float max;

	max = BuscarMaximo(len);

	ThresholdSup = 0.6*max;
	ThresholdInf = 0.3*max;
}

float BuscarMaximo(uint32_t len){
	int i;
	float data;
	float max = 0;

	RingBuffSmooth[IR].tail -= len;
	for(i=0; i< len; i++){
		RingBuffer_Pop(RingBuffSmooth, &data);
		if(data > max){
			max = data;
		}
	}
	return max;
}

#if MODO == ENVOLVENTE
void SetearThreshold2(float data){
	static float envolvente, maximo;
	static int n = 0;

	if(envolvente <= data){
		envolvente = data;
		maximo = data;
		n = 0;
	}
	else{
		envolvente = maximo * exp(-0.001 * n);
		n++;
	}

	ThresholdSup = 0.6*envolvente;
	ThresholdInf = 0.3*envolvente; //TODO REVISAR VALORES EN AMBOS MODOS
}

#endif
//void get_min_max_values(pulse_t pulse[2]){
//	led_t led_local;
//	uint16_t i, min, max, pos_Dmax;
//
//	for (led_local = RED; led_local <= IR; led_local++) {
//
//		pos_Dmax = pulse[led_local].pos_Dmax;
//
//		min = smooth[led_local][pos_Dmax];
//
//		for(i = pos_Dmax; i < (pos_Dmax + MIN_WINDOW); i++){
//			if(smooth[led_local][i] < min)
//				min = smooth[led_local][i];
//		}
//
//		max = smooth[led_local][pos_Dmax];
//
//		for(i = pos_Dmax; i > (pos_Dmax - MAX_WINDOW) && i > 0; i--){
//			if(smooth[led_local][i] > max)
//				max = smooth[led_local][i];
//		}
//
//		shiftBuffer(pulse[led_local].Max, N_PROM);
//		shiftBuffer(pulse[led_local].Min, N_PROM);
//
//		pulse[led_local].Min[0] = min;
//		pulse[led_local].Max[0] = max;
//	}
//}
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
