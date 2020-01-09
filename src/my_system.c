/*
 * my_system.c
 *
 *  Created on: 21 oct. 2019
 *      Author: ordonezt
 */


#include "my_include.h"

void initSystem(void)
{
	initTimer();	//Inicializo el timer cada 1 ms
	initGpio();		//Inicializo entradas y salidas
	initADC();		//Inicializo el ADC
	initUart();
	setLedState(AWAKE);
	led = IR;
	setLed(led);
	initOLED();
	tick = 0;

	RingBuffer_Init(&RingBuffSmooth[RED], &smooth[RED], sizeof(smooth[RED][0]), BUFFER_LENGTH);
	RingBuffer_Init(&RingBuffSmooth[IR], &smooth[IR], sizeof(smooth[IR][0]), BUFFER_LENGTH);
}
