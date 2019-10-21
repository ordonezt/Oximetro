/*
 * my_adc.c
 *
 *  Created on: 7 oct. 2019
 *      Author: brousse
 */


#include "my_include.h"

void initADC(void)
{
	//La referencia es AVcc interna, el resultado esta ajustado a la derecha y la entrada es ADC3 (PC3)
	ADMUX = (1 << REFS0) | (0 << ADLAR) | (3 << MUX0);
	//ADC habilitado, en pausa, auto trigger deshabilitado, interrupcion habilitada, prescaler = 8 fadc = 125 KHz, la conversion tarda 104us, 200us la primera
	ADCSRA = (1 << ADEN) | (0 << ADSC) | (0 << ADATE) | (1 << ADIE) | (3 << ADPS0);
	//Desactivo el buffer digital de entrada para tener menos consumo
	DIDR0 |= 0x01 << ADC3D;
}

ISR(ADC_vect)
{
	static uint8_t mini_counter = 0;

	flags.conversion_done = true;

	ADC_buff[led][ADC_index] = filter(ADCL | (ADCH << 8));	//Cuidado: el datasheet especifica primero leer ADCL y desp ADCH

	if(mini_counter)
	{
		mini_counter = 0;
		ADC_index++;
	}
	mini_counter++;

	if(ADC_index == BUFFER_LENGTH)
	{
		ADC_index = 0;
		mini_counter = 0;
		flags.sample_buffer_full = true;
	}
}
