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
	led = RED;
	setLed(led);
	initOLED();
	tick = 0;
}
