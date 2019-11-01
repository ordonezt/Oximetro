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
#define BUFFER_LENGTH 	400	//2 seg de span
							//Tamaño del buffer 2*400*2B = 1,6 KB

#define N_RAW	7
#define N_GRADIENT 250
#define N_SMOOTH BUFFER_LENGTH

#define MAX_WINDOW 10
#define MIN_WINDOW 10
//-----------------------------------TIPOS DE DATOS-----------------------------

typedef struct {
	uint16_t Led;
	uint16_t Max;
	uint16_t Min;
	uint8_t pos_Dmax;
} pulse_t;

//-----------------------------------EXTERNAS-----------------------------------
extern const float h[N_RAW];
extern const uint8_t A;
extern const uint8_t B;
extern volatile uint8_t cuenta_muestras;

extern float raw[BUFFER_HEIGHT][N_RAW];
extern float smooth[BUFFER_HEIGHT][BUFFER_LENGTH];
extern float gradient[BUFFER_HEIGHT][N_GRADIENT];



//-----------------------------------PROTOTIPOS----------------------------------
bool isFinger(void);
uint8_t calculateSpO2(void);
uint8_t calculateBPM(void);
float filter (volatile float* x,const float* h, uint8_t length);
void shiftBuffer(float *buffer, uint16_t length);
void get_min_max_values(pulse_t *Data[]);
void process(pulse_t *pulse);

#endif /* MY_DATAMANAGMENT_H_ */
