/*
 * my_dataManagment.h
 *
 *  Created on: 10 oct. 2019
 *      Author: ordonezt
 */

#ifndef MY_DATAMANAGMENT_H_
#define MY_DATAMANAGMENT_H_



#define BUFFER_HEIGHT	2
#define BUFFER_LENGTH 	400	//2 seg de span
//Tamaño del buffer 2*400*2B = 1,6 KB

#define N_RAW	5
#define N_GRADIENT 3
#define N_SMOOTH BUFFER_LENGTH

#define MAX_WINDOW 10
#define MIN_WINDOW 10

extern const float h[N_RAW];
extern const uint8_t A;
extern const uint8_t B;

extern volatile uint16_t raw[BUFFER_HEIGHT][N_RAW];
extern volatile uint16_t smooth[BUFFER_HEIGHT][BUFFER_LENGTH];
extern volatile uint16_t gradient[BUFFER_HEIGHT][N_GRADIENT];

bool isFinger(void);
uint8_t calculateSpO2(void);
uint8_t calculateBPM(void);
float filter (volatile uint16_t* x,const float* h, uint8_t length);
void shiftBuffer(volatile uint16_t *buffer, uint8_t length);
void processData(void);

#endif /* MY_DATAMANAGMENT_H_ */
