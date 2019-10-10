/*
 * my_extInt.c
 *
 *  Created on: 4 oct. 2019
 *      Author: brousse
 */
#include "my_include.h"

void initExtInt(void)	//Boton con pull up a masa en PD2
{
	EICRA &= ~(0x03 << ISC00); 	//Interrupciones por low level
	EIMSK |= 0x01 << INT0;		//Activo interrupciones
}

void delExtInt(void)
{
	EIMSK &= ~(0x01 << INT0);		//Desactivo interrupciones
}

ISR(INT0_vect)
{
	sleep_disable();
	delExtInt();
	PORTC |= 0x01 << PORTC5;
}
