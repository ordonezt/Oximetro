/*
 * my_dataManagement.c
 *
 *  Created on: 10 oct. 2019
 *      Author: ordonezt
 */

#include "my_include.h"

volatile uint16_t ADC_buff[BUFFER_HEIGHT][BUFFER_LENGTH] = {0};

uint16_t filter(uint16_t sample)
{
	return sample;
}
