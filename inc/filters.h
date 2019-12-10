/*
 * filters.h
 *
 *  Created on: Dec 10, 2019
 *      Author: jreiszer
 */

#ifndef FILTERS_H_
#define FILTERS_H_

extern const float h[N_RAW];
extern uint16_t taps[N_RAW];

float filter(uint16_t sample, const float* h, uint16_t* taps, uint8_t length);

#endif /* FILTERS_H_ */
