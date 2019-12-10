/*
 * filters.c
 *
 *  Created on: Dec 10, 2019
 *      Author: jreiszer
 */

#include "my_include.h"


const float h[N_RAW] =
{
		0.038626528332096295,
		0.14300345854663776,
		0.233752240231106,
		0.28624515754407337,
		0.233752240231106,
		0.14300345854663776,
		0.038626528332096295,
};

uint16_t taps[N_RAW] = {0};

float filter(uint16_t sample, const float* h, uint16_t* taps, uint8_t length)
{
	uint8_t n = 0;
	float y = 0;

	shiftBuffer(taps, length);
	taps[0] = sample;

	for(n = 0; n < length; n++)
		y += ((float)taps[n]) * h[n];

	return y;
}


