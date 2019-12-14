/*
 * my_measure.c
 *
 *  Created on: Dec 13, 2019
 *      Author: jreiszer
 */
#include "my_include.h"

void Calculate(pulse_t *pulse) {
	bpm = calculateBPM(pulse[RED].Delta);
}

uint8_t calculateBPM(uint8_t deltaN)
{
	static uint8_t memory[NUMBER_OF_BPMS];
	static uint8_t index = 0;
	uint8_t new;
	uint8_t bp = bpm;

	new = 60000 / (deltaN * SAMPLE_PERIOD);

	if(new < MAX_BPM_ACEPTED && new >= MIN_BPM_ACEPTED) {
//		memory[index] = new;
//		index++;
//		index %= NUMBER_OF_BPMS;
		bp = new;
	}
	return bp; //Average(memory, NUMBER_OF_BPMS);
}

uint8_t calculateSpO2(pulse_t *Data[])
{
	uint8_t i = 0;

#if(PROM == PROM_R)

	float R = 0;
	uint8_t SpO2 = 0;

	for(i = 0; i < N_PROM; i++)
		R += log10(Data[RED]->Max[i] / Data[RED]->Min[i]) / log10(Data[IR]->Max[i] / Data[IR]->Min[i]);

	R = (R / N_PROM);

	SpO2 = A - (B * R);

#endif

#if(PROM == PROM_SPO2)

	float R = 0;
	uint16_t SpO2 = 0;

		for(i = 0; i < SAMPLES_LENGTH; i++)
	{
		R = log10(MAX[RED][i] / MIN[RED][i]) / log10(MAX[IR][i] / MIN[IR][i]);
		SpO2 += A - (B * R);
	}

	SpO2 /= SAMPLES_LENGTH;
#endif

	return SpO2;
}

uint8_t Average(uint8_t *array, uint8_t len){
	uint8_t i;
	uint16_t sum = 0;

	for(i=0; i<len; i++)
		sum += array[i];

	return sum/len;
}
