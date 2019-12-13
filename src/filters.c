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

float taps[BUFFER_HEIGHT][N_RAW] = {0};

float filter(float sample, const float* h, float* tapsLocal, uint8_t length)
{
	uint8_t n = 0;
	float y = 0;
	shiftBuffer(tapsLocal, length);
	tapsLocal[0] = sample;

	for(n = 0; n < length; n++)
		y += ((float)tapsLocal[n]) * h[n];

	return y;
}


