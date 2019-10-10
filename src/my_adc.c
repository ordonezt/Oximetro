/*
 * my_adc.c
 *
 *  Created on: 7 oct. 2019
 *      Author: brousse
 */


#include "my_include.h"

volatile uint16_t ADC_buff[BUFFER_HEIGHT][BUFFER_LENGTH] = {0};

void initADC(void)
{
	ADMUX = 0b01000100;	//La referencia es AVcc interna, el resultado esta ajustado a la derecha y la entrada es ADC4
	ADCSRA = 0b10001101;//ADC habilitado, en pausa, interrupcion habilitada, fadc = 31,250 KHz, la conversion tarda 0,8 ms maximo
	DIDR0 |= 0x01 << ADC4D;	//Desactivo el buffer de entrada para tener menos consumo
}

ISR(ADC_vect)
{
	static uint8_t i = 0;

	ADC_buff[led][i] = (ADCH << 8) | ADCL;

	i++;
	i %= BUFFER_LENGTH;
}
