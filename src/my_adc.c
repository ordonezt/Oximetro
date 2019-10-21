/*
 * my_adc.c
 *
 *  Created on: 7 oct. 2019
 *      Author: brousse
 */


#include "my_include.h"

void initADC(void)
{
	ADMUX = 0b01000100;	//La referencia es AVcc interna, el resultado esta ajustado a la derecha y la entrada es ADC4 (PC4)
	ADCSRA = 0b10001011;//ADC habilitado, en pausa, interrupcion habilitada, fadc = 125 KHz, la conversion tarda 8 us maximo
	DIDR0 |= 0x01 << ADC4D;	//Desactivo el buffer de entrada para tener menos consumo
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
