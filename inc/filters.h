/*
 * filters.h
 *
 *  Created on: Dec 10, 2019
 *      Author: jreiszer
 */

#ifndef FILTERS_H_
#define FILTERS_H_

extern const float h[N_RAW];
extern float taps[N_RAW];

float filter(float sample, const float* h, float* taps, uint8_t length);

#endif /* FILTERS_H_ */
