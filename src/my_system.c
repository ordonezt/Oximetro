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
	initPWM();		//Inicializo el PWM
//	set_sleep_mode(SLEEP_MODE_PWR_DOWN);//Selecciono power-down mode
	setLedState(AWAKE);
	setLed(IR);
}
