/*
 * my_adc.h
 *
 *  Created on: 7 oct. 2019
 *      Author: brousse
 */

#ifndef MY_ADC_H_
#define MY_ADC_H_

#define BUFFER_HEIGHT	2
#define BUFFER_LENGTH 	400	//2 seg de span
//Tamaño del buffer 2*400*2B = 1,6 KB

extern volatile uint16_t ADC_buff[BUFFER_HEIGHT][BUFFER_LENGTH];

void initADC(void);

#endif /* MY_ADC_H_ */
