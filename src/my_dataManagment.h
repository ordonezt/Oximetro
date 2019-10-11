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

extern volatile uint16_t ADC_buff[BUFFER_HEIGHT][BUFFER_LENGTH];

extern volatile uint16_t ADC_index;

uint16_t filter(uint16_t sample);

bool isFinger(void);

#endif /* MY_DATAMANAGMENT_H_ */
