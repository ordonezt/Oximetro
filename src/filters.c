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


float filter (volatile float* x,const float* h, uint8_t length) // TODO modificar el filtro para que reciba su bufferr
{
	uint8_t n = 0;
	float y = 0;

	for(n = 0; n < length; n++)
		y += ((float)x[n]) * h[n];

	return y;
}


