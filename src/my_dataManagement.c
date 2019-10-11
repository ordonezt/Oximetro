/*
 * my_dataManagement.c
 *
 *  Created on: 10 oct. 2019
 *      Author: ordonezt
 */

#include "my_include.h"

volatile uint16_t ADC_buff[BUFFER_HEIGHT][BUFFER_LENGTH] = {0};
volatile uint16_t ADC_index = 0;

uint16_t filter(uint16_t sample)
{
	return sample;
}

bool isFinger(void)
{
	int16_t prev = (int16_t)ADC_index - 3;

	prev = prev < 0? BUFFER_LENGTH + prev: prev;

	return ((ADC_buff[RED][ADC_index] >> 7) != (ADC_buff[RED][prev] >> 7));		//Solo uso para comparar los 6 bits mas significativos

}
