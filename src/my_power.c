/*
 * my_power.c
 *
 *  Created on: 4 oct. 2019
 *      Author: brousse
 */
#include "my_include.h"

void goToSleep(void)
{
	//Apagar ADC
	//Poner puertos como entradas pull down

	//Activar sleep

//	SMCR |= 0x01 << SE;	//Activo la instruccion sleep
//	sei();
//	__asm__ __volatile__("sleep");
	initExtInt();
	PORTC &= ~(0x01 << PORTC5);
	PORTD &= ~(0x01 << PORTD3);
	sleep_enable();
	sei();
	sleep_cpu();
}
