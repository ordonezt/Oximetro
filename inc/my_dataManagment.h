/*
 * my_dataManagment.h
 *
 *  Created on: 10 oct. 2019
 *      Author: ordonezt
 */

#ifndef MY_DATAMANAGMENT_H_
#define MY_DATAMANAGMENT_H_

//----------------------------------CONSTANTES----------------------------------
#define BUFFER_HEIGHT	2
#define BUFFER_LENGTH 	8//512 //2^9 la razon de este tamaño es que tiene que ser potencia de dos para que anden las funciones de ring buffer
							//TODO VER SI SE PUEDE HACER MAS CHICO

//#define BUFFER_LENGTH 	400	//2 seg de span
							//Tamaño del buffer 2*400*2B = 1,6 KB

#define N_RAW	7
#define N_GRADIENT 250
#define N_SMOOTH BUFFER_LENGTH

#define N_FINGER	10
#define THRESHOLD	100		//TODO
#define DC_LEVEL	500		//TODO

#define MAX_WINDOW 10   // TODO ALCANZA???
#define MIN_WINDOW 10   //

#define N_PROM 	10

//-----------------------------------TIPOS DE DATOS-----------------------------

typedef struct {
	uint16_t Led;
	uint16_t muestra;
	float Max[N_PROM];
	float Min[N_PROM];
	uint32_t pos_Cruce;
	uint32_t Delta;
	uint8_t porDebajo;
} pulse_t;

//-----------------------------------EXTERNAS-----------------------------------
extern const uint8_t A;
extern const uint8_t B;
extern volatile uint8_t cuenta_muestras;

extern float raw[BUFFER_HEIGHT][N_RAW];
extern float smooth[BUFFER_HEIGHT][BUFFER_LENGTH];
extern float gradient[BUFFER_HEIGHT][N_GRADIENT];

extern RINGBUFF_T RingBuffADC[BUFFER_HEIGHT];
extern RINGBUFF_T RingBuffSmooth[BUFFER_HEIGHT];

//-----------------------------------PROTOTIPOS----------------------------------
uint8_t checkFinger(void);
void shiftBuffer(float *buffer, uint16_t length);
void get_min_max_values(pulse_t *pulse);
void process(pulse_t *pulse);
float BuscarMaximo(void);
void SetearThreshold(void);

#endif /* MY_DATAMANAGMENT_H_ */
