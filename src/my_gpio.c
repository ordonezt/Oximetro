/*
 * my_gpio.c
 *
 *  Created on: 6 oct. 2019
 *      Author: brousse
 */

#include "my_include.h"

volatile key_t button = {0};

void initGpio(void)
{
	//Salidas
	DDRD |= 0x01 << DDD3;		//Pin D3 salida
	DDRC |= 0x01 << DDC5;		//Pin C5 salida

	//Entradas
	DDRD &= ~(0x01 << DDD2);	//Pin D2 es entrada
	PORTD |= 0x01 << PORTD2;	//Con pull-up
}

void debounce(void)
{
	static keyState_t prev = NO_PRESSED;
	keyState_t curr;
	static uint8_t counter_equals = 0;

	curr = ((PIND >> PIND2) & 0x01)? NO_PRESSED : PRESSED;

	counter_equals = (prev == curr)? counter_equals + 1 : 0;

	if(counter_equals == N_TIMES)
	{
		counter_equals = 0;
		if(button.state == NO_PRESSED && curr == PRESSED)
		{
			button.state = PRESSED;
			button.wasPressed = true;
		}
		else if(button.state == PRESSED && curr == NO_PRESSED)
		{
			button.state = NO_PRESSED;
			button.wasRelease = true;
		}
		else
			button.state = curr;
	}

	prev = curr;
}
