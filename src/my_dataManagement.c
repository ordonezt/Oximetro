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
float envolventeMax = 0, envolventeMin = 5000;

float globMax;
float globMin;

volatile uint8_t cuenta_muestras = 0;
uint32_t cuenta_impresas = 0;

void process(pulse_t pulse[2]) //TODO que recibe que devuelve?? RECIBIMOS UN ARRAY DE *pulse y calculamos frecuencia de pulso y oxigeno y dejamos en globales
{
	uint8_t led_index;

	float data;
	uint32_t indice;

	for (led_index = IR; led_index < 2; led_index++)
	{
		data = filter((float)pulse[led_index].muestra, h, taps[led_index], N_RAW);
		RingBuffer_Insert(&RingBuffSmooth[led_index], &data);
	}

	if(!flags.iniciando){
		RingBuffer_Pop(&RingBuffSmooth[IR], &data); //Descartamos el valor del led rojo

		if(RingBuffer_Pop(&RingBuffSmooth[RED], &data)){

#if MODO == MAXIMO
			if(pulse[RED].porDebajo && data > ThresholdSup){
				indice = RingBuffSmooth[RED].tail - 1;
				if(pulse[RED].posCruce){ //TODO ver que pasa cuando por casualidad el cruce está en cero
					pulse[RED].Delta = indice - pulse[RED].posCruce;
					flags.beat_detected = TRUE;
					SetearThreshold(pulse[RED].Delta);
				}
				pulse[RED].posCruce = indice;
				pulse[RED].porDebajo = FALSE;
			}
	#endif
	#if MODO == ENVOLVENTE
			SetearThreshold2(data);
			if(pulse[RED].porDebajo && data > ThresholdSup){
				indice = RingBuffSmooth[RED].tail - 1;
				if(pulse[RED].posCruce){ //TODO ver que pasa cuando por casualidad el cruce está en cero
					pulse[RED].Delta = indice - pulse[RED].posCruce;
					flags.beat_detected = TRUE;
				}
				pulse[RED].posCruce = indice;
				pulse[RED].porDebajo = FALSE;
			}
	#endif
			if(data < ThresholdInf){
				pulse[RED].porDebajo = TRUE;
			}
		}
	}

	if(flags.iniciando == TRUE && RingBuffer_IsFull(&RingBuffSmooth[IR]) && RingBuffer_IsFull(&RingBuffSmooth[RED])){
		flags.iniciando = FALSE;
		//Resetear el buffer smooth
		RingBuffSmooth[RED].tail = RingBuffSmooth[RED].head;
		RingBuffSmooth[IR].tail = RingBuffSmooth[IR].head;
		SetearThreshold(BUFFER_LENGTH);
//		RingBuffSmooth[IR].tail = 0;
//		RingBuffSmooth[IR].head = 0;
		pulse[RED].porDebajo = FALSE;
		pulse[RED].posCruce = 0;
	}

	cuenta_impresas++;
}

void SetearThreshold(uint32_t len){
	float max = 0;
	float min = 5000;
	float Amp;

	BuscarMaximo(len, &min, &max);
	globMax = max;
	globMin = min;

	Amp = max - min;
	ThresholdSup = min + 0.6 * Amp;
	ThresholdInf = min + 0.4 * Amp;
}

void BuscarMaximo(uint32_t len, float *min, float *max){
	int i;
	float data;

	RingBuffSmooth[RED].tail -= len;
	for(i=0; i< len; i++){
		RingBuffer_Pop(&RingBuffSmooth[RED], &data);
		if(data > *max){
			*max = data;
		}
		if(data < *min){
			*min = data;
		}
	}
	return;
}

#if MODO == ENVOLVENTE
void SetearThreshold2(float data){
	float Amp;
	//static float envolventeMax = 0, envolventeMin = 5000;
	static float maximo, minimo;
	static int n, n2 = 0;
	static float tao = 0;

//	tao = -2 / (float)(maximo - minimo);
	tao = TAO;

	if(envolventeMax <= data){
		envolventeMax = data;
		maximo = data;
		n = 0;
	}
	else{
		envolventeMax = ThresholdSup + (maximo - ThresholdSup) * exp(tao * n);
		n++;
	}
	if(envolventeMin >= data){
		envolventeMin = data;
		minimo = data;
		n2 = 0;
	}
	else{
		envolventeMin = ThresholdInf + (minimo - ThresholdInf) * exp(tao * n2);
		n2++;
	}
#if MODO != MAXIMO
	globMax = maximo;
	globMin = minimo;
#endif
	Amp = envolventeMax - envolventeMin;
	ThresholdSup = envolventeMin + 0.6 * Amp;
	ThresholdInf = envolventeMin + 0.3 * Amp;
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
